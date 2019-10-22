#include "spi.h"
/*
Author:HIT_XiaoWu
Date:2019/10/19
Description:�Դ���FLASH��SPI����ѡ�˽��г�ʼ��
*/
uint8_t  spi5Init(void){
		{ 
			  /*��SPI�����ʱ�ӽ���ʹ��*/
			 RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5,ENABLE);
		    /*ʹ��GPIO���ŵ�ʱ��*/
			  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
		}
		 /*��ʼ��GPIO��ص�����  PF7 SCK PF8 MISO PF9 MOSI*/
		  GPIO_InitTypeDef  gpio;
		  gpio.GPIO_Mode=GPIO_Mode_AF;
		  gpio.GPIO_OType=GPIO_OType_PP;
		  gpio.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
		  gpio.GPIO_PuPd=GPIO_PuPd_NOPULL;
		  gpio.GPIO_Speed=GPIO_Speed_50MHz;
		  GPIO_Init(GPIOF,&gpio);
		 /*����Ƭѡ�˵��ź�*/
		  gpio.GPIO_Mode=GPIO_Mode_OUT;
		  gpio.GPIO_OType=GPIO_OType_PP;
		  gpio.GPIO_Pin=GPIO_Pin_6;
		  GPIO_Init(GPIOF,&gpio);
		  /*�������ŵĸ��ù���*/
	      GPIO_PinAFConfig(GPIOF,GPIO_PinSource7,GPIO_AF_SPI5);
		  GPIO_PinAFConfig(GPIOF,GPIO_PinSource8,GPIO_AF_SPI5);
		  GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_SPI5);
      /*����SPI5 ������ģʽ3*/
         SPI_InitTypeDef spi;
		 spi.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2;     /*��ʱSPI��ͨ�����ʴﵽ��22.5MHz*/
		 /*������ģʽ3����һ�����ز���������״̬�ĵ�ƽΪ�ߵ�ƽ*/
		 spi.SPI_CPHA=SPI_CPHA_2Edge;
		 spi.SPI_CPOL=SPI_CPOL_High;
		 /*CRC�������ʽ*/
		 spi.SPI_CRCPolynomial=7;
		 /*SPI����С���䵥λΪһ��Byte*/
		 spi.SPI_DataSize=SPI_DataSize_8b;
		 /*����SPI������˫��ȫ˫��*/
		 spi.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
		 /*����SPI�����ڸ�λ����*/
		 spi.SPI_FirstBit=SPI_FirstBit_MSB;
		 /*��������ķ�ʽ����Ƭѡ�ź�*/
		 spi.SPI_NSS=SPI_NSS_Soft;
		 /*SPI��������ģʽ*/
		 spi.SPI_Mode=SPI_Mode_Master;
		 SPI_Init(SPI5,&spi);
		 SPI_Cmd(SPI5,ENABLE);  /*ʹ��SPI��ͨ��*/	 
		 /*�ر�FLASH��Ƭѡ�ź�*/
		 SPI_FLASH_STOP();
         /*����W25Q128*/
         spiFlashWakeup();
         return 1;
}
static uint32_t spiTimeOut=0;
/*
Author:HIT_XiaoWu
Date:2019/10/19
Description:SPI�˿ڷ���һ���ֽ�,���Ӷ˿ڽ���һ���ֽ�
*/
uint8_t spiSendByte(SPI_TypeDef *spi,uint8_t buf){
    spiTimeOut=SPIT_FLAG_TIMEOUT;
	/*�ȴ��������*/
	while(SPI_I2S_GetFlagStatus(spi,SPI_I2S_FLAG_TXE)==RESET){
     if(spiTimeOut--==0) return spiTimeOutUserCallBack(0);     /*�ȴ���ʱ���ҷ��ش������*/
    }
		/*����һ���ֽ�*/
	SPI_I2S_SendData(spi,(uint8_t)buf);
	/*�ȴ����ݼĴ������ݵĵ���*/
 while(SPI_I2S_GetFlagStatus(spi,SPI_I2S_FLAG_RXNE)==RESET){
        if(spiTimeOut--==0) return spiTimeOutUserCallBack(1);     /*�ȴ���ʱ���ҷ��ش������*/
     }
	 return SPI_I2S_ReceiveData(spi);
}
/*
Author:HIT_XiaoWu
Date:2019/10/19
Description:SPI�˿ڷ���һ�����֣����Ӷ˿ڽ���һ������
*/
uint16_t spiSendHalfWord(SPI_TypeDef *spi,uint16_t buf){
	/*�ȴ��������*/
	while(SPI_I2S_GetFlagStatus(spi,SPI_I2S_FLAG_TXE)==RESET){
      if(spiTimeOut--==0) return spiTimeOutUserCallBack(2);     /*�ȴ���ʱ���ҷ��ش������*/
    }
		/*����һ������*/
	SPI_I2S_SendData(spi,(uint16_t)buf);
	/*�ȴ����ݼĴ������ݵĵ���*/
  while(SPI_I2S_GetFlagStatus(spi,SPI_I2S_FLAG_RXNE)==RESET){
       if(spiTimeOut--==0) return spiTimeOutUserCallBack(3);     /*�ȴ���ʱ���ҷ��ش������*/
  }
	return SPI_I2S_ReceiveData(spi);
}
/*
Author:HIT_XiaoWu
Date:2019/10/19
Description:��ȡw25q128��оƬID
*/
uint32_t w25q128GetDevciceId(void){
      uint8_t temp0,temp1,temp2;
      /*��ʼͨ��*/
      SPI_FLASH_START();
      spiSendByte(FLASH_SPI,W25X_JedecDeviceID);   
      temp0=spiSendByte(FLASH_SPI,DUMMY_BYTE);          /*����0xEF*/
      temp1=spiSendByte(FLASH_SPI,DUMMY_BYTE);           /*����0x40*/
      temp2=spiSendByte(FLASH_SPI,DUMMY_BYTE);           /*����0x18*/
      /*ֹͣͨ��*/
      SPI_FLASH_STOP();
      return ((uint32_t)temp0<<16)|((uint32_t)temp1<<8)|((uint32_t)temp2);
}
/*
Author:HIT_XiaoWu
Date:2019/10/19
Description:����W25Q128���������
*/
uint8_t w25q128TestConnection(void){
           if(w25q128GetDevciceId()==FLASH_DEVICE_ID){
              FLASH_INFO("%s","correctly W25Q128 device ID\r\n");
              return 1;
           }
           else{
              FLASH_INFO("%s","cannot attach to W25Q128,try check you cable connection!\r\n");
           }
           return 0;
}
/*
Author:HIT_XiaoWu
Date:2019/10/19
Description��W25Q128��дʹ��
*/
void  w25q128WriteEnable(void){
       /*��ʼͨ��*/
       SPI_FLASH_START();
       spiSendByte(FLASH_SPI,W25X_WriteEnable);
       /*ֹͣͨ��*/
       SPI_FLASH_STOP();
}
/*
Author:HIT_XiaoWu
Date:2019/10/19
Description���ȴ�W25Q128æµ�������
*/
void  w25q128WaitForWrtieEnd(void){
      spiTimeOut=SPIT_LONG_TIMEOUT;
      /*��ʼͨ��*/
      SPI_FLASH_START();
      /*���Ͷ�ȡ״̬�Ĵ���ָ��*/
      spiSendByte(FLASH_SPI,W25X_ReadStatusReg);
      uint8_t flashStatus=0;
      do{
            flashStatus=spiSendByte(FLASH_SPI,DUMMY_BYTE);
            if(spiTimeOut--==0) {spiTimeOutUserCallBack(4); return;}
      }while((flashStatus&WIP_Flag)==SET);     /**/
      /*ֹͣͨ��*/
      SPI_FLASH_STOP();
}
/*
Author:HIT_XiaoWu
Date:2019/10/19
Description��FLASH��������ָ��
*/
void w25q128EraseSector(uint32_t addr){
    /*����дʹ��ָ��*/
    w25q128WriteEnable();
    /*�ȴ��������*/
    w25q128WaitForWrtieEnd();
    /*�ȴ��ڲ�æµ����*/
    //w25q128WaitForWrtieEnd();
       /*��ʼͨ��*/
    SPI_FLASH_START();
    /*������������ָ��*/
    spiSendByte(FLASH_SPI,W25X_SectorErase);
//    /*��������������ַ�ĸ�λ*/
    spiSendByte(FLASH_SPI,(addr&0x00FF0000)>>16);
    /*��������������ַ����λ*/
    spiSendByte(FLASH_SPI,(addr&0x00FF00)>>8);
    /*��������������ַ�ĵ�λ*/
    spiSendByte(FLASH_SPI,(addr&0x0000FF));
     /*ֹͣͨ��*/
    SPI_FLASH_STOP();
    /*�ȴ��������*/
    w25q128WaitForWrtieEnd();
}
/*
Author:HIT_XiaoWu
Date:2019/10/20
Description��FLASHҳд�롣Ҫ��Ŀ������������֮�󣬶����Ӧ�������������ݵ�д��
ʹ��ҳд���ָ�һ����ഫ��256Bytes,�����ͳ���256���ֽڣ����ɾ������ǰ�淢�͵�����
*/
void w25q128WritePage(uint8_t* buf,uint32_t addr ,uint16_t len){
     /*дʹ���ź�*/
     w25q128WriteEnable();
     /*�ȴ��������*/
     w25q128WaitForWrtieEnd();
     /*��ʼͨ��*/
     SPI_FLASH_START();
    /*����дҳָ��*/
     spiSendByte(FLASH_SPI,W25X_PageProgram);
    /*��������������ַ����λ*/
     spiSendByte(FLASH_SPI,(addr&0xFF0000)>>16);
     spiSendByte(FLASH_SPI,(addr&0x00FF00)>>8);
    /*��������������ַ�ĵ�λ*/
     spiSendByte(FLASH_SPI,(addr&0x0000FF));
    if(len>W25Q128_PAGE_SIZE){
       len=W25Q128_PAGE_SIZE;
       FLASH_ERROR("attempt to write over than 256Bytes once��");
    }
    while(len--){
        spiSendByte(FLASH_SPI,*buf);
        buf++;
    }
     /*ֹͣͨ��*/
     SPI_FLASH_STOP();
     /*�ȴ�д�����*/
    w25q128WaitForWrtieEnd();
}
/*
Author:HIT_XiaoWu
Date:2019/10/20
Description��FLASH�ڴ�Ķ�ȡ
*/
void w25q128ReadData(uint8_t *buf,uint32_t addr,uint16_t len){
     /*��ʼͨ��*/
    SPI_FLASH_START();
    /*���Ͷ�ȡָ��*/
    spiSendByte(FLASH_SPI,W25X_ReadData);
     /*��������������ַ�ĸ�λ*/
    //spiSendByte(FLASH_SPI,(addr&0xFF000000)>>24);
    /*��������������ַ����λ*/
    spiSendByte(FLASH_SPI,(addr&0x00FF0000)>>16);
    spiSendByte(FLASH_SPI,(addr&0x0000FF00)>>8);
    /*��������������ַ�ĵ�λ*/
    spiSendByte(FLASH_SPI,(addr&0x000000FF));
    /*��ʼ��ȡ����*/
    while(len--){
       *buf=spiSendByte(FLASH_SPI,DUMMY_BYTE);
        buf++;
    }
    /*ֹͣͨ��*/
    SPI_FLASH_STOP();

}
/*
Author:HIT_XiaoWu
Date:2019/10/19
Description:�ϵ�֮����W25Q128
*/
void spiFlashWakeup(void){
     /*��ʼͨ��*/
     SPI_FLASH_START();
     spiSendByte(FLASH_SPI,W25X_ReleasePowerDown);
     /*ֹͣͨ��*/
     SPI_FLASH_STOP();

}

