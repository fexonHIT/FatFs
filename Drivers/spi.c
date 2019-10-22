#include "spi.h"
/*
Author:HIT_XiaoWu
Date:2019/10/19
Description:对串行FLASH的SPI外设选端进行初始化
*/
uint8_t  spi5Init(void){
		{ 
			  /*对SPI外设的时钟进行使能*/
			 RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5,ENABLE);
		    /*使能GPIO引脚的时钟*/
			  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
		}
		 /*初始化GPIO相关的引脚  PF7 SCK PF8 MISO PF9 MOSI*/
		  GPIO_InitTypeDef  gpio;
		  gpio.GPIO_Mode=GPIO_Mode_AF;
		  gpio.GPIO_OType=GPIO_OType_PP;
		  gpio.GPIO_Pin=GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
		  gpio.GPIO_PuPd=GPIO_PuPd_NOPULL;
		  gpio.GPIO_Speed=GPIO_Speed_50MHz;
		  GPIO_Init(GPIOF,&gpio);
		 /*配置片选端的信号*/
		  gpio.GPIO_Mode=GPIO_Mode_OUT;
		  gpio.GPIO_OType=GPIO_OType_PP;
		  gpio.GPIO_Pin=GPIO_Pin_6;
		  GPIO_Init(GPIOF,&gpio);
		  /*配置引脚的复用功能*/
	      GPIO_PinAFConfig(GPIOF,GPIO_PinSource7,GPIO_AF_SPI5);
		  GPIO_PinAFConfig(GPIOF,GPIO_PinSource8,GPIO_AF_SPI5);
		  GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_SPI5);
      /*配置SPI5 工作在模式3*/
         SPI_InitTypeDef spi;
		 spi.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2;     /*此时SPI的通信速率达到了22.5MHz*/
		 /*工作在模式3，第一个边沿采样，空闲状态的电平为高电平*/
		 spi.SPI_CPHA=SPI_CPHA_2Edge;
		 spi.SPI_CPOL=SPI_CPOL_High;
		 /*CRC计算多项式*/
		 spi.SPI_CRCPolynomial=7;
		 /*SPI的最小传输单位为一个Byte*/
		 spi.SPI_DataSize=SPI_DataSize_8b;
		 /*配置SPI工作在双线全双工*/
		 spi.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
		 /*配置SPI工作在高位先行*/
		 spi.SPI_FirstBit=SPI_FirstBit_MSB;
		 /*采用软件的方式触发片选信号*/
		 spi.SPI_NSS=SPI_NSS_Soft;
		 /*SPI工作在主模式*/
		 spi.SPI_Mode=SPI_Mode_Master;
		 SPI_Init(SPI5,&spi);
		 SPI_Cmd(SPI5,ENABLE);  /*使能SPI的通信*/	 
		 /*关闭FLASH的片选信号*/
		 SPI_FLASH_STOP();
         /*唤醒W25Q128*/
         spiFlashWakeup();
         return 1;
}
static uint32_t spiTimeOut=0;
/*
Author:HIT_XiaoWu
Date:2019/10/19
Description:SPI端口发送一个字节,并从端口接收一个字节
*/
uint8_t spiSendByte(SPI_TypeDef *spi,uint8_t buf){
    spiTimeOut=SPIT_FLAG_TIMEOUT;
	/*等待发送完成*/
	while(SPI_I2S_GetFlagStatus(spi,SPI_I2S_FLAG_TXE)==RESET){
     if(spiTimeOut--==0) return spiTimeOutUserCallBack(0);     /*等待超时并且返回错误代码*/
    }
		/*发送一个字节*/
	SPI_I2S_SendData(spi,(uint8_t)buf);
	/*等待数据寄存器数据的到达*/
 while(SPI_I2S_GetFlagStatus(spi,SPI_I2S_FLAG_RXNE)==RESET){
        if(spiTimeOut--==0) return spiTimeOutUserCallBack(1);     /*等待超时并且返回错误代码*/
     }
	 return SPI_I2S_ReceiveData(spi);
}
/*
Author:HIT_XiaoWu
Date:2019/10/19
Description:SPI端口发送一个半字，并从端口接收一个半字
*/
uint16_t spiSendHalfWord(SPI_TypeDef *spi,uint16_t buf){
	/*等待发送完成*/
	while(SPI_I2S_GetFlagStatus(spi,SPI_I2S_FLAG_TXE)==RESET){
      if(spiTimeOut--==0) return spiTimeOutUserCallBack(2);     /*等待超时并且返回错误代码*/
    }
		/*发送一个半字*/
	SPI_I2S_SendData(spi,(uint16_t)buf);
	/*等待数据寄存器数据的到达*/
  while(SPI_I2S_GetFlagStatus(spi,SPI_I2S_FLAG_RXNE)==RESET){
       if(spiTimeOut--==0) return spiTimeOutUserCallBack(3);     /*等待超时并且返回错误代码*/
  }
	return SPI_I2S_ReceiveData(spi);
}
/*
Author:HIT_XiaoWu
Date:2019/10/19
Description:获取w25q128的芯片ID
*/
uint32_t w25q128GetDevciceId(void){
      uint8_t temp0,temp1,temp2;
      /*开始通信*/
      SPI_FLASH_START();
      spiSendByte(FLASH_SPI,W25X_JedecDeviceID);   
      temp0=spiSendByte(FLASH_SPI,DUMMY_BYTE);          /*返回0xEF*/
      temp1=spiSendByte(FLASH_SPI,DUMMY_BYTE);           /*返回0x40*/
      temp2=spiSendByte(FLASH_SPI,DUMMY_BYTE);           /*返回0x18*/
      /*停止通信*/
      SPI_FLASH_STOP();
      return ((uint32_t)temp0<<16)|((uint32_t)temp1<<8)|((uint32_t)temp2);
}
/*
Author:HIT_XiaoWu
Date:2019/10/19
Description:测试W25Q128的链接情况
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
Description：W25Q128的写使能
*/
void  w25q128WriteEnable(void){
       /*开始通信*/
       SPI_FLASH_START();
       spiSendByte(FLASH_SPI,W25X_WriteEnable);
       /*停止通信*/
       SPI_FLASH_STOP();
}
/*
Author:HIT_XiaoWu
Date:2019/10/19
Description：等待W25Q128忙碌操作完成
*/
void  w25q128WaitForWrtieEnd(void){
      spiTimeOut=SPIT_LONG_TIMEOUT;
      /*开始通信*/
      SPI_FLASH_START();
      /*发送读取状态寄存器指令*/
      spiSendByte(FLASH_SPI,W25X_ReadStatusReg);
      uint8_t flashStatus=0;
      do{
            flashStatus=spiSendByte(FLASH_SPI,DUMMY_BYTE);
            if(spiTimeOut--==0) {spiTimeOutUserCallBack(4); return;}
      }while((flashStatus&WIP_Flag)==SET);     /**/
      /*停止通信*/
      SPI_FLASH_STOP();
}
/*
Author:HIT_XiaoWu
Date:2019/10/19
Description：FLASH扇区擦除指令
*/
void w25q128EraseSector(uint32_t addr){
    /*发送写使能指令*/
    w25q128WriteEnable();
    /*等待擦除完毕*/
    w25q128WaitForWrtieEnd();
    /*等待内部忙碌结束*/
    //w25q128WaitForWrtieEnd();
       /*开始通信*/
    SPI_FLASH_START();
    /*发送扇区擦除指令*/
    spiSendByte(FLASH_SPI,W25X_SectorErase);
//    /*发送扇区擦除地址的高位*/
    spiSendByte(FLASH_SPI,(addr&0x00FF0000)>>16);
    /*发送扇区擦除地址的中位*/
    spiSendByte(FLASH_SPI,(addr&0x00FF00)>>8);
    /*发送扇区擦除地址的低位*/
    spiSendByte(FLASH_SPI,(addr&0x0000FF));
     /*停止通信*/
    SPI_FLASH_STOP();
    /*等待擦除完毕*/
    w25q128WaitForWrtieEnd();
}
/*
Author:HIT_XiaoWu
Date:2019/10/20
Description：FLASH页写入。要在目标扇区被擦除之后，对相对应的扇区进行数据的写入
使用页写入的指令，一次最多传输256Bytes,若发送超过256个字节，则会删除覆盖前面发送的数据
*/
void w25q128WritePage(uint8_t* buf,uint32_t addr ,uint16_t len){
     /*写使能信号*/
     w25q128WriteEnable();
     /*等待操作完成*/
     w25q128WaitForWrtieEnd();
     /*开始通信*/
     SPI_FLASH_START();
    /*发送写页指令*/
     spiSendByte(FLASH_SPI,W25X_PageProgram);
    /*发送扇区擦除地址的中位*/
     spiSendByte(FLASH_SPI,(addr&0xFF0000)>>16);
     spiSendByte(FLASH_SPI,(addr&0x00FF00)>>8);
    /*发送扇区擦除地址的低位*/
     spiSendByte(FLASH_SPI,(addr&0x0000FF));
    if(len>W25Q128_PAGE_SIZE){
       len=W25Q128_PAGE_SIZE;
       FLASH_ERROR("attempt to write over than 256Bytes once！");
    }
    while(len--){
        spiSendByte(FLASH_SPI,*buf);
        buf++;
    }
     /*停止通信*/
     SPI_FLASH_STOP();
     /*等待写入完毕*/
    w25q128WaitForWrtieEnd();
}
/*
Author:HIT_XiaoWu
Date:2019/10/20
Description：FLASH内存的读取
*/
void w25q128ReadData(uint8_t *buf,uint32_t addr,uint16_t len){
     /*开始通信*/
    SPI_FLASH_START();
    /*发送读取指令*/
    spiSendByte(FLASH_SPI,W25X_ReadData);
     /*发送扇区擦除地址的高位*/
    //spiSendByte(FLASH_SPI,(addr&0xFF000000)>>24);
    /*发送扇区擦除地址的中位*/
    spiSendByte(FLASH_SPI,(addr&0x00FF0000)>>16);
    spiSendByte(FLASH_SPI,(addr&0x0000FF00)>>8);
    /*发送扇区擦除地址的低位*/
    spiSendByte(FLASH_SPI,(addr&0x000000FF));
    /*开始读取数据*/
    while(len--){
       *buf=spiSendByte(FLASH_SPI,DUMMY_BYTE);
        buf++;
    }
    /*停止通信*/
    SPI_FLASH_STOP();

}
/*
Author:HIT_XiaoWu
Date:2019/10/19
Description:上电之后唤醒W25Q128
*/
void spiFlashWakeup(void){
     /*开始通信*/
     SPI_FLASH_START();
     spiSendByte(FLASH_SPI,W25X_ReleasePowerDown);
     /*停止通信*/
     SPI_FLASH_STOP();

}

