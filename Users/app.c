#include "app.h"
uint8_t srcBuf[256]={0};
uint8_t dstBuf[256]={0};
/*比较缓存是否相等*/
uint8_t bufferCompare(uint8_t *src,uint8_t *dst,uint16_t len){
           for(int i=0;i<len;++i){
                   if(*(src+i)!=*(dst+i)) return 0;
                   printf("0x%x ",dst[i]);
                   if(i%16==15) printf("\r\n");
           }
           return 1;
}
/*对源缓存进行初始化*/
void initSrcBuf(void){
     for(int i=0;i<512;++i){
        srcBuf[i]=i;printf("0x%x ",srcBuf[i]);
        if(i%16==15)printf("\r\n");
     }
}
/*定义文件系统的文件句柄*/
FATFS f;
FRESULT res;
/*定义一个文件句柄*/
FIL  f_t;
/*定义逻辑设备的名字*/
#define  PATH  "1:"

uint8_t w_buf[]="测试FatFS文件系统";
uint8_t r_buf[1024];
uint32_t bw;
uint32_t br;
int main(){
	  /*外设串口的初始化*/
      uart1Init(115200);       /**/
	  /*初始化延时函数*/
      delay_init();
        /*将硬盘立即挂载到文件系统*/
      res=f_mount(&f,"1:",1);
      if(res==FR_NO_FILESYSTEM){
         /*如果还没有文件系统就先格式化分区创建文件系统*/
          res=f_mkfs("1:",0,0);
          if(res==FR_OK){
          /*格式化之后，先取消挂载*/
              res=f_mount(NULL,"1:",1);
          /*重新对文件系统进行挂载*/
              res=f_mount(&f,"1:",1);
          }
           else{
               printf("格式化失败!\r\n");
           }   
      }
       if(res !=FR_OK){
           printf("文件系统挂载失败，请查找设备芯片问题！res=%d\r\n",res);
       }
       else{
            printf("文件系统挂载成功，可以进行读写测试\r\n");
       }
      res=f_open(&f_t,"1:/test.txt",FA_OPEN_ALWAYS|FA_WRITE|FA_READ);    /*强制打开并且创建一个文件*/
      if(!res){ printf("文件成功被打开!\r\n");}
      else{printf("文件没有成功被打开res=%d！\r\n",res);}
      /*打印出文件系统和文件的大小*/
      printf("文件系统大小:%d,文件大小%d\r\n",sizeof(f),sizeof(f_t));
      res=f_write(&f_t,w_buf,sizeof(w_buf),&bw);
      printf("写入的结果res=%d",res);
      res=f_lseek(&f_t,0);          /*相对于顶部的偏移量*/
      printf("f_lseek=%d\r\n",res);
      f_read(&f_t,r_buf,sizeof(w_buf),&br);
      printf("%s",r_buf);           /*将写入的文件读取出来*/
      //printf("")
      /*关闭当前文件*/
      f_close(&f_t);
      while(1){
      
      }
	  }

