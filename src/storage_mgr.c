#include "storage_mgr.h"
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


/* manipulating page files */
int main()
{
	SM_FileHandle fh;
	SM_PageHandle ph;
	int a,i;
	ph = (SM_PageHandle) malloc(PAGE_SIZE);
	
	initStorageManager();

	a = createPageFile("Hello.txt");
	printf("RC: %d Create Page file\n",a);
	
	a = openPageFile("Hello.txt", &fh);
	printf("RC: %d open Page file\n",a);

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

	a = readPreviousBlock(&fh, ph);
	printf("RC: %d read Previous block\n", a);

	a = readCurrentBlock(&fh, ph);
	printf("RC: %d read Current block\n", a);

	a = readNextBlock(&fh, ph);
	printf("RC: %d read Next block\n", a);

	a = readLastBlock(&fh, ph);
	printf("RC: %d read Last block\n", a);
	
	for (i=0; i < PAGE_SIZE; i++)
    		ph[i] = (i % 10) + '0';
	a = writeBlock(2,&fh,ph);
	printf("RC: %d write block\n", a);
/*
	a = writeCurrentBlock(&fh,ph);
	printf("RC: %d write current block\n", a);

	a = appendEmptyBlock(&fh);
	printf("RC: %d Append Empty Block!!\n", a);
	*/
}

void initStorageManager (void)
{
	printf("Storage Manager Initialized!!\n");

}

RC createPageFile (char *fileName)
{
	FILE *fp;
	int i;
	char ops;
	fp = fopen(fileName, "r");
	if(fp != NULL)
	{
		printf("File Already Present!!\nDo you want to over-write the file (y/n): ");
		scanf("%c",&ops);
		fclose(fp);
	}
	if(ops == 'y' || fp == NULL)
	{
		fp = fopen(fileName, "w");	
		fseek(fp , 0 , SEEK_SET);

		for(i = 0; i < PAGE_SIZE; i++)
		{
			//printf("4");
			fwrite("\0",1, 1,fp);
			fseek(fp,0,SEEK_END);
		}

		return RC_OK;
	}
	else
	{
		return RC_FILE_PRESENT;
	}
}

RC openPageFile(char *fileName, SM_FileHandle *fHandle)
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
	printf("Initital Pointer Position : %ld\n",ftell(fp));
	fHandle->curPagePos = (ftell(fp)/PAGE_SIZE) + 1;
	printf("Current Page Position : %d\n",fHandle->curPagePos);
	fHandle->mgmtInfo = fp;

	return RC_OK;

}

RC closePageFile (SM_FileHandle *fHandle)
{
	if(fHandle->mgmtInfo == NULL || fopen(fHandle->fileName,"r") == NULL)
		return RC_FILE_NOT_FOUND;
	switch(fclose(fHandle->mgmtInfo))
	{
		case 0: return RC_OK;
		default : return RC_FILE_NOT_FOUND;
	}
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
	if(fHandle == NULL || fopen(fHandle->fileName,"r") == NULL)
		return RC_FILE_NOT_FOUND;
	if(pageNum > fHandle->totalNumPages)
		return RC_READ_NON_EXISTING_PAGE;
	fseek(fHandle->mgmtInfo , (pageNum-1)*PAGE_SIZE , SEEK_SET);
	if(fread(memPage, 1, PAGE_SIZE, fHandle->mgmtInfo) != PAGE_SIZE)
		return RC_FILE_READ_ERROR;
	fHandle->curPagePos = (ftell(fHandle->mgmtInfo)/PAGE_SIZE) + 1;
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
	int i;
	if(fHandle == NULL || fopen(fHandle->fileName,"r") == NULL)
		return RC_FILE_NOT_FOUND;
	fseek(fHandle->mgmtInfo , 0 , SEEK_SET);
	if(fread(memPage, 1, PAGE_SIZE, fHandle->mgmtInfo) != PAGE_SIZE)
		return RC_FILE_READ_ERROR;
	fHandle->curPagePos = (ftell(fHandle->mgmtInfo)/PAGE_SIZE) + 1;
	return RC_OK;
}

RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
 	int i;
        if(fHandle == NULL || fopen(fHandle->fileName,"r") == NULL)
		return RC_FILE_NOT_FOUND;
	if(fHandle->curPagePos < 1)
		return RC_READ_NON_EXISTING_PAGE;
	fseek(fHandle->mgmtInfo , -PAGE_SIZE , SEEK_CUR);
	if(fread(memPage, 1, PAGE_SIZE, fHandle->mgmtInfo) != PAGE_SIZE)
		return RC_FILE_READ_ERROR;
	fHandle->curPagePos = (ftell(fHandle->mgmtInfo)/PAGE_SIZE) + 1;
	return RC_OK;
}

RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
 	int i;
        if(fHandle == NULL || fopen(fHandle->fileName,"r") == NULL)
                return RC_FILE_NOT_FOUND;
	if(fHandle->curPagePos >= fHandle->totalNumPages)
		return RC_READ_NON_EXISTING_PAGE;
	fseek(fHandle->mgmtInfo , 0 , SEEK_CUR);
        if(fread(memPage, 1, PAGE_SIZE, fHandle->mgmtInfo) != PAGE_SIZE)
        	return RC_FILE_READ_ERROR;
	fHandle->curPagePos = (ftell(fHandle->mgmtInfo)/PAGE_SIZE) + 1;
        return RC_OK;

}

RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
 	int i;
        if(fHandle == NULL || fopen(fHandle->fileName,"r") == NULL)
                return RC_FILE_NOT_FOUND;
	if(fHandle->curPagePos >= fHandle->totalNumPages)
		return RC_READ_NON_EXISTING_PAGE;
        fseek(fHandle->mgmtInfo , PAGE_SIZE , SEEK_CUR);
        if(fread(memPage, 1, PAGE_SIZE, fHandle->mgmtInfo) != PAGE_SIZE)
        	return RC_FILE_READ_ERROR;
	fHandle->curPagePos = (ftell(fHandle->mgmtInfo)/PAGE_SIZE) + 1;
        return RC_OK;
}

RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	int i;
        if(fHandle == NULL || fopen(fHandle->fileName,"r") == NULL)
                return RC_FILE_NOT_FOUND;
        fseek(fHandle->mgmtInfo , -PAGE_SIZE , SEEK_END);
        if(fread(memPage, 1, PAGE_SIZE, fHandle->mgmtInfo) != PAGE_SIZE)
        	return RC_FILE_READ_ERROR;
	fHandle->curPagePos = (ftell(fHandle->mgmtInfo)/PAGE_SIZE) + 1;
        return RC_OK;
}

/* writing blocks to a page file */
RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	int i;
	char *ops="11111111";
	if(fHandle == NULL || fopen(fHandle->fileName,"a+") == NULL)
		return RC_FILE_NOT_FOUND;

	if(pageNum > ((fHandle->totalNumPages)+1))
		return RC_WRITE_OUT_OF_BOUND_INDEX;
	/*
	if(pageNum <= fHandle->totalNumPages)
	{
		printf("Page Already Present!!\nDo you want to over-write the file (y/n): ");
		scanf("%c",&ops);
	}
	if(ops == 'y')
	{*/
		int a = fseek(fHandle->mgmtInfo , (pageNum-1)*PAGE_SIZE , SEEK_SET);
		printf("%d",a);
		printf("%ld",ftell(fHandle->mgmtInfo));
		a = fwrite(memPage,PAGE_SIZE,1,fHandle->mgmtInfo);
		printf("%d",a);
	//}

	fHandle->curPagePos = (ftell(fHandle->mgmtInfo)/PAGE_SIZE) + 1;
	fHandle->totalNumPages += 1;
	return RC_OK;
}
        
RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
int i;
char *ch="6";
    if(fHandle == NULL && fopen(fHandle->fileName,"r+") == NULL)
                return RC_WRITE_FAILED;
    if(fseek(fHandle->mgmtInfo , 0 , SEEK_CUR)!=0)
                return RC_WRITE_FAILED;
    else
    {
        for(i=0;i<PAGE_SIZE;i++)
        {
            memPage=memPage+fwrite(ch,1,PAGE_SIZE,fHandle->mgmtInfo);
        }
    }
 
 
    fHandle->curPagePos = ftell(fHandle->mgmtInfo)/PAGE_SIZE;
    return RC_OK;
}

RC appendEmptyBlock (SM_FileHandle *fHandle)
{
	char buffer[PAGE_SIZE];	
	FILE *fp = NULL;
	int i;
	
	if (fHandle->mgmtInfo == NULL) 
	{
		return RC_FILE_NOT_FOUND;
	} 
	else
	{
		fp = fopen(fHandle->fileName,"r+");
		/*int totalPages = fHandle->totalNumPages;
        	if (totalPages == 0) {
            		return RC_NO_PAGES_ERROR;
		}
		else
		{*/
			fseek(fp, 0, SEEK_END);
			for(i = 0; i < PAGE_SIZE; i++)
			{
				fwrite("\0",1, 1,fp);
				fseek(fp,0,SEEK_END);
			}
			
		//}
	}
	return RC_OK;
}
RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle)
{
	
	if (fHandle == NULL)   // If file handle is null, throw error
		return RC_FILE_HANDLE_NOT_INIT;
	
	if (fHandle->totalNumPages < numberOfPages) //Check if total number of pages is less than number of pages
	{
		while (fHandle->totalNumPages != numberOfPages) 
		{
			appendEmptyBlock(fHandle); // Append empty block
		}
	}
	return RC_OK;
}