/*
Author:HIT_XiaoWu
Date:2019/10/19
Description:W25Q128���������ݵ�д��
*/
void w25q128WriteBuffer(uint8_t *buf,uint32_t addr,uint16_t len){
     uint8_t numOfPage =0,numOfSingle=0,Addr=0,count=0,temp=0;
     Addr=addr%W25Q128_PAGE_SIZE;
     /*�����������ٸ��ֽڿ��Զ��뵽ҳ��ַ*/
     count=W25Q128_PAGE_SIZE-Addr;
     /*����Ҫд���ٸ�����ҳ*/
     numOfPage=len/W25Q128_PAGE_SIZE;
     /*��������еĲ���һ��ҳ���ֽ���*/
     numOfSingle=len%W25Q128_PAGE_SIZE;
     if(Addr==0){
        /*����һҳ*/
       if(numOfPage==0){
          w25q128WritePage(buf,addr,len);
       }
       else
       {
           /*�Ȱ�����ҳд��*/
           while(numOfPage--){
           w25q128WritePage(buf,addr,W25Q128_PAGE_SIZE);
              buf+=W25Q128_PAGE_SIZE;
              addr+=W25Q128_PAGE_SIZE;
           }
           /*ʣ�µĲ���һҳ�����ݰ���ֱ�ӽ���д��*/
           w25q128WritePage(buf,addr,numOfSingle);
       }
     }
      else{
      /*����ַ������*/
          if(numOfPage==0){
            if(numOfSingle>count){
               /*��д�굱ǰҳ*/
              w25q128WritePage(buf,addr,count);
              temp=numOfSingle-count;buf +=count;addr +=count;
              /*��дʣ�������*/
              w25q128WritePage(buf,addr,temp);
            }
            else w25q128WritePage(buf,addr,count);
          }
          else{
              /*����ж�ҳ���ֽ���ǰҳд��*/
              w25q128WritePage(buf,addr,count);
              len -=count;
              numOfPage=len/W25Q128_PAGE_SIZE;
              numOfSingle=len%W25Q128_PAGE_SIZE;
              /*����ǰҳ*/
              w25q128WritePage(buf,addr,count);
              buf +=count;
              addr +=count;
              /*д����ҳ*/
              while(numOfPage--){
                w25q128WritePage(buf,addr,W25Q128_PAGE_SIZE);
                buf  += W25Q128_PAGE_SIZE;
                addr += W25Q128_PAGE_SIZE;
              }
              /*�������ʣ����ֽ�*/
              if(numOfSingle) w25q128WritePage(buf,addr,numOfSingle);    
          }
      }
}
//#define SPI_FLASH_PageSize 256
//void w25q128WriteBuffer(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
//{
//  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
//	
//	/*mod�������࣬��writeAddr��SPI_FLASH_PageSize��������������AddrֵΪ0*/
//  Addr = WriteAddr % SPI_FLASH_PageSize;
//	
//	/*��count������ֵ���պÿ��Զ��뵽ҳ��ַ*/
//  count = SPI_FLASH_PageSize - Addr;	
//	/*�����Ҫд��������ҳ*/
//  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
//	/*mod�������࣬�����ʣ�಻��һҳ���ֽ���*/
//  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

