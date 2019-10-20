#include "app.h"
uint8_t srcBuf[256]={0};
uint8_t dstBuf[256]={0};
/*�Ƚϻ����Ƿ����*/
uint8_t bufferCompare(uint8_t *src,uint8_t *dst,uint16_t len){
           for(int i=0;i<len;++i){
                   if(*(src+i)!=*(dst+i)) return 0;
                   printf("0x%x ",dst[i]);
                   if(i%16==15) printf("\r\n");
           }
           return 1;
}
/*��Դ������г�ʼ��*/
void initSrcBuf(void){
     for(int i=0;i<512;++i){
        srcBuf[i]=i;printf("0x%x ",srcBuf[i]);
        if(i%16==15)printf("\r\n");
     }
}
int main(){
	  /*���贮�ڵĳ�ʼ��*/
	  uart1Init(115200);       /**/
	  /*��ʼ����ʱ����*/
       delay_init();
      /*��ʼ��SPI5*/
      spi5Init();
//      delay_ms(500);
      spiFlashWakeup();      /*����W25Q128*/
      w25q128TestConnection();
      printf("<*************************************>\r\n");
      /*��Դ������г�ʼ��*/
      initSrcBuf();
      /*����0��ַһ��4KB������*/
      w25q128EraseSector(0x000000);
      /*��0��ַдȥ���ݰ�*/
      w25q128WriteBuffer(srcBuf,0x000000,512);
      /*��0��ַ��ȡ���ݰ�*/
      w25q128ReadData(dstBuf,0x000000,512);
      /*��srcBuf��dstBuf��У��*/
      if(bufferCompare(srcBuf,dstBuf,512)) printf("W25Q128��д���Գɹ�!\r\n");
      else                                 printf("W25Q128��д����ʧ��!\r\n");     
	  while(1){
//      delay_ms(1000);
//      w25q128TestConnection();
	  }
}
