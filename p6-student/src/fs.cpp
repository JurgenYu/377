#include "fs.h"

using namespace std;

myFileSystem::myFileSystem(char diskName[16]) {
  // open the file with the above name
  // this file will act as the "disk" for your file system
  disk.open(diskName, ios::in | ios::out);
  if (!disk.is_open()) {
    perror("disk dosen't exist!");
    exit(1);
  }
  printf("%s created\n", diskName);
}

int myFileSystem::create_file(char name[8], int size) {
  // create a file with this name and this size

  // high level pseudo code for creating a new file
  // Step 1: Check to see if we have sufficient free space on disk by
  //   reading in the free block list. To do this:
  // Move the file pointer to the start of the disk file.
  // Read the first 128 bytes (the free/in-use block information)
  // Scan the list to make sure you have sufficient free blocks to
  //   allocate a new file of this size
  disk.seekg(0, ios::beg);
  char *super = (char *)calloc(128, sizeof(char));
  disk.read(super, 128);
  int blocked = 0;
  for (int i = 0; i < 128; i++) {
    blocked += (int)super[i];
  }
  if (128 - blocked < size) {
    perror("not enough space");
    return -1;
  }
  // Step 2: we look for a free inode on disk
  // Read in an inode
  // Check the "used" field to see if it is free
  // If not, repeat the above two steps until you find a free inode
  // Set the "used" field to 1
  // Copy the filename to the "name" field
  // Copy the file size (in units of blocks) to the "size" field
  idxNode inode;
  int idx;
  for (idx = 128; idx < 1024; idx += sizeof(idxNode)) {
    disk.read((char *)&inode, sizeof(idxNode));
    if (inode.used == 0) {
      inode.used = 1;
      strncpy(inode.name, name, 8);
      inode.size = size;
      break;
    }
  }
  if ((idx - 128) / sizeof(idxNode) >= 16) {
    perror("maximum number of files reached");
    return -1;
  }
  // Step 3: Allocate data blocks to the file
  // for(i=0;i<size;i++)
  // Scan the block list that you read in Step 1 for a free block
  // Once you find a free block, mark it as in-use (Set it to 1)
  // Set the blockPointer[i] field in the inode to this block number.
  // end for
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < 128; j++) {
      if (super[j] == 0) {
        inode.blockPointers[i] = j;
        super[j] = 1;
        break;
      }
    }
  }
  // Step 4: Write out the inode and free block list to disk
  // Move the file pointer to the start of the file
  // Write out the 128 byte free block list
  // Move the file pointer to the position on disk where this inode was stored
  // Write out the inode
  disk.seekp(0, ios::beg);
  disk.write(super, 128);
  disk.seekp(idx, ios::beg);
  disk.write((char *)&inode, sizeof(idxNode));
  delete super;
  printf("Created a file %s, at inode %i\n", name,
         (int)((idx - 128) / sizeof(idxNode)));
  return 1;
} // End Create

int myFileSystem::delete_file(char name[8]) {
  // Delete the file with this name

  // Step 1: Locate the inode for this file
  // Move the file pointer to the 1st inode (129th byte)
  // Read in an inode
  // If the inode is free, repeat above step.
  // If the inode is in use, check if the "name" field in the
  //   inode matches the file we want to delete. If not, read the next
  //   inode and repeat
  disk.seekg(128, ios::beg);
  int idx;
  idxNode inode;
  bool found = false;
  for (idx = 128; idx < 1024; idx += sizeof(idxNode)) {
    disk.read((char *)&inode, sizeof(idxNode));
    if (inode.used == 1) {
      if (strncmp(name, inode.name, 8) == 0) {
        found = true;
        break;
      }
    }
  }
  if (!found) {
    perror("deleting file not found");
    return -1;
  }
  // Step 2: free blocks of the file being deleted
  // Read in the 128 byte free block list (move file pointer to start
  //   of the disk and read in 128 bytes)
  // Free each block listed in the blockPointer fields as follows:
  // for(i=0;i< inode.size; i++)
  // freeblockList[ inode.blockPointer[i] ] = 0;
  disk.seekg(0, ios::beg);
  char *freeBlockList = (char *)calloc(128, sizeof(char));
  disk.read(freeBlockList, 128);
  for (int i = 0; i < inode.size; i++) {
    freeBlockList[inode.blockPointers[i]] = 0;
  }
  // Step 3: mark inode as free
  // Set the "used" field to 0.
  inode.used = 0;
  // Step 4: Write out the inode and free block list to disk
  // Move the file pointer to the start of the file
  // Write out the 128 byte free block list
  // Move the file pointer to the position on disk where this inode was stored
  // Write out the inode
  disk.seekp(0, ios::beg);
  disk.write(freeBlockList, 128);
  disk.seekp(idx, ios::beg);
  disk.write((char *)&inode, sizeof(idxNode));
  delete freeBlockList;
  printf("Deleted a file %s, at inode %i\n", name,
         (int)((idx - 128) / sizeof(idxNode)));
  return 1;
} // End Delete

int myFileSystem::ls() {
  // List names of all files on disk

  // Step 1: read in each inode and print
  // Move file pointer to the position of the 1st inode (129th byte)
  // for(i=0;i<16;i++)
  // Read in an inode
  // If the inode is in-use
  // print the "name" and "size" fields from the inode
  // end for
  printf("Listing files\n");
  disk.seekg(128, ios::beg);
  for (int i = 0; i < 16; i++) {
    idxNode inode;
    disk.read((char *)&inode, sizeof(idxNode));
    if (inode.used == 1) {
      printf("%s %i\n", inode.name, inode.size);
    }
  }
  return 1;
} // End ls

int myFileSystem::read(char name[8], int blockNum, char buf[1024]) {
  // read this block from this file

  // Step 1: locate the inode for this file
  // Move file pointer to the position of the 1st inode (129th byte)
  // Read in an inode
  // If the inode is in use, compare the "name" field with the above file
  // If the file names don't match, repeat
  disk.seekg(128, ios::beg);
  int idx;
  bool found = false;
  idxNode inode;
  for (idx = 128; idx < 1024; idx += sizeof(idxNode)) {
    disk.read((char *)&inode, sizeof(idxNode));
    if (inode.used == 1) {
      if (strcmp(inode.name, name) == 0) {
        found = true;
        break;
      }
    }
  }
  if (!found) {
    perror("reading file not found");
    return -1;
  }
  // Step 2: Read in the specified block
  // Check that blockNum < inode.size, else flag an error
  // Get the disk address of the specified block
  // That is, addr = inode.blockPointer[blockNum]
  // Move the file pointer to the block location (i.e., to byte #
  //   addr*1024 in the file)
  if (blockNum >= inode.size) {
    perror("block number not smaller than inode size");
    return -1;
  }
  int addr = inode.blockPointers[blockNum];
  disk.seekg(addr * 1024, ios::beg);
  // Read in the block => Read in 1024 bytes from this location
  //   into the buffer "buf"
  disk.read(buf, 1024);
  printf("Read from file %s at block %i\n", name, blockNum);
  return 1;
} // End read

int myFileSystem::write(char name[8], int blockNum, char buf[1024]) {
  // write this block to this file

  // Step 1: locate the inode for this file
  // Move file pointer to the position of the 1st inode (129th byte)
  // Read in a inode
  // If the inode is in use, compare the "name" field with the above file
  // If the file names don't match, repeat
  disk.seekg(128, ios::beg);
  idxNode inode;
  int idx;
  bool found = false;
  for (idx = 128; idx < 1024; idx += sizeof(idxNode)) {
    disk.read((char *)&inode, sizeof(idxNode));
    if (inode.used == 1) {
      if (strcmp(inode.name, name) == 0) {
        found = true;
        break;
      }
    }
  }
  if (!found) {
    perror("writing file not found");
    return -1;
  }
  // Step 2: Write to the specified block
  // Check that blockNum < inode.size, else flag an error
  // Get the disk address of the specified block
  // That is, addr = inode.blockPointer[blockNum]
  // Move the file pointer to the block location (i.e., byte # addr*1024)
  if (blockNum >= inode.size) {
    perror("block number error");
    return -1;
  }
  int addr = inode.blockPointers[blockNum];
  disk.seekp(addr * 1024, ios::beg);
  // Write the block! => Write 1024 bytes from the buffer "buff" to
  //   this location
  disk.write(buf, 1024);
  printf("writing complete\n");
  return 0;
} // end write

int myFileSystem::close_disk() {
  // close the disk!
  disk.close();
  return 1;
}
