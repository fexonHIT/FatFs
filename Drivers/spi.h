#ifndef __SPI_H
#define __SPI_H
#include "app.h"
/*W25Q128的命令选项*/
#define W25X_WriteEnable 		   	0x06
#define W25X_WriteDisable 	   	    0x04
#define W25X_ReadStatusReg     	    0x05
#define W25X_WriteStatusReg    	    0x01
#define W25X_ReadData          	    0x03
#define W25X_FastReadData      	    0x0B
#define W25X_FastReadDual      	    0x3B
#define W25X_PageProgram       	    0x02
#define W25X_BlockErase        	    0xD8
#define W25X_SectorErase       	    0x20
#define W25X_ChipErase         	    0xC7
#define W25X_PowerDown         	    0xB9
#define W25X_ReleasePowerDown  	    0xAB
#define W25X_DeviceID          	    0xAB
#define W25X_ManufactDeviceID  	    0x90
#define W25X_JedecDeviceID     		0x9F
#define W25X_Enter4ByteMode    		0xB7
#define W25X_ReadStatusRegister3 	0x15
#define WIP_Flag                    0x01       
#define Dummy_Byte                  0xFF 
#define DUMMY_BYTE                  0xFF
#define FLASH_SPI                   SPI5
#define FLASH_DEVICE_ID             0xEF4018
#define SPIT_FLAG_TIMEOUT           10000
#define SPIT_LONG_TIMEOUT           (10*SPIT_FLAG_TIMEOUT)
/*W25Q128忙标志*/
#define WIP                         0x01
#define  W25Q128_PAGE_SIZE          256          /*W25Q128每页的字节数最大256字节*/
/*打印日志文件*/
#define FLASH_INFO(fmt,arg...)      printf("<---W25Q128 INFO--->"fmt"\r\n",##arg)
#define FLASH_ERROR(fmt,arg...)     printf("<----W25Q128 ERROR-->"fmt" at line[%d]\r\n",##arg,__LINE__);
#define FLASH_DEBUG_ON               1
#define FLASH_DEBUG(fmt,arg...)  do{ \
	                                 if(FLASH_DEBUG_ON)\
                                    printf("<---ERROR--DEBUG--->[%d]"fmt"\r\n",__LINE__,##arg);\
                                  }while(0)
                               
#define  SPI_FLASH_START()   (GPIOF->BSRRH=GPIO_Pin_6)
#define  SPI_FLASH_STOP()    (GPIOF->BSRRL=GPIO_Pin_6)
uint8_t  spi5Init(void);
static uint8_t spiTimeOutUserCallBack(uint8_t errCode);
FunctionalState w25q128TestConnection(void);
uint32_t w25q128GetDevciceId(void); 
void w25q128EraseSector(uint32_t addr);
void w25q128WritePage(uint8_t* buf,uint32_t addr ,uint16_t len); 
void w25q128ReadData(uint8_t *buf,uint32_t addr,uint16_t len); 
uint8_t w25q128TestConnection(void);    
void spiFlashWakeup(void); 
void w25q128WriteBuffer(uint8_t *buf,uint32_t addr,uint16_t len);  
uint32_t w25q128GetDevciceId(void);                                  
#endif /*__SPI_H*/
