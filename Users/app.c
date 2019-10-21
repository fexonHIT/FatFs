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
/*�����ļ�ϵͳ���ļ����*/
FATFS f;
FRESULT res;
/*����һ���ļ����*/
FIL  f_t;
/*�����߼��豸������*/
#define  PATH  "1:"
int main(){
	  /*���贮�ڵĳ�ʼ��*/
      uart1Init(115200);       /**/
	  /*��ʼ����ʱ����*/
      delay_init();
        /*��Ӳ���������ص��ļ�ϵͳ*/
      if(f_mount(&f,"1:",1)==FR_NO_FILESYSTEM){
         /*�����û���ļ�ϵͳ���ȸ�ʽ�����������ļ�ϵͳ*/
          res=f_mkfs("1:",0,0);
          if(res==FR_OK){
          /*��ʽ��֮����ȡ������*/
              res=f_mount(NULL,"1:",1);
          /*���¶��ļ�ϵͳ���й���*/
              res=f_mount(&f,"1:",1);
          }
           else{
               printf("��ʽ��ʧ��!\r\n");
           }   
      }
      else if(res !=FR_OK){
           printf("�ļ�ϵͳ����ʧ�ܣ�������豸оƬ���⣡\r\n");
      }
      else{
            printf("�ļ�ϵͳ���سɹ������Խ��ж�д����\r\n");
      }
      res=f_open(&f_t,"1:\test.txt",FA_OPEN_ALWAYS);    /*ǿ�ƴ򿪲��Ҵ���һ���ļ�*/
      if(!res){ printf("�ļ��ɹ�����:res=%d\r\n",res);}
      else{printf("�ļ�û�гɹ�����res=%d��\r\n",res);}
	  while(1){
//      delay_ms(1000);
//      w25q128TestConnection();
	  }
}
