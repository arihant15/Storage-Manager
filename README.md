			              #Storage Manager

Project Modules
--------------------------------------------------------------------------------------
C source files: storage_mgr.c, dberror.c, test_assign1_1.c,test_assign1_2.c
Header files: storage_mgr.h, dberror.h, test_helper.h

Team members
---------------------------------------
1)Anuja Velankar
2)Arihant Nagarajan
3)Sher Syed
4)Sonaki Bengali


Aim:
----------------------------------------------------------------------------------------
The goal of this assignment is to implement a storage manager - a module that 
is capable of reading blocks from a file on disk into memory and writing blocks from 
memory to a file on disk. The storage manager deals with pages (blocks) of fixed 
size (PAGE_SIZE). We have written functions to create, open and close files and read,
write,append pages in the file. 
Read  functions can also read current block ,next block, previous block etc.

Instructions to run the code
----------------------------
1) Go to the path where the extracted files are present.
2) Run the below command:
   make
3) Run the below command for testing test_assign1.c:
   make run
4) To remove object files, run following command:
   make clean
5) We have provided additional testcases in file "test_assign1_2.c 
   To run those testcases use the below command:
   ./test_assign1_2
  
Functions:
------------
Function used to implement storage manager are described below:

initStorageManager()
------------------------------
Initializes the program.

createPageFile()
--------------------------
This function checks if the file already exists by trying to open the file in 'read' mode. If it is present ,then user is being asked, if file can be overwritten. If user permits then the file is opened in 'write' mode. If file doesnt exist,  then new file is created and 1 page is added to it.If user doesnt allow to overwrite, return the code 'RC_FILE_PRESENT'.

openPageFile()
--------------------
Opens the file in 'read' mode, if the file exists.Returns error code is file does not exist.Initializes the file handler variables like file file name ,number of pages and current page position.

closePageFile()
---------------
Closes the file. If the file doesn't exist or unable to open in 'read' mode ,then returns the error code accordingly.

destroyPageFile()
-------------------
Removes the existing file and returns success message. If file does not exist returns the error code.

readBlock()
----------------

Moves the  file descriptor to a particular block from beginning of the file and reads data of 1 page size (4096 bytes)and loads it to the memory specified by mempage.Finds out the current page position using 'ftell' and stores it in 'curPagePos' for further operations.
Returns error code if: i)File descriptor is not initialised, ii)file does not exist and is unable to open in 'read' mode ,iii)The page accessed is out of bound for the file , iv)Unable to read the given page.

getBlockPos()
--------------------
Finds out current page position using file descriptor. Returns error code if file handler is not intialised or file  failed to open in 'read' mode. 

readFirstBlock ()
----------------------
Moves the file descriptor in file handler to the beginning of the file and reads 1st page contents into 'mempage'. Returns error code if: i) i)File descriptor is not initialised, ii)file does not exist or is unable to open in 'read' mode  iii)Unable to read the given page.

readPreviousBlock ()
------------------------

Moves the file descriptor in filehandler to the previous page with respect to current page postion and  reads the content of the previous page to buffer specified by mempage.
Returns error code if: i) i)File handler is not initialised, ii)file does not exist or is unable to open in 'read' mode iii) Current page position in file handler points to non-existing page  iv)Unable to read the given page.

readCurrentBlock ()
----------------------
Sets the  file descriptor in file handler to current page position and reads the page content to 'mempage'.
Returns error code if: i) i)File handler is not initialised, ii)file does not exist or is unable to open in 'read' mode iii) Current page position in file handler points to non-existing page  iv)Unable to read the given page.

readNextBlock ()
--------------------

Sets the  file descriptor in file handler to next page position and reads the page content to 'mempage'.
Returns error code if: i) File handler is not initialised, ii)file does not exist or is unable to open in 'read' mode iii) Current page position in file handler points to non-existing page  iv)Unable to read the given page.

readLastBlock ()
-------------------

Sets the  file descriptor in file handler to last page  and reads the page content to buffer specified by 'mempage'.
Returns error code if: i) i)File handler is not initialised, ii)file does not exist or is unable to open in 'read' mode iii) Current page position in file handler points to non-existing page  iv)Unable to read the entire given page.

writeBlock()
-------------
Checks if file is present and moves the file pointer to the specific page from the beginning of the file and writes 1 page to it.
File handle variable are updated. 

Returns error code if: i) i)File handler is not initialised, ii)file does not exist or is unable to open in 'read' mode iii) Current page position in file handler points to non-existing page  iv)Unable to write the entire given page.

writeCurrentBlock()
------------------------
Checks if file is present and moves the file pointer to current page from the beginning of the file and writes 1 page to it.
File handle variable are updated. 

Returns error code if: i)File does not exist or is unable to open in 'read' mode iii) Current page position in file handler points to non-existing page  iv)Unable to write the entire given page.

appendEmptyBlock()
----------------------

Checks if file is present and can be opened in 'read' mode .
If these conditions are satisfied, then the existing file is opened in 'read+' mode to write the data. File pointer is set to the end of the file and one page is added with character '\0' in newly added empty block.
File handle variable are updated. 

Returns error code if: i)File does not exist or is unable to open in 'read' mode 

ensureCapacity()
--------------------
Checks if the file has capacity to to accomodate numberofpages. If the file does not sufficient pages, using 'appendEmptyBlock' function, add more pages.
Returns error code if: i)File handle is null ii)File does not exist or is unable to open in 'read' mode iii) Current page position in file handler points to non-existing page  

Test Cases:
-------------
Few more test cases are defined in 'test_assign1_2.c'.
Following fuctions are used:

testCreateOpenClose(void)
--------------------------
Creates, opens, close and destroys page file. If file is opened after being destroyed , must return error.

testSinglePageContent(void)
----------------------------
Creates, opens and reads the newly initialized page. Block will be empty. Then the block is written and read again.

testMultiPageContent(void)
----------------------------
Checks the contents of multiple pages by using the phases same as 'testSinglePageContent'.
