# Heap Management Scheme

This program implements a simple heap management scheme with the `allocate` and `free` functions Using First Fit Algorithm.
It contains Meta data which stores size of block and pointer to nextblock
A Free List Is maintained where all free block are connected like linked lists
while a particular block frees it ensures that adjacent free blocks are merged together.

## Functionality

- **allocate_block**: Allocates a block of memory of the specified size and ensure that there is no internal fragmentation by splitting the block after allocation
- **free**: It frees previously allocated block and is added to the free list ensuring adjacent free blocks are merged
