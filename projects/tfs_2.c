//Joel Miller
//CPSC 3320
//07/30/2023

#include "tfs.h"

/* tfs_delete()
 *
 * deletes a closed directory entry having the given file descriptor
 *   (changes the status of the entry to unused) and releases all
 *   allocated file blocks
 *
 * preconditions:
 *   (1) the file descriptor is in range
 *   (2) the directory entry is closed
 *
 * postconditions:
 *   (1) the status of the directory entry is set to unused
 *   (2) all file blocks have been set to free
 *
 * input parameter is a file descriptor
 *
 * return value is TRUE when successful or FALSE when failure
 */

unsigned int tfs_delete(unsigned int file_descriptor) {
	
	// Check if the directory entry is already unused
	if (directory[file_descriptor].status == UNUSED) 
  {
		return FALSE; // Failure: Directory entry is already unused.
	}

	// Set the status of the directory entry to unused.
	directory[file_descriptor].status = UNUSED;

	// Start from the first block of the file in the file allocation table.
	unsigned int table = directory[file_descriptor].first_block;

	unsigned int nxt;

	// Release all allocated file blocks by traversing the file allocation table.
	while (file_allocation_table[table] != FREE) 
  {
		if (file_allocation_table[table] != FREE)
    {
			nxt = file_allocation_table[table];


			file_allocation_table[table] = FREE; // Mark the current block as free.

			table = nxt;
		}
	}

	return TRUE; // Successful deletion.
}


/* tfs_read()
 *
 * reads a specified number of bytes from a file starting
 *   at the byte offset in the directory into the specified
 *   buffer; the byte offset in the directory entry is
 *   incremented by the number of bytes transferred
 *
 * depending on the starting byte offset and the specified
 *   number of bytes to transfer, the transfer may cross two
 *   or more file blocks
 *
 * the function will read fewer bytes than specified if the
 *   end of the file is encountered before the specified number
 *   of bytes have been transferred
 *
 * preconditions:
 *   (1) the file descriptor is in range
 *   (2) the directory entry is open
 *   (3) the file has allocated file blocks
 *
 * postconditions:
 *   (1) the buffer contains bytes transferred from file blocks
 *   (2) the specified number of bytes has been transferred
 *         except in the case that end of file was encountered
 *         before the transfer was complete
 *
 * input parameters are a file descriptor, the address of a
 *   buffer of bytes to transfer, and the count of bytes to
 *   transfer
 *
 * return value is the number of bytes transferred
 */

unsigned int tfs_read(unsigned int file_descriptor, char *buffer, unsigned int byte_count) {
	
	// Check if the file is readable
	if (!(isReadable(file_descriptor))) {
		return -1; // Failure: File is not readable.
	}

	unsigned int bytes_transferred = 0;


	    unsigned short byte_offset = directory[file_descriptor].byte_offset;

	unsigned char block = directory[file_descriptor].first_block;

	// Read data from file blocks into the buffer.
	while (bytes_transferred < byte_count) {
		for (int i = 0; i < BLOCK_SIZE && bytes_transferred < byte_count; i++) {
			// Read the current byte from the current block.
			unsigned char curr = blocks[block].bytes[byte_offset++];
			buffer[bytes_transferred++] = curr;

			// Check if we reached the end of the file.
			if (byte_offset == directory[file_descriptor].size) {
				return bytes_transferred; // Reached the end of the file.
			}
		}

		// Move to the next block in the file allocation table.
		block = file_allocation_table[block];
	}

	return bytes_transferred; // Return the number of bytes transferred.
}
/* tfs_write()
 *
 * writes a specified number of bytes from a specified buffer
 *   into a file starting at the byte offset in the directory;
 *   the byte offset in the directory entry is incremented by
 *   the number of bytes transferred
 *
 * depending on the starting byte offset and the specified
 *   number of bytes to transfer, the transfer may cross two
 *   or more file blocks
 *
 * furthermore, depending on the starting byte offset and the
 *   specified number of bytes to transfer, additional file
 *   blocks, if available, will be added to the file as needed;
 *   in this case, the size of the file will be adjusted
 *   based on the number of bytes transferred beyond the
 *   original size of the file
 *
 * the function will read fewer bytes than specified if file
 *   blocks are not available
 *
 * preconditions:
 *   (1) the file descriptor is in range
 *   (2) the directory entry is open
 *
 * postconditions:
 *   (1) the file contains bytes transferred from the buffer
 *   (2) the specified number of bytes has been transferred
 *         except in the case that file blocks needed to
 *         complete the transfer were not available
 *   (3) the size of the file is increased as appropriate
 *         when transferred bytes extend beyond the previous
 *         end of the file
 *
 * input parameters are a file descriptor, the address of a
 *   buffer of bytes to transfer, and the count of bytes to
 *   transfer
 *
 * return value is the number of bytes transferred
 */

unsigned int tfs_write(unsigned int file_descriptor, char *buffer, unsigned int byte_count) {
	
	// Find the last block in the file's file allocation table.
	unsigned int table = directory[file_descriptor].first_block;


	while (table != FREE && file_allocation_table[table] != LAST_BLOCK) {
		table = file_allocation_table[table];
	}

	// If no blocks are in use, allocate a new block for the file.
	if (table == FREE) {
		table = tfs_new_block();


		directory[file_descriptor].first_block = table;


		file_allocation_table[table] = LAST_BLOCK;
		makeWritable(file_descriptor);
	}

	// Check if the file is writable.
	if (!(isWritable(file_descriptor))) {
		return -1; // Failure: File is not writable.
	}

	// Calculate the remaining space in the current block.
	unsigned int firstTable = tfs_size(file_descriptor) % BLOCK_SIZE;


	unsigned int free = BLOCK_SIZE - firstTable;


	unsigned int totalBytesTransferred = 0;

	while (totalBytesTransferred < byte_count) {
		// Write data from the buffer into the file blocks.
		while (free > 0 && totalBytesTransferred < byte_count)
     {
			blocks[table].bytes[firstTable] = buffer[totalBytesTransferred];

			totalBytesTransferred++;

			firstTable++;

			free--;
		}

		// If more bytes are to be written, allocate a new block for the file.
		if (totalBytesTransferred < byte_count)
     {
			file_allocation_table[table] = tfs_new_block();

			table = file_allocation_table[table];

			file_allocation_table[table] = LAST_BLOCK;

			free = BLOCK_SIZE;
			firstTable = 0;
		}
	}

	// Update file size and byte offset in the directory entry.
	directory[file_descriptor].size += byte_count;

	directory[file_descriptor].byte_offset = totalBytesTransferred % BLOCK_SIZE;

	return totalBytesTransferred;
}


/* isReadable()
 * Checks if the file represented by the given file descriptor has read permission (READONLY).
 *
 * input parameter:
 *   - file_descriptor: The file descriptor of the file to check.
 *
 * return value:
 *   - Returns TRUE if the file has read permission (READONLY), otherwise returns FALSE.
 */
bool isReadable(unsigned int file_descriptor) {
	return (directory[file_descriptor].permissions & READONLY) != 0; // Bitwise AND operation to check read permission.
}

/* isWritable()
 * Checks if the file represented by the given file descriptor has write permission (WRITEONLY).
 *
 * input parameter:
 *   - file_descriptor: The file descriptor of the file to check.
 *
 * return value:
 *   - Returns TRUE if the file has write permission (WRITEONLY), otherwise returns FALSE.
 */
bool isWritable(unsigned int file_descriptor) {
	return (directory[file_descriptor].permissions & WRITEONLY) != 0; // Bitwise AND operation to check write permission.
}

/* makeReadable()
 * Sets the read permission (READONLY) for the file represented by the given file descriptor.
 *
 * input parameter:
 *   - file_descriptor: The file descriptor of the file to set read permission.
 */
void makeReadable(unsigned int file_descriptor) {
	directory[file_descriptor].permissions |= READONLY; // Bitwise OR operation to set read permission.
}

/* makeWritable()
 * Sets the write permission (WRITEONLY) for the file represented by the given file descriptor.
 *
 * input parameter:
 *   - file_descriptor: The file descriptor of the file to set write permission.
 */
void makeWritable(unsigned int file_descriptor) {
	directory[file_descriptor].permissions |= WRITEONLY; // Bitwise OR operation to set write permission.
}

/* makeUnread()
 * Clears the read permission (READONLY) for the file represented by the given file descriptor.
 *
 * input parameter:
 *   - file_descriptor: The file descriptor of the file to clear read permission.
 */
