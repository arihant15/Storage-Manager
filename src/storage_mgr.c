#include "storage_mgr.h"
#include <sys/stat.h>
#include <stdio.h>


/* manipulating page files */
int main()
{
	SM_FileHandle fh;
	SM_PageHandle ph;
	int a;
	ph = (SM_PageHandle) malloc(PAGE_SIZE);

	initStorageManager();

	a = createPageFile("Hello.txt");
	printf("RC: %d Create Page file\n", a);

	a = openPageFile("Hello.txt", &fh);
	printf("RC: %d open Page file\n", a);

	a = closePageFile(&fh);
	printf("RC: %d file closed\n", a);

	//a = destroyPageFile("Hello.txt");
	//printf("RC: %d file destroyPageFile\n", a);

	a = readBlock(1,&fh,ph);
	printf("RC: %d read block\n", a);

	a = getBlockPos(&fh);
	printf("RC: %d block Position\n", a);

	a = readFirstBlock(&fh, ph);
	printf("RC: %d read First block\n", a);

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
	fHandle->curPagePos = fseek(fp,0,SEEK_SET);//should point to the
	//beginning of the current page
	fHandle->mgmtInfo = fp;
	return RC_OK;

}
RC closePageFile (SM_FileHandle *fHandle)
{
	if(fclose(fHandle->mgmtInfo) == 0)
		return RC_OK;
	else
		return RC_FILE_NOT_FOUND;
}
RC destroyPageFile (char *fileName)
{
	if(remove(fileName) == 0)
		return RC_OK;
	else
		return RC_FILE_NOT_FOUND;
}

/* reading blocks from disc */
RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	int i;
	if(pageNum > fHandle->totalNumPages || fHandle == NULL || fopen(fHandle->fileName,"r") == NULL)
		return RC_READ_NON_EXISTING_PAGE;
	fseek(fHandle->mgmtInfo , (pageNum-1)*PAGE_SIZE , SEEK_SET);
	for( i = 0; i < PAGE_SIZE; i++)
	{
		memPage = memPage + fgetc(fHandle->mgmtInfo);
	}
	fHandle->curPagePos = ftell(fHandle->mgmtInfo)/PAGE_SIZE;
	return RC_OK;
}

int getBlockPos (SM_FileHandle *fHandle)
{
	if(fHandle == NULL || fopen(fHandle->fileName,"r") == NULL)
		return RC_READ_NON_EXISTING_PAGE;
	return fHandle->curPagePos;
}

RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	//define a string buffer to read the data into
	int i;
	if(fHandle == NULL || fopen(fHandle->fileName,"r") == NULL)
		return RC_READ_NON_EXISTING_PAGE;
	fseek(fHandle->mgmtInfo , 0 , SEEK_SET);
	for( i = 0; i < PAGE_SIZE; i++)
	{
		memPage = memPage + fgetc(fHandle->mgmtInfo);
	}
	fHandle->curPagePos = ftell(fHandle->mgmtInfo)/PAGE_SIZE;
	return RC_OK;
}

RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
 int i;
        if(fHandle == NULL || fopen(fHandle->fileName,"r") == NULL)
                return RC_READ_NON_EXISTING_PAGE;
	if(fseek(fHandle->mgmtInfo , -PAGE_SIZE , SEEK_CUR)!=0)
		return RC_READ_NON_EXISTING_PAGE;
	else
		if(fread(memPage, 1, PAGE_SIZE, fHandle->mgmtInfo)==-1)
		return RC_READ_NON_EXISTING_PAGE;
		else
		fHandle->curPagePos = ftell(fHandle->mgmtInfo)/PAGE_SIZE;
		return RC_OK;
}
RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
 int i;
        if(fHandle == NULL || fopen(fHandle->fileName,"r") == NULL)
                return RC_READ_NON_EXISTING_PAGE;
        if(fseek(fHandle->mgmtInfo , 0 , SEEK_CUR)!=0)
                return RC_READ_NON_EXISTING_PAGE;
        else
                if(fread(memPage, 1, PAGE_SIZE, fHandle->mgmtInfo)==-1)
                return RC_READ_NON_EXISTING_PAGE;
                else
		fHandle->curPagePos = ftell(fHandle->mgmtInfo)/PAGE_SIZE;
                return RC_OK;

}
RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
 int i;
        if(fHandle == NULL || fopen(fHandle->fileName,"r") == NULL)
                return RC_READ_NON_EXISTING_PAGE;
        if(fseek(fHandle->mgmtInfo , PAGE_SIZE , SEEK_CUR)!=0)
                return RC_READ_NON_EXISTING_PAGE;
        else
                if(fread(memPage, 1, PAGE_SIZE, fHandle->mgmtInfo)==-1)
                return RC_READ_NON_EXISTING_PAGE;
                else
		fHandle->curPagePos = ftell(fHandle->mgmtInfo)/PAGE_SIZE;
                return RC_OK;
}

RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
 int i;
        if(fHandle == NULL || fopen(fHandle->fileName,"r") == NULL)
                return RC_READ_NON_EXISTING_PAGE;
        if(fseek(fHandle->mgmtInfo , -PAGE_SIZE , SEEK_END)!=0)
                return RC_READ_NON_EXISTING_PAGE;
        else
                if(fread(memPage, 1, PAGE_SIZE, fHandle->mgmtInfo)==-1)
                return RC_READ_NON_EXISTING_PAGE;
                else
		fHandle->curPagePos = ftell(fHandle->mgmtInfo)/PAGE_SIZE;
                return RC_OK;
}

/* writing blocks to a page file */
RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage);
        
RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage);

RC appendEmptyBlock (SM_FileHandle *fHandle);
RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle);

