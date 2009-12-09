/*****************************************************************************
 *  Cache for LibELM                                                         *
 *  By yellow.wood.goblin and SquidMan                                       *
 *****************************************************************************/

#include <broadway.h>
#include <diskmii/diskio.h>
#include <diskmii/ff.h>
#include <diskmii/cache.h>
#include <string.h>

/*-----------------------------------------------------------------------*/
/* CACHE                                                                 */

typedef struct {
	u8	data[_MAX_SS];
	int	valid;
	u32	sector;
	u32	stamp;
	u32	drive;
} CACHE;

static CACHE _cache[CACHE_SIZE];
static u32 accessCounter = 0;

static inline u32 stamp()
{
	return ++accessCounter;
}

int _libelmcache_read_from_cache(u32 drv, u32 sector, u8 *buff)
{
	int res = 0;
	int i;
	for(i = 0; i < CACHE_SIZE; i++) {
		if((_cache[i].valid) && (_cache[i].drive == drv) && (_cache[i].sector == sector)) {
			_cache[i].stamp = stamp();
			memcpy(buff, _cache[i].data, _MAX_SS);
			res = 1;
			break;
		}
	}
	return res;
}

void _libelmcache_add_to_cache(u32 drv, u32 sector, u8 *buff)
{
	int i;
	int oldest_item = -1;
	int free_item = -1;
	u32 oldest_stamp = ((u32)(((u32)0) - ((u32)1))); /* lolhax */
	for(i = 0; i < CACHE_SIZE; i++) {
		if(!_cache[i].valid) {
			free_item = i;
			break;
		}else if(_cache[i].stamp < oldest_stamp) {
			oldest_item = i;
			oldest_stamp = _cache[i].stamp;
		}
	}
	if(free_item < 0) free_item = oldest_item;
	if(free_item < 0) return;				/* ALGORITHM ERROR */
	memcpy(_cache[free_item].data, buff, _MAX_SS);
	_cache[free_item].valid  = 1;
	_cache[free_item].sector = sector;
	_cache[free_item].stamp  = stamp();
	_cache[free_item].drive  = drv;
}

void _libelmcache_invalidate_cache(u32 drv, u32 sector, u8 count)
{
	int i;
	u32 top = sector + count;
	for(i = 0; i < CACHE_SIZE; i++) {
		if((_cache[i].valid) && (_cache[i].drive == drv) && (_cache[i].sector >= sector) && (_cache[i].sector < top))
			_cache[i].valid = 0;
	}
}