void makeUnread(unsigned int file_descriptor) {
	directory[file_descriptor].permissions &= ~READONLY; // Bitwise AND with complement to clear read permission.
}

/* makeUnwrite()
 * Clears the write permission (WRITEONLY) for the file represented by the given file descriptor.
 *
 * input parameter:
 *   - file_descriptor: The file descriptor of the file to clear write permission.
 */
void makeUnwrite(unsigned int file_descriptor) {
	directory[file_descriptor].permissions &= ~WRITEONLY; // Bitwise AND with complement to clear write permission.
}


/* allRead()
 * Enables read permission for all files in the directory.
 * This function iterates through each directory entry and sets the read permission (READONLY) for each file that has allocated blocks.
 * Files with no allocated blocks will not have read permission set.
 */
void allRead() {
	// Declare a variable for iteration.
	unsigned int firstDirect;

	// Loop through each directory entry to enable read permission for files with allocated blocks.
	for (firstDirect = FIRST_VALID_FD; firstDirect < N_DIRECTORY_ENTRIES; firstDirect++) {
		// Check if the file has allocated blocks (is not FREE).
		if (directory[firstDirect].first_block != FREE) {
			makeReadable(firstDirect); // Set read permission (READONLY) for the file.
		}
	}
}

/* allWrite()
 * Enables write permission for all files in the directory.
 * This function iterates through each directory entry and sets the write permission (WRITEONLY) for each file that has allocated blocks.
 * Files with no allocated blocks will not have write permission set.
 */
void allWrite() {
	// Declare a variable for iteration.
	unsigned int firstDirect;

	// Loop through each directory entry to enable write permission for files with allocated blocks.
	for (firstDirect = FIRST_VALID_FD; firstDirect < N_DIRECTORY_ENTRIES; firstDirect++) {
		// Check if the file has allocated blocks (is not FREE).
		if (directory[firstDirect].first_block != FREE) {
			makeWritable(firstDirect); // Set write permission (WRITEONLY) for the file.
		}
	}
}


/* listDirectory()
 * Lists the entries in the directory along with their status, size, and file allocation table (FAT).
 * This function prints the information to the console.
 * The function also displays the read and write permissions of each file.
 */
void listDirectory() {
	// Declare variables for iteration.
	unsigned int firstDirect;
	unsigned char bits;

	// Print the header for the directory listing.
	printf("-- directory listing --\n");

	// Loop through each directory entry and print its information.
	for (firstDirect = FIRST_VALID_FD; firstDirect < N_DIRECTORY_ENTRIES; firstDirect++) {
		// Print the file descriptor (fd) and status of the directory entry.
		printf("  fd = %2d: ", firstDirect);
		
		// Check the status of the directory entry.
		if (directory[firstDirect].status == UNUSED) {
			printf("unused\n");
		}
		else if (directory[firstDirect].status == CLOSED) {
			// Print information for a closed file.
			printf("%s, currently closed, %d bytes in size\n", directory[firstDirect].name, directory[firstDirect].size);
		}
		else if (directory[firstDirect].status == OPEN) {
			// Print information for an open file.
			printf("%s, currently open, %d bytes in size\n", directory[firstDirect].name, directory[firstDirect].size);
		}
		else {
			printf("*** status error\n"); // Print an error message for an invalid status.
		}

		// If the directory entry is closed or open, display the FAT information.
		if ((directory[firstDirect].status == CLOSED) || (directory[firstDirect].status == OPEN)) {
			printf("           FAT:");
			// Check if the first block is used.
			if (directory[firstDirect].first_block == 0) {
				printf(" no blocks in use\n"); // Print if no blocks are used in the file.
			}
			else {
				bits = directory[firstDirect].first_block;
				// Traverse the file allocation table (FAT) and print used block numbers.
				while (bits != LAST_BLOCK) {
					printf(" %d", bits);
					bits = file_allocation_table[bits];
				}
				printf("\n");

				// PART 2: Display the read and write permissions of the file.
				printf("           permissions | read: ");
				printf("%s", isReadable(firstDirect) ? "true" : "false");
				printf(" | write: ");
				printf("%s", isWritable(firstDirect) ? "true |\n" : "false |\n");
			}
		}
	}

	// Print the end of the directory listing.
	printf("-- end --\n");
}