/*
Author:HIT_XiaoWu
Date:2019/10/19
Description:W25Q128不定量数据的写入
*/
void w25q128WriteBuffer(uint8_t *buf,uint32_t addr,uint16_t len){
     uint8_t numOfPage =0,numOfSingle=0,Addr=0,count=0,temp=0;
     Addr=addr%W25Q128_PAGE_SIZE;
     /*计算出差出多少个字节可以对齐到页地址*/
     count=W25Q128_PAGE_SIZE-Addr;
     /*计算要写多少个整数页*/
     numOfPage=len/W25Q128_PAGE_SIZE;
     /*计算出所有的不满一个页的字节数*/
     numOfSingle=len%W25Q128_PAGE_SIZE;
     if(Addr==0){
        /*不满一页*/
       if(numOfPage==0){
          w25q128WritePage(buf,addr,len);
       }
       else
       {
           /*先把整数页写满*/
           while(numOfPage--){
           w25q128WritePage(buf,addr,W25Q128_PAGE_SIZE);
              buf+=W25Q128_PAGE_SIZE;
              addr+=W25Q128_PAGE_SIZE;
           }
           /*剩下的不满一页的数据包，直接将其写完*/
           w25q128WritePage(buf,addr,numOfSingle);
       }
     }
      else{
      /*若地址不对齐*/
          if(numOfPage==0){
            if(numOfSingle>count){
               /*先写完当前页*/
              w25q128WritePage(buf,addr,count);
              temp=numOfSingle-count;buf +=count;addr +=count;
              /*再写剩余的数据*/
              w25q128WritePage(buf,addr,temp);
            }
            else w25q128WritePage(buf,addr,count);
          }
          else{
              /*如果有多页，现将当前页写完*/
              w25q128WritePage(buf,addr,count);
              len -=count;
              numOfPage=len/W25Q128_PAGE_SIZE;
              numOfSingle=len%W25Q128_PAGE_SIZE;
              /*处理当前页*/
              w25q128WritePage(buf,addr,count);
              buf +=count;
              addr +=count;
              /*写整数页*/
              while(numOfPage--){
                w25q128WritePage(buf,addr,W25Q128_PAGE_SIZE);
                buf  += W25Q128_PAGE_SIZE;
                addr += W25Q128_PAGE_SIZE;
              }
              /*处理最后剩余的字节*/
              if(numOfSingle) w25q128WritePage(buf,addr,numOfSingle);    
          }
      }
}
//#define SPI_FLASH_PageSize 256
//void w25q128WriteBuffer(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
//{
//  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
//	
//	/*mod运算求余，若writeAddr是SPI_FLASH_PageSize整数倍，运算结果Addr值为0*/
//  Addr = WriteAddr % SPI_FLASH_PageSize;
//	
//	/*差count个数据值，刚好可以对齐到页地址*/
//  count = SPI_FLASH_PageSize - Addr;	
//	/*计算出要写多少整数页*/
//  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
//	/*mod运算求余，计算出剩余不满一页的字节数*/
//  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

