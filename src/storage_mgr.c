#include "storage_mgr.h"
#include <sys/stat.h>


/* manipulating page files */
int main()
{
	SM_FileHandle fh;
	int a;
	initStorageManager();
	createPageFile("Hello.txt");
	openPageFile("Hello.txt", &fh);
	a = closePageFile(&fh);
	printf("%d file closed\n", a);
	a = destroyPageFile("Hello.txt");
	printf("%d file closed\n", a);
}
void initStorageManager (void)
{
	printf("Storage Manager Initialized!!\n");

}
RC createPageFile (char *fileName)
{
	FILE *fp;
	int i;
	fp = fopen(fileName, "w");
	if(fp == NULL)
		return RC_FILE_NOT_FOUND;
	fseek(fp , 0 , SEEK_SET);
	for(i = 0; i < PAGE_SIZE; i++)
		{
			
			fwrite("\0",1, 1,fp);
			fseek(fp,0,SEEK_END);
		}
	return RC_OK;

}
RC openPageFile (char *fileName, SM_FileHandle *fHandle)
{
	FILE *fp;
	fp = fopen(fileName, "r");
	if(fp == NULL)
		return RC_FILE_NOT_FOUND;
	struct stat fileStat;
	stat(fileName,&fileStat);
	int fileSize = fileStat.st_size;
	fHandle->fileName = fileName;
	fHandle->totalNumPages = fileSize/PAGE_SIZE;
	fHandle->curPagePos = fseek(fp,0,SEEK_SET);
	fHandle->mgmtInfo = fp;
	return RC_OK;

}
RC closePageFile (SM_FileHandle *fHandle)
{
	if(fclose(fHandle->mgmtInfo) == 0)
		return RC_OK;
	else
		return 10;
}
RC destroyPageFile (char *fileName)
{
	if(remove(fileName) == 0)
		return RC_OK;
	else
		return 10;
}

/* reading blocks from disc */
RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage);
int getBlockPos (SM_FileHandle *fHandle);
RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage);

RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage);
RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage);
RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage);
RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage);

/* writing blocks to a page file */
RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage);
RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage);

RC appendEmptyBlock (SM_FileHandle *fHandle);
RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle);

