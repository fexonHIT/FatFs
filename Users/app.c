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
int main(){
	  /*外设串口的初始化*/
	  uart1Init(115200);       /**/
	  /*初始化延时函数*/
       delay_init();
      /*初始化SPI5*/
      spi5Init();
//      delay_ms(500);
      spiFlashWakeup();      /*唤醒W25Q128*/
      w25q128TestConnection();
      printf("<*************************************>\r\n");
      /*对源缓存进行初始化*/
      initSrcBuf();
      /*擦除0地址一个4KB的扇区*/
      w25q128EraseSector(0x000000);
      /*向0地址写去数据包*/
      w25q128WriteBuffer(srcBuf,0x000000,512);
      /*从0地址读取数据包*/
      w25q128ReadData(dstBuf,0x000000,512);
      /*对srcBuf和dstBuf做校验*/
      if(bufferCompare(srcBuf,dstBuf,512)) printf("W25Q128读写测试成功!\r\n");
      else                                 printf("W25Q128读写测试失败!\r\n");     
	  while(1){
//      delay_ms(1000);
//      w25q128TestConnection();
	  }
}
