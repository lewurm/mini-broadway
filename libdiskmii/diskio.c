/*
	diskio.c -- glue interface to ElmChan FAT FS driver. Part of the
	BootMii project.
 
Copyright (C) 2008, 2009	Haxx Enterprises <bushing@gmail.com>
Copyright (C) 2008, 2009	Sven Peter <svenpeter@gmail.com>
Copyright (C) 2009		Alex Marshall <SquidMan72@gmail.com>
 
# This code is licensed to you under the terms of the GNU GPL, version 2;
# see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
*/

#include <broadway.h>
#include <string.h>

/* Our stuff. */
#include <diskmii.h>
#include <diskmii/ff.h>
#include <diskmii/diskio.h>
#include <diskmii/cache.h>

int SD_disk_deinitialize();
int NAND_disk_deinitialize();

int SD_disk_initialize();
int NAND_disk_initialize();

int SD_disk_status();
int NAND_disk_status();

int SD_disk_read(BYTE *buff, DWORD sector, BYTE count);
int NAND_disk_read(BYTE *buff, DWORD sector, BYTE count);

int SD_disk_write(const BYTE *buff, DWORD sector, BYTE count);
int NAND_disk_write(const BYTE *buff, DWORD sector, BYTE count);

int SD_disk_ioctl(BYTE ctrl, BYTE *buff);
int NAND_disk_ioctl(BYTE ctrl, BYTE *buff);

BYTE buffer[512] ATTRIBUTE_ALIGN(32);

DRESULT get_resluts(int reslut)
{
	if(reslut == -1)
		return RES_PARERR;
	if(reslut == -2)
		return RES_ERROR;
	if(reslut == -3)
		return RES_WRPRT;
	if(reslut == -4)
		return RES_NOTRDY;
	return RES_OK;
}

DSTATUS get_satts(int reslut)
{
	if(reslut == -1)
		return STA_NOINIT;
	if(reslut == -2)
		return STA_NODISK;
	if(reslut == -3)
		return STA_PROTECT;
	return 0;
}

/*-----------------------------------------------------------------------*/
/* Deinidialize a Drive                                                  */
/* drv						Drive number.		 */

DSTATUS disk_deinitialize(BYTE drv)
{
	DSTATUS satt;
	int reslut = -1;
	
	switch (drv) {
		case ELM_SD:
			reslut = SD_disk_deinitialize();
			break;
		case ELM_NAND:
			reslut = NAND_disk_deinitialize();
			break;
	}
	satt = get_satts(reslut);
	return satt;
}

int SD_disk_deinitialize()
{
	return 0;
}

