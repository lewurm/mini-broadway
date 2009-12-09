/*------------------------------------------------------------------------*/
/* OS independant sync objects. Using µSync.                              */
/* for LibELM  (C)SquidMan, 2009                                          */
/*------------------------------------------------------------------------*/

#include <broadway.h>
#include <diskmii/ffconf.h>
#include <usync.h>

#if _FS_REENTRANT

#define REQUEST_COUNT	10

/*------------------------------------------------------------------------*/
/* Create a Synchronization Object for a Volume                           */
/*------------------------------------------------------------------------*/
/* This function is called in f_mount function to create a new
/  synchronization object, such as semaphore and mutex. When a FALSE is
/  returned, the f_mount function fails with FR_INT_ERR.
*/

BOOL ff_cre_syncobj (	/* TRUE:Function succeeded, FALSE:Could not create due to any error */
	BYTE vol,	/* Corresponding logical drive being processed */
	_SYNC_t *sobj	/* Pointer to return the created sync object */
)
{
	*sobj = uSyncCreate(&vol);
	if(*sobj == NULL)
		return FALSE;
	return TRUE;
}



/*------------------------------------------------------------------------*/
/* Delete a Synchronization Object                                        */
/*------------------------------------------------------------------------*/
/* This function is called in f_mount function to delete a synchronization
/  object that created with ff_cre_syncobj function. When a FALSE is
/  returned, the f_mount function fails with FR_INT_ERR.
*/

BOOL ff_del_syncobj (	/* TRUE:Function succeeded, FALSE:Could not delete due to any error */
	_SYNC_t sobj	/* Sync object tied to the logical drive to be deleted */
)
{
	BOOL ret;
	ret = uSyncDelete(sobj);
	return ret;
}



/*------------------------------------------------------------------------*/
/* Request Grant to Access the Volume                                     */
/*------------------------------------------------------------------------*/
/* This function is called on entering file functions to lock the volume.
/  When a FALSE is returned, the file function fails with FR_TIMEOUT.
*/

BOOL ff_req_grant (	/* TRUE:Got a grant to access the volume, FALSE:Could not get a grant */
	_SYNC_t sobj	/* Sync object to wait */
)
{
	BOOL ret;
	int i;
	for(i = 0; i < REQUEST_COUNT; i++) {
		ret = (uSyncObtain(sobj) == NULL) ? FALSE : TRUE;
		if(ret == TRUE)
			break;
		usleep(100);
	}
	return ret;
}



/*------------------------------------------------------------------------*/
/* Release Grant to Access the Volume                                     */
/*------------------------------------------------------------------------*/
/* This function is called on leaving file functions to unlock the volume.
*/

void ff_rel_grant (
	_SYNC_t sobj	/* Sync object to be signaled */
)
{
	uSyncRelease(sobj);
}


#endif
