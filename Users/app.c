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
int main(){
	  /*外设串口的初始化*/
      uart1Init(115200);       /**/
	  /*初始化延时函数*/
      delay_init();
        /*将硬盘立即挂载到文件系统*/
      if(f_mount(&f,"1:",1)==FR_NO_FILESYSTEM){
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
      else if(res !=FR_OK){
           printf("文件系统挂载失败，请查找设备芯片问题！\r\n");
      }
      else{
            printf("文件系统挂载成功，可以进行读写测试\r\n");
      }
      res=f_open(&f_t,"1:\test.txt",FA_OPEN_ALWAYS);    /*强制打开并且创建一个文件*/
      if(!res){ printf("文件成功被打开:res=%d\r\n",res);}
      else{printf("文件没有成功被打开res=%d！\r\n",res);}
	  while(1){
//      delay_ms(1000);
//      w25q128TestConnection();
	  }
}
