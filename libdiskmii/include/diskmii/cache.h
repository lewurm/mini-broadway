/*****************************************************************************
 *  Cache for LibELM                                                         *
 *  By yellow.wood.goblin and SquidMan                                       *
 *****************************************************************************/

#ifndef _CACHE_ELM

#ifdef __cplusplus
extern "C" {
#endif

#define CACHE_SIZE	8

int _libelmcache_read_from_cache(u32 drv, u32 sector, u8 *buff);
void _libelmcache_add_to_cache(u32 drv, u32 sector, u8 *buff);
void _libelmcache_invalidate_cache(u32 drv, u32 sector, u8 count);

#ifdef __cplusplus
};
#endif

#define _CACHE_ELM
#endif
