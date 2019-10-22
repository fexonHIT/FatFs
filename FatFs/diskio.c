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
		res = w25q128TestConnection();   /*如果是1表示测试通过*/
		 if(res){
            /*设备ID读取正确*/
            status &=~STA_NOINIT;
         }
         else{
            /*设备ID读取错误*/
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
		spi5Init(); /*对SPIFLASH进行初始化*/
        while(i--);     /*延时一小段时间*/
    /*对SPI FLASH进行唤醒*/
       spiFlashWakeup();
    /*获取芯片的状态*/
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
     return RES_OK;                         /*返回成功读取的标志*/
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
        /*擦除扇区*/
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
                *(DWORD*)buff= 2560;      /*一共4096个扇区,不操作前面的6MB字节，也就是1536个扇区*/
            break;
            case GET_SECTOR_SIZE:
                *(WORD*)buff=4096;
                 break;
            case GET_BLOCK_SIZE:
                *(DWORD*)buff=1;                /*一次操作读写多少个扇区，这里设置一次操作读写1个扇区*/
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
	/* 返回当前时间戳 */
	return	  ((DWORD)(2015 - 1980) << 25)	/* Year 2015 */
			| ((DWORD)1 << 21)				/* Month 1 */
			| ((DWORD)1 << 16)				/* Mday 1 */
			| ((DWORD)0 << 11)				/* Hour 0 */
			| ((DWORD)0 << 5)				  /* Min 0 */
			| ((DWORD)0 >> 1);				/* Sec 0 */
}