//	 /* Addr=0,则WriteAddr 刚好按页对齐 aligned  */
//  if (Addr == 0) 
//  {
//		/* NumByteToWrite < SPI_FLASH_PageSize */
//    if (NumOfPage == 0) 
//    {
//      w25q128WritePage(pBuffer, WriteAddr, NumByteToWrite);
//    }
//    else /* NumByteToWrite > SPI_FLASH_PageSize */
//    {
//			/*先把整数页都写了*/
//      while (NumOfPage--)
//      {
//        w25q128WritePage(pBuffer, WriteAddr, SPI_FLASH_PageSize);
//        WriteAddr +=  SPI_FLASH_PageSize;
//        pBuffer += SPI_FLASH_PageSize;
//      }
//			
//			/*若有多余的不满一页的数据，把它写完*/
//      w25q128WritePage(pBuffer, WriteAddr, NumOfSingle);
//    }
//  }
//	/* 若地址与 SPI_FLASH_PageSize 不对齐  */
//  else 
//  {
//		/* NumByteToWrite < SPI_FLASH_PageSize */
//    if (NumOfPage == 0) 
//    {
//			/*当前页剩余的count个位置比NumOfSingle小，写不完*/
//      if (NumOfSingle > count) 
//      {
//        temp = NumOfSingle - count;
//				
//				/*先写满当前页*/
//        w25q128WritePage(pBuffer, WriteAddr, count);
//        WriteAddr +=  count;
//        pBuffer += count;
//				
//				/*再写剩余的数据*/
//        w25q128WritePage(pBuffer, WriteAddr, temp);
//      }
//      else /*当前页剩余的count个位置能写完NumOfSingle个数据*/
//      {				
//        w25q128WritePage(pBuffer, WriteAddr, NumByteToWrite);
//      }
//    }
//    else /* NumByteToWrite > SPI_FLASH_PageSize */
//    {
//			/*地址不对齐多出的count分开处理，不加入这个运算*/
//      NumByteToWrite -= count;
//      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
//      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

//      w25q128WritePage(pBuffer, WriteAddr, count);
//      WriteAddr +=  count;
//      pBuffer += count;
//			
//			/*把整数页都写了*/
//      while (NumOfPage--)
//      {
//        w25q128WritePage(pBuffer, WriteAddr, SPI_FLASH_PageSize);
//        WriteAddr +=  SPI_FLASH_PageSize;
//        pBuffer += SPI_FLASH_PageSize;
//      }
//			/*若有多余的不满一页的数据，把它写完*/
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
Description:出错代码回调函数
*/
static uint8_t spiTimeOutUserCallBack(uint8_t errCode){
        FLASH_ERROR("%d",errCode);
        return 0;    
}
