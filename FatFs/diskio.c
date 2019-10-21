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
	int result;
	switch (pdrv) {
    case ATA:	/* SD CARD */
	break;
	case SPI_FLASH :
		 result = w25q128TestConnection();   /*如果是1表示测试通过*/
		// translate the reslut code here
         if(result) return 0;                /*成功检测到硬盘*/       
         else return STA_NODISK;             /*没有检测到硬盘*/
	}
	     return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;
	switch (pdrv) {
    case ATA :
		return stat;
	case SPI_FLASH :
		result = spi5Init(); /*对SPIFLASH进行初始化*/
        stat=disk_status(SPI_FLASH);
		return stat;
	}
	return STA_NOINIT;
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
	DRESULT res;
	switch (pdrv) {
	case ATA :
	 return res;
	case SPI_FLASH :
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
		return res;
    case SPI_FLASH:
        switch(cmd){
            case GET_SECTOR_COUNT:
                *(DWORD*)buff= 4096-1536;      /*一共4096个扇区,不操作前面的6MB字节，也就是1536个扇区*/
            break;
            case GET_SECTOR_SIZE:
                *(DWORD*)buff=4096;
            case GET_BLOCK_SIZE:
                *(DWORD*)buff=1;                /*一次操作读写多少个扇区，这里设置一次操作读写1个扇区*/
            break;
        }
        return RES_OK;
	}
	return RES_PARERR;
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