//	 /* Addr=0,��WriteAddr �պð�ҳ���� aligned  */
//  if (Addr == 0) 
//  {
//		/* NumByteToWrite < SPI_FLASH_PageSize */
//    if (NumOfPage == 0) 
//    {
//      w25q128WritePage(pBuffer, WriteAddr, NumByteToWrite);
//    }
//    else /* NumByteToWrite > SPI_FLASH_PageSize */
//    {
//			/*�Ȱ�����ҳ��д��*/
//      while (NumOfPage--)
//      {
//        w25q128WritePage(pBuffer, WriteAddr, SPI_FLASH_PageSize);
//        WriteAddr +=  SPI_FLASH_PageSize;
//        pBuffer += SPI_FLASH_PageSize;
//      }
//			
//			/*���ж���Ĳ���һҳ�����ݣ�����д��*/
//      w25q128WritePage(pBuffer, WriteAddr, NumOfSingle);
//    }
//  }
//	/* ����ַ�� SPI_FLASH_PageSize ������  */
//  else 
//  {
//		/* NumByteToWrite < SPI_FLASH_PageSize */
//    if (NumOfPage == 0) 
//    {
//			/*��ǰҳʣ���count��λ�ñ�NumOfSingleС��д����*/
//      if (NumOfSingle > count) 
//      {
//        temp = NumOfSingle - count;
//				
//				/*��д����ǰҳ*/
//        w25q128WritePage(pBuffer, WriteAddr, count);
//        WriteAddr +=  count;
//        pBuffer += count;
//				
//				/*��дʣ�������*/
//        w25q128WritePage(pBuffer, WriteAddr, temp);
//      }
//      else /*��ǰҳʣ���count��λ����д��NumOfSingle������*/
//      {				
//        w25q128WritePage(pBuffer, WriteAddr, NumByteToWrite);
//      }
//    }
//    else /* NumByteToWrite > SPI_FLASH_PageSize */
//    {
//			/*��ַ����������count�ֿ������������������*/
//      NumByteToWrite -= count;
//      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
//      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

//      w25q128WritePage(pBuffer, WriteAddr, count);
//      WriteAddr +=  count;
//      pBuffer += count;
//			
//			/*������ҳ��д��*/
//      while (NumOfPage--)
//      {
//        w25q128WritePage(pBuffer, WriteAddr, SPI_FLASH_PageSize);
//        WriteAddr +=  SPI_FLASH_PageSize;
//        pBuffer += SPI_FLASH_PageSize;
//      }
//			/*���ж���Ĳ���һҳ�����ݣ�����д��*/
//      if (NumOfSingle != 0)
//      {
//        w25q128WritePage(pBuffer, WriteAddr, NumOfSingle);
//      }
//    }
//  }
//}
/*
Author:HIT_XiaoWu
Date:2019/10/19
Description:�������ص�����
*/
static uint8_t spiTimeOutUserCallBack(uint8_t errCode){
        FLASH_ERROR("%d",errCode);
        return 0;    
}