int NAND_disk_deinitialize()
{
	return 0;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/* drv						Drive number.		 */

DSTATUS disk_initialize(BYTE drv)
{
	DSTATUS satt;
	int reslut = -1;

	switch (drv) {
		case ELM_SD:
			reslut = SD_disk_initialize();
			break;
		case ELM_NAND:
			reslut = NAND_disk_initialize();
			break;
		default:
			break;
	}
	satt = get_satts(reslut);
	return satt;
}

int SD_disk_initialize()
{
	int reslut = 0;
	int state = sd_get_state();
	switch(state) {
		case SDMMC_NO_CARD:
			reslut = -2;
			break;
		case SDMMC_NEW_CARD:
			if(sd_mount())
				reslut = -1;
			else
				reslut = 0;
			break;
		default:
			reslut = 0;
			break;
	}
	return reslut;
}

int NAND_disk_initialize()
{
	return -2;
}

/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */
/* drv						Drive number.		 */

DSTATUS disk_status(BYTE drv)
{
	DSTATUS satt;
	int reslut = -1;

	switch (drv) {
		case ELM_SD:
			reslut = SD_disk_status();
			break;
		case ELM_NAND:
			reslut = NAND_disk_status();
			break;
	}
	satt = get_satts(reslut);
	return satt;
}

int SD_disk_status()
{
	int reslut = 0;
	int state = sd_get_state();
	
	switch (state) {
		case SDMMC_NO_CARD:
			reslut = -2;
			break;
		case SDMMC_NEW_CARD:
			reslut = -1;
			break;
		default:
			reslut = 0;
			break;
	}
	return reslut;
}

int NAND_disk_status()
{
	return -2;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/* drv						Drive number.		 */
/* buff						Data buffer.		 */
/* sector					Sector Address. (LBA)	 */
/* count					Sector Count. (1 - 255)	 */

DRESULT disk_read(BYTE drv, BYTE *buff, DWORD sector, BYTE count)
{
	DRESULT res;
	int reslut = -1;

	switch (drv) {
		case ELM_SD:
			reslut = SD_disk_read(buff, sector, count);
			break;
		case ELM_NAND:
			reslut = NAND_disk_read(buff, sector, count);
			break;
	}
	res = get_resluts(reslut);
	return res;
}

int SD_disk_read_core(BYTE *buff, DWORD sector, BYTE count)
{
	u32 i;
	int reslut = 0;
	
	if((count > 1) && (((u32)buff % 64) == 0)) {
		if(sd_read(sector, count, buff) != 0)
			reslut = -2;
		return reslut;
	}
	
	reslut = 0;
	for(i = 0; i < count; i++) {
		if(sd_read(sector + i, 1, buffer) != 0) {
			reslut = -2;
			break;
		}
		memcpy(buff + i * 512, buffer, 512);
	}
	return reslut;
}

int SD_disk_read(BYTE *buff, DWORD sector, BYTE count)
{
	int reslut = 0;
	if((count == 1) && (_libelmcache_read_from_cache(ELM_SD, sector, buff)))
		return RES_OK;
	if(count != 1)
		_libelmcache_invalidate_cache(ELM_SD, sector, count);
	reslut = SD_disk_read_core(buff, sector, count);
	if((reslut == 0) && (count == 1))
		_libelmcache_add_to_cache(ELM_SD, sector, buff);
	return reslut;
}

int NAND_disk_read(BYTE *buff, DWORD sector, BYTE count)
{
	(void)buff;
	(void)sector;
	(void)count;
	return -2;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/* drv						Drive number.		 */
/* buff						Data buffer.		 */
/* sector					Sector Address. (LBA)	 */
/* count					Sector Count. (1 - 255)	 */

#if _READONLY == 0
DRESULT disk_write(BYTE drv, const BYTE *buff, DWORD sector, BYTE count)
{
	DRESULT res;
	int reslut = -1;

	switch (drv) {
		case ELM_SD:
			reslut = SD_disk_write(buff, sector, count);
			break;
		case ELM_NAND:
			reslut = NAND_disk_write(buff, sector, count);
			break;
	}
	res = get_resluts(reslut);
	return res;
}

int SD_disk_write(const BYTE *buff, DWORD sector, BYTE count)
{
	u32 i;
	int reslut = 0;
	_libelmcache_invalidate_cache(ELM_SD, sector, count);
	if((count > 1) && (((u32)buff % 64) == 0)) {
		if(sd_write(sector, count, buff) != 0)
			reslut = -2;
		return reslut;
	}
	
	for(i = 0; i < count; i++) {
		memcpy(buffer, buff + i * 512, 512);
		if(sd_write(sector + i, 1, buffer) != 0) {
			reslut = -2;
			break;
		}
	}
	return reslut;
}

int NAND_disk_write(const BYTE *buff, DWORD sector, BYTE count)
{
	(void)buff;
	(void)sector;
	(void)count;
	return -2;
}

#endif /* _READONLY */

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/* drv						Drive number.		 */
/* ctrl						Control Code.		 */
/* buff						Data buffer.		 */

DRESULT disk_ioctl(BYTE drv, BYTE ctrl, void *buff)
{
	DRESULT res;
	int reslut = -1;

	switch (drv) {
		case ELM_SD:
			reslut = SD_disk_ioctl(ctrl, buff);
			break;
		case ELM_NAND:
			reslut = NAND_disk_ioctl(ctrl, buff);
			break;
	}
	res = get_resluts(reslut);
	return res;
}

int SD_disk_ioctl(BYTE ctrl, BYTE *buff)
{
	u32 *buff_u32 = (u32*)buff;
	int reslut = 0;
	
	switch (ctrl) {
		case CTRL_SYNC:
			break;
		case GET_SECTOR_COUNT:
			*buff_u32 = sd_getsize();
			break;
		case GET_SECTOR_SIZE:
			*buff_u32 = 512;
			break;
		case GET_BLOCK_SIZE:
			*buff_u32 = 512;
			break;
		default:
			reslut = -1;
			break;
	}
	return reslut;
}

int NAND_disk_ioctl(BYTE ctrl, BYTE *buff)
{
	(void)buff;
	(void)ctrl;
	return 0;
}

DWORD get_fattime()
{
	return 0;
}
