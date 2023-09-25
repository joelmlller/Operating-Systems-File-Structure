//Joel Miller
//CPSC 3320
//07/30/2023

#include "tfs.h"

int main(){
  unsigned int fd[20];
  char buffer1[1024], buffer2[1024], buffer3[1024];
  unsigned int length1, length2, count1, count2, count3;

  sprintf( buffer1, "%s",
    "This is a simple-minded test for the trivial file system code.  " );
  sprintf( buffer1 + 64, "%s",
    "This is a simple-minded test for the trivial file system code.  " );
  sprintf( buffer1 + 128, "%s",
    "This is a simple-minded test for the trivial file system code.  " );
  sprintf( buffer1 + 192, "%s",
    "This is a simple-minded test for the trivial file system code.  " );
  sprintf( buffer1 + 256, "%s",
    "This is a simple-minded test for the trivial file system code.  " );

  sprintf( buffer2, "%s",
    "And now for something completely different." );

  length1 = strlen( buffer1 );
  length2 = strlen( buffer2 );
  printf( "length of buffer1 is %d\n", length1 );
  printf( "length of buffer2 is %d\n", length2 );

  tfs_init();

  tfs_list_directory();

  fd[0] = tfs_create( "file.txt" );
  if( fd[0] == 0 ) printf( "first create failed\n" );

  fd[1] = tfs_create( "my_file" );
  if( fd[1] == 0 ) printf( "second create failed\n" );

  tfs_list_directory();

  count1 = tfs_write( fd[0], buffer1, length1 );
  printf( "%d bytes written to first file\n", count1 );

  count2 = tfs_write( fd[1], buffer2, length2 );
  printf( "%d bytes written to second file\n", count2 );

  count1 = tfs_write( fd[0], buffer1, length1 );
  printf( "%d bytes written to first file\n", count1 );

  tfs_close( fd[1] );

  tfs_list_directory();
  tfs_list_blocks();
  makeReadable(fd[0]); 
  tfs_seek( fd[0], 600 );
  count3 = tfs_read( fd[0], buffer3, 640 );
  printf( "%d bytes read from first file\n", count3 );
  buffer3[count3] = '\0';
  printf( "[%s]\n", buffer3 );

  tfs_seek( fd[0], 250 );
  count3 = tfs_read( fd[0], buffer3, 20 );
  printf( "%d bytes read from first file\n", count3 );
  buffer3[count3] = '\0';
  printf( "[%s]\n", buffer3 );

  fd[2] = tfs_create( "file.txt" );
  printf( "fd for creating a file with identical name" );
  printf( " as existing file - %d\n", fd[2] );
  fd[2] = tfs_create( "file3" );
  fd[3] = tfs_create( "file4" );
  fd[4] = tfs_create( "file5" );
  fd[5] = tfs_create( "file6" );
  fd[6] = tfs_create( "file7" );
  fd[7] = tfs_create( "file8" );
  fd[8] = tfs_create( "file9" );
  fd[9] = tfs_create( "file10" );
  fd[10] = tfs_create( "file11" );
  fd[11] = tfs_create( "file12" );
  fd[12] = tfs_create( "file13" );
  fd[13] = tfs_create( "file14" );
  fd[14] = tfs_create( "file15" );
  fd[15] = tfs_create( "file16" );
  printf( "fd for creating a sixteenth file - %d\n", fd[15] );

// Print a header for the test.
printf("\n\n|||||||||||Joel Miller READ and WRITE TEST|||||||||||\n");
printf("I will add text to files and will revoke their write permissions right after writing!\n");

// Sample text to test writing to files.
char *testText = "Text to test to write to the file";

// Loop through file descriptors (from 2 to 14) in increments of 0, 1, or 2 (randomly).
for (int i = 2; i <= 14; i += rand() % 3) {
	// Get the length of the testText.
	int textLen = strlen(testText);

	// Enable write permission for the file.
	makeWritable(fd[i]);

	// Write the testText to the file.
	int varCount = tfs_write(fd[i], testText, textLen);
	printf("%d bytes written to file %d\n", varCount, i+1);

	// If the index is even, revoke the write permission for the file.
	if (i % 2 == 0) {
		makeUnwrite(fd[i]);
		printf("File %d has lost permissions to write!!\n", i+1);
	}
}

// Print a message to check permissions after adding text.
printf("\nThe permissions will now be checked after adding text!\n");

// List the directory to display file information.
listDirectory();

// Enable read permission for all files in the directory.
printf("Making all readable!\n");
allRead();

// Print a message to check read permissions after enabling them.
printf("Checking read permissions!\n");
listDirectory();

// Enable write permission for all files in the directory.
printf("Making all writable!\n");
allWrite();

// List the directory to display file information after enabling write permissions.
listDirectory();


  

  return 0;
}

