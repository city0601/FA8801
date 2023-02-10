#include "system_FITI_M4.h"
//#include "CM3DS_function.h"
#include <stdio.h>
//#include <math.h>
//#include <stdlib.h>
//#include <unistd.h>
#include "string.h"
//#include "rle.h"
//#include "data.h"

#define HW32_REG(ADDRESS)  (*((volatile unsigned long  *)(ADDRESS)))
#define HW16_REG(ADDRESS)  (*((volatile unsigned short *)(ADDRESS)))
#define HW8_REG(ADDRESS)   (*((volatile unsigned char  *)(ADDRESS)))
//--------------------------------------------------------------------------------
#define ADDR_c 0xa0000000
#define REG_ADDR_DMA        (ADDR_c + 0x0010)//(ADDR_c+0x0078)  //120 bytes, the 31th registers
#define REG_ADDR_CONV       (ADDR_c + 0x0020)//(ADDR_c+0x0078)  //120 bytes, the 31th registers

#define JOB_ORG_DEFINE _tmp_data_8M

//--------------------------------------------------------------------------------

typedef int8_t q7_t;
typedef int16_t q15_t;
typedef int32_t q31_t;
typedef int64_t q63_t;

    typedef struct S_RESIZE{   //only support square in, x2 out and channel is x16
       unsigned int addr_input_data;     //q7
       unsigned int addr_output_data;    //q7
       unsigned short input_x;
       unsigned short input_ch;
       unsigned short output_x;
       unsigned char reserve0[2];
       //16B
   } T_RESIZE;
   
//--------------------------------------------------------------------------------   
//static char _tmp_data_8K[128*1024] __attribute__ ((aligned (64)))={0};
//static char _tmp_data_8M[2*1024*1024] __attribute__ ((aligned (64)))={0};
	 static char _tmp_data_8M[128*1024] ={0};
//--------------------------------------------------------------------------------   
extern int zdma_fp_init(void);
extern int zdma_fp_send(void* dst, void* src, int size);

//--------------------------------------------------------------------------------
   void demo_c_resize(signed char* in, signed char* out, int in_x, int out_x, int ch)
   {
		 #if 0
        //int8_t resize[12*12*32];
        //int8_t placeholder[6*6*32];
        signed char* resize = out;
        signed char* placeholder = in;
        int line_in = in_x*ch;
        int line_out = out_x*ch;

        signed char* p0 = resize;
        signed char* p1 = placeholder;
        for(int y=0; y<in_x; y++) {
          for(int x=0; x<out_x; x++) {
              memcpy(p0 + x*ch, p1 + (x>>1)*ch, ch);
          }
          memcpy(p0+line_out, p0, line_out);
          p0 += line_out*2;//768;  // 2 lines
          p1 += line_in;//192;  // 1 lines

        }
		 #endif
  }//void demo_c_resize()
  
  void util_dla_job_send(unsigned int par0, unsigned int par1, unsigned int par2, unsigned int par3, void* flush_point, int flush_size)
   {
		 #if 0
	   //unsigned int _my_reg_x4[4] __attribute__ ((aligned (16)))={par0, par1, par2, par3};
		 unsigned int _my_reg_x4[4] ={par0, par1, par2, par3};
	   //unsigned int _my_reg_x4[4] __attribute__ ((aligned (16)))={0x01234567, 0x89abcdef, 0, 0};

	   //REG_MIX(_my_reg_x4, par0, 0x30<<16, 0, 0);

//			REG_MIX(_my_reg_x4, job_org, 0x30<<16, 0, 0);
//			Xil_DCacheFlushRange(job_org,  a_end - job_org);
//			Xil_DCacheInvalidateRange(job_org,   a_end - job_org);

	  // Xil_DCacheFlushRange(flush_point,		flush_size);
	   //Xil_DCacheFlushRange(_my_reg_x4,			16);
	   //Xil_DCacheInvalidateRange(flush_point,   flush_size);
#if 0
	   Xil_Out32(REG_ADDR_CONV, par0);
#else
//	   zdma_fp_send(REG_ADDR_DMA, _my_reg_x4, 16);
	   //zdma_fp_send(REG_ADDR_CONV, _my_reg_x4, 16);

	   memcpy((uint32_t*)REG_ADDR_DMA, _my_reg_x4, 16);
#endif
#endif
   }


   void util_dla_done_wait (void)
   {
	   int status=0;
	   while(1) {
	       status = HW32_REG(REG_ADDR_DMA+12);
	       if(status==1)
	           break;
	   }
   }

   int do_dut_test_resize(void)
   {
		 #if 0
       printf("%s\n", __FUNCTION__);

       int result=0;

       T_RESIZE rs;
       rs.input_x = 12;
       rs.output_x = 24;
       rs.input_ch = 16;

       int SIZE_TOT_IN0 = (rs.input_x*rs.input_x*rs.input_ch);
       int SIZE_OUT0 = (rs.output_x*rs.output_x*rs.input_ch);

       /*signed char *input = (signed char*)malloc(SIZE_TOT_IN0);
       signed char *output = (signed char*)malloc(SIZE_OUT0);
       signed char *c_output = (signed char*)malloc(SIZE_OUT0);*/
			
		 signed char *input ;
       signed char *output;
       signed char *c_output;

       // generate test input
       for(int i=0, a=0; i<SIZE_TOT_IN0; i++) {
           if(i % rs.input_ch == 0) {
               a++;
           }
           input[i]=a;
       }
       demo_c_resize(input, c_output, rs.input_x, rs.output_x, rs.input_ch);

       int job_org = (int)&JOB_ORG_DEFINE;
       int jobbass = job_org;
       int joboffset = sizeof(rs);
       unsigned int a_in                = jobbass+joboffset;
       unsigned int a_out               = a_in + SIZE_TOT_IN0;
       unsigned int a_end               = a_out + SIZE_OUT0;

       rs.addr_input_data = a_in;
       rs.addr_output_data = a_out;

       for(int i=0; i<SIZE_TOT_IN0; i++)
				HW32_REG(a_in + i) = input[i];
           //mem_helper_do_write(a_in + i, input[i]);

       char* p = (char*)&rs;
       for(int i=0; i<sizeof(rs); i++)
				HW32_REG(job_org+i) = p[i];
           //mem_helper_do_write(job_org+i, p[i]);

       // job-start
       util_dla_job_send(job_org, 0x31<<16, 0, 0, (void*)job_org, a_end-job_org);

       // wait for job
       util_dla_done_wait();

       //check result
       for(int i=0; i<SIZE_OUT0; i++) {
    	   q7_t* dla_out = (q7_t*)a_out;
    	   if(dla_out[i]!=c_output[i]) {
    		   printf("error: i %d c %d dla %d\n", i, c_output[i], dla_out[i]);
    	   }

       }

       free(input);
       free(output);
       free(c_output);
#endif
        //return result;
				return 0;
   }
   
   