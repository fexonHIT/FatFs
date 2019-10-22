/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
//#include "usbdisk.h"	/* Example: Header file of existing USB MSD control module */
//#include "atadrive.h"	/* Example: Header file of existing ATA harddisk control module */
//#include "sdcard.h"		/* Example: Header file of existing MMC/SDC contorl module */
#include  "app.h"
/* Definitions of physical drive number for each drive */
  #define ATA		0	/* Example: Map ATA harddisk to physical drive 0 */
//#define MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
//#define USB		2	/* Example: Map USB MSD to physical drive 2 */
  #define SPI_FLASH     1  /* Example: Map ATA harddisk to physical drive 1 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS status =STA_NOINIT;
    uint8_t res=0;
	switch (pdrv) {
    case ATA:	/* SD CARD */
	break;
	case SPI_FLASH :
		res = w25q128TestConnection();   /*�����1��ʾ����ͨ��*/
		 if(res){
            /*�豸ID��ȡ��ȷ*/
            status &=~STA_NOINIT;
         }
         else{
            /*�豸ID��ȡ����*/
             status=STA_NOINIT;
         }
         break;
     default:
            status=STA_NOINIT;
          
	}
	     return status;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS status=STA_NOINIT;
	int i=500;
	switch (pdrv) {
    case ATA :
		return status;
	case SPI_FLASH :
		spi5Init(); /*��SPIFLASH���г�ʼ��*/
        while(i--);     /*��ʱһС��ʱ��*/
    /*��SPI FLASH���л���*/
       spiFlashWakeup();
    /*��ȡоƬ��״̬*/
       status=disk_status(SPI_FLASH);
       break;
    default:
        status=STA_NOINIT;

	}
	return status;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
	switch (pdrv) {
	case ATA :
	 break;
	case SPI_FLASH :
     sector +=1536;
     w25q128ReadData(buff,sector<<12,count<<12);
     return RES_OK;                         /*���سɹ���ȡ�ı�־*/
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	switch (pdrv) {
	case ATA :
    return res;
	case SPI_FLASH :
        /*��������*/
        sector +=1536;
        w25q128EraseSector(sector<<12);
        w25q128WriteBuffer((uint8_t *)buff,sector<<12,count<<12);
		return res;
	}

	return RES_PARERR;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	switch (pdrv) {
	case ATA :
		break;
    case SPI_FLASH:
        switch(cmd){
            case GET_SECTOR_COUNT:
                *(DWORD*)buff= 2560;      /*һ��4096������,������ǰ���6MB�ֽڣ�Ҳ����1536������*/
            break;
            case GET_SECTOR_SIZE:
                *(WORD*)buff=4096;
                 break;
            case GET_BLOCK_SIZE:
                *(DWORD*)buff=1;                /*һ�β�����д���ٸ���������������һ�β�����д1������*/
                 break;
        }
        res=RES_OK;
        break;
        default :
        res=RES_PARERR;      
	}
	return res;
}
#endif
__weak DWORD get_fattime(void) {
	/* ���ص�ǰʱ��� */
	return	  ((DWORD)(2015 - 1980) << 25)	/* Year 2015 */
			| ((DWORD)1 << 21)				/* Month 1 */
			| ((DWORD)1 << 16)				/* Mday 1 */
			| ((DWORD)0 << 11)				/* Hour 0 */
			| ((DWORD)0 << 5)				  /* Min 0 */
			| ((DWORD)0 >> 1);				/* Sec 0 */
}