/* this test driver should print

length of buffer1 is 320
length of buffer2 is 43
-- directory listing --
  fd =  1: unused
  fd =  2: unused
  fd =  3: unused
  fd =  4: unused
  fd =  5: unused
  fd =  6: unused
  fd =  7: unused
  fd =  8: unused
  fd =  9: unused
  fd = 10: unused
  fd = 11: unused
  fd = 12: unused
  fd = 13: unused
  fd = 14: unused
  fd = 15: unused
-- end --
-- directory listing --
  fd =  1: file.txt, currently open, 0 bytes in size
           FAT: no blocks in use
  fd =  2: my_file, currently open, 0 bytes in size
           FAT: no blocks in use
  fd =  3: unused
  fd =  4: unused
  fd =  5: unused
  fd =  6: unused
  fd =  7: unused
  fd =  8: unused
  fd =  9: unused
  fd = 10: unused
  fd = 11: unused
  fd = 12: unused
  fd = 13: unused
  fd = 14: unused
  fd = 15: unused
-- end --
320 bytes written to first file
43 bytes written to second file
320 bytes written to first file
-- directory listing --
  fd =  1: file.txt, currently open, 640 bytes in size
           FAT: 4 5 6 8 9
  fd =  2: my_file, currently closed, 43 bytes in size
           FAT: 7
  fd =  3: unused
  fd =  4: unused
  fd =  5: unused
  fd =  6: unused
  fd =  7: unused
  fd =  8: unused
  fd =  9: unused
  fd = 10: unused
  fd = 11: unused
  fd = 12: unused
  fd = 13: unused
  fd = 14: unused
  fd = 15: unused
-- end --
-- file alllocation table listing of used blocks --
  block   4 is used and points to   5
  block   5 is used and points to   6
  block   6 is used and points to   8
  block   7 is used and points to   1
  block   8 is used and points to   9
  block   9 is used and points to   1
-- end --
40 bytes read from first file
[test for the trivial file system code.  ]
20 bytes read from first file
[ode.  This is a simp]
fd for creating a file with identical name as existing file - 0
fd for creating a sixteenth file - 0
-- directory listing --
  fd =  1: file.txt, currently open, 640 bytes in size
           FAT: 4 5 6 8 9
  fd =  2: my_file, currently closed, 43 bytes in size
           FAT: 7
  fd =  3: file3, currently open, 0 bytes in size
           FAT: no blocks in use
  fd =  4: file4, currently open, 0 bytes in size
           FAT: no blocks in use
  fd =  5: file5, currently open, 0 bytes in size
           FAT: no blocks in use
  fd =  6: file6, currently open, 0 bytes in size
           FAT: no blocks in use
  fd =  7: file7, currently open, 0 bytes in size
           FAT: no blocks in use
  fd =  8: file8, currently open, 0 bytes in size
           FAT: no blocks in use
  fd =  9: file9, currently open, 0 bytes in size
           FAT: no blocks in use
  fd = 10: file10, currently open, 0 bytes in size
           FAT: no blocks in use
  fd = 11: file11, currently open, 0 bytes in size
           FAT: no blocks in use
  fd = 12: file12, currently open, 0 bytes in size
           FAT: no blocks in use
  fd = 13: file13, currently open, 0 bytes in size
           FAT: no blocks in use
  fd = 14: file14, currently open, 0 bytes in size
           FAT: no blocks in use
  fd = 15: file15, currently open, 0 bytes in size
           FAT: no blocks in use
-- end --
-- directory listing --
  fd =  1: unused
  fd =  2: my_file, currently closed, 43 bytes in size
           FAT: 7
  fd =  3: file3, currently open, 0 bytes in size
           FAT: no blocks in use
  fd =  4: file4, currently open, 0 bytes in size
           FAT: no blocks in use
  fd =  5: file5, currently open, 0 bytes in size
           FAT: no blocks in use
  fd =  6: file6, currently open, 0 bytes in size
           FAT: no blocks in use
  fd =  7: file7, currently open, 0 bytes in size
           FAT: no blocks in use
  fd =  8: file8, currently open, 0 bytes in size
           FAT: no blocks in use
  fd =  9: file9, currently open, 0 bytes in size
           FAT: no blocks in use
  fd = 10: file10, currently open, 0 bytes in size
           FAT: no blocks in use
  fd = 11: file11, currently open, 0 bytes in size
           FAT: no blocks in use
  fd = 12: file12, currently open, 0 bytes in size
           FAT: no blocks in use
  fd = 13: file13, currently open, 0 bytes in size
           FAT: no blocks in use
  fd = 14: file14, currently open, 0 bytes in size
           FAT: no blocks in use
  fd = 15: file15, currently open, 0 bytes in size
           FAT: no blocks in use
-- end --
-- directory listing --
  fd =  1: unused
  fd =  2: my_file, currently closed, 43 bytes in size
           FAT: 7
  fd =  3: file3, currently open, 0 bytes in size
           FAT: no blocks in use
  fd =  4: file4, currently closed, 0 bytes in size
           FAT: no blocks in use
  fd =  5: file5, currently closed, 0 bytes in size
           FAT: no blocks in use
  fd =  6: file6, currently closed, 0 bytes in size
           FAT: no blocks in use
  fd =  7: unused
  fd =  8: unused
  fd =  9: file9, currently open, 0 bytes in size
           FAT: no blocks in use
  fd = 10: file10, currently open, 0 bytes in size
           FAT: no blocks in use
  fd = 11: file11, currently open, 0 bytes in size
           FAT: no blocks in use
  fd = 12: file12, currently open, 0 bytes in size
           FAT: no blocks in use
  fd = 13: file13, currently open, 0 bytes in size
           FAT: no blocks in use
  fd = 14: file14, currently open, 0 bytes in size
           FAT: no blocks in use
  fd = 15: file15, currently open, 0 bytes in size
           FAT: no blocks in use
-- end --
-- directory listing --
  fd =  1: added_1, currently open, 0 bytes in size
           FAT: no blocks in use
  fd =  2: my_file, currently closed, 43 bytes in size
           FAT: 7
  fd =  3: file3, currently open, 0 bytes in size
           FAT: no blocks in use
  fd =  4: file4, currently closed, 0 bytes in size
           FAT: no blocks in use
  fd =  5: file5, currently closed, 0 bytes in size
           FAT: no blocks in use
  fd =  6: file6, currently closed, 0 bytes in size
           FAT: no blocks in use
  fd =  7: added_2, currently open, 0 bytes in size
           FAT: no blocks in use
  fd =  8: unused
  fd =  9: file9, currently open, 0 bytes in size
           FAT: no blocks in use
  fd = 10: file10, currently open, 0 bytes in size
           FAT: no blocks in use
  fd = 11: file11, currently open, 0 bytes in size
           FAT: no blocks in use
  fd = 12: file12, currently open, 0 bytes in size
           FAT: no blocks in use
  fd = 13: file13, currently open, 0 bytes in size
           FAT: no blocks in use
  fd = 14: file14, currently open, 0 bytes in size
           FAT: no blocks in use
  fd = 15: file15, currently open, 0 bytes in size
           FAT: no blocks in use
-- end --
-- file alllocation table listing of used blocks --
  block   7 is used and points to   1
-- end --

*/
