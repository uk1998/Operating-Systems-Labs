#include "simplefs-ops.h"
extern struct filehandle_t file_handle_array[MAX_OPEN_FILES]; // Array for storing opened files

int simplefs_create(char *filename){
    /*
	    Create file with name `filename` from disk
	*/
	struct inode_t *inode = malloc(sizeof(struct inode_t));
	int i;
	for(i=0; i<NUM_INODES; i++){
		simplefs_readInode(i, inode);
		if(!strcmp(inode->name, filename)) // strcmp returns 0 if strings are equal
			return -1;
	}
	int inode_number = simplefs_allocInode();
	if(inode_number == -1)
		return -1;
    
    memcpy(inode->name, filename, sizeof(filename));

    inode->status = INODE_IN_USE;
    inode->file_size = 0;
	
    for(i=0; i<MAX_FILE_SIZE; i++)
    	inode->direct_blocks[i] = -1;
	
    simplefs_writeInode(inode_number, inode);
    free(inode);
    return inode_number;
}


void simplefs_delete(char *filename){
    /*
	    delete file with name `filename` from disk
	*/
	int inode_num = -1;
	struct inode_t *inode = malloc(sizeof(struct inode_t));
	int i;

	// Check if the file with name `filename` exists
	for(i=0; i<NUM_INODES; i++){
		simplefs_readInode(i, inode);
		if(!strcmp(inode->name, filename)){
			inode_num = i;
			break;
		}
	}

	if(inode_num == -1)
		return -1;			// File does not exist

	for(i=0; i<MAX_FILE_SIZE; i++){
		if(inode->direct_blocks[i] != -1)
			simplefs_freeDataBlock(inode->direct_blocks[i]);
	}

	simplefs_freeInode(inode_num);
	free(inode);
}

int simplefs_open(char *filename){
    /*
	    open file with name `filename`
	*/
	int inode_num = -1;
    struct inode_t *inode = malloc(sizeof(struct inode_t));
	int i;
	
	// Check if the file with name `filename` exists
	for(i=0; i<NUM_INODES; i++){
		simplefs_readInode(i, inode);
		if(!strcmp(inode->name, filename)){
			inode_num = i;
			break;
		}
	}

	if(inode_num == -1)
		return -1;			// File does not exist

	int fd = -1;
	for(i=0; i<MAX_OPEN_FILES; i++){
        if(file_handle_array[i].inode_number == -1){	
        	file_handle_array[i].offset = inode->file_size;
        	file_handle_array[i].inode_number = inode_num;
        	fd = i;
        	break;
        }
    }
    free(inode);

    if(fd == -1)
    	return -1;
}

void simplefs_close(int file_handle){
    /*
	    close file pointed by `file_handle`
	*/
	file_handle_array[file_handle].offset = 0;
	file_handle_array[file_handle].inode_number = -1;

}

int simplefs_read(int file_handle, char *buf, int nbytes){
    /*
	    read `nbytes` of data into `buf` from file pointed by `file_handle` starting at current offset

		Does not support misaligned access
	*/
	if (nbytes > MAX_FILE_SIZE * BLOCKSIZE){
		return -1;
	}
	if (file_handle >= MAX_OPEN_FILES){
		return -1;
	}
	struct inode_t *inode = malloc(sizeof(struct inode_t));

	simplefs_readInode(file_handle_array[file_handle]->inode_number, inode);
	for (size_t i = 0; i < NUM_INODE_BLOCKS; i++){
		if (inode->direct_blocks[i] != -1){
			simplefs_readDataBlock(inode->direct_blocks[i], buf[i*BLOCKSIZE]);
		}		
	}
	
    return 0;
}


int simplefs_write(int file_handle, char *buf, int nbytes){
    /*
	    write `nbytes` of data from `buf` to file pointed by `file_handle` starting at current offset

		Does not support misaligned access
	*/
	if (nbytes > MAX_FILE_SIZE * BLOCKSIZE){
		return -1;
	}
	if (file_handle >= MAX_OPEN_FILES){
		return -1;
	}
	struct inode_t *inode = malloc(sizeof(struct inode_t));

	simplefs_readInode(file_handle_array[file_handle]->inode_number, inode);
	for (size_t i = 0; i < NUM_INODE_BLOCKS; i++){
		if (inode->direct_blocks[i] == -1){
			simplefs_writeDataBlock(inode->direct_blocks[i], buf[i*BLOCKSIZE]);
		}		
	}
	
    return 0;
}


int simplefs_seek(int file_handle, int nseek){
    /*
	   increase `file_handle` offset by `nseek`
	*/
    if(file_handle_array[file_handle].offset + nseek < 0 || file_handle_array[file_handle].offset + nseek >= MAX_FILE_SIZE*BLOCKSIZE)
		return -1;

	file_handle_array[file_handle].offset += nseek;	  
	return 0;
}