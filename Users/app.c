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

uint8_t w_buf[]="����FatFS�ļ�ϵͳ";
uint8_t r_buf[1024];
uint32_t bw;
uint32_t br;
int main(){
	  /*���贮�ڵĳ�ʼ��*/
      uart1Init(115200);       /**/
	  /*��ʼ����ʱ����*/
      delay_init();
        /*��Ӳ���������ص��ļ�ϵͳ*/
      res=f_mount(&f,"1:",1);
      if(res==FR_NO_FILESYSTEM){
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
       if(res !=FR_OK){
           printf("�ļ�ϵͳ����ʧ�ܣ�������豸оƬ���⣡res=%d\r\n",res);
       }
       else{
            printf("�ļ�ϵͳ���سɹ������Խ��ж�д����\r\n");
       }
      res=f_open(&f_t,"1:/test.txt",FA_OPEN_ALWAYS|FA_WRITE|FA_READ);    /*ǿ�ƴ򿪲��Ҵ���һ���ļ�*/
      if(!res){ printf("�ļ��ɹ�����!\r\n");}
      else{printf("�ļ�û�гɹ�����res=%d��\r\n",res);}
      /*��ӡ���ļ�ϵͳ���ļ��Ĵ�С*/
      printf("�ļ�ϵͳ��С:%d,�ļ���С%d\r\n",sizeof(f),sizeof(f_t));
      res=f_write(&f_t,w_buf,sizeof(w_buf),&bw);
      printf("д��Ľ��res=%d",res);
      res=f_lseek(&f_t,0);          /*����ڶ�����ƫ����*/
      printf("f_lseek=%d\r\n",res);
      f_read(&f_t,r_buf,sizeof(w_buf),&br);
      printf("%s",r_buf);           /*��д����ļ���ȡ����*/
      //printf("")
      /*�رյ�ǰ�ļ�*/
      f_close(&f_t);
      while(1){
      
      }
	  }

