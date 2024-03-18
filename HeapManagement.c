#include<stdio.h>
#include<stdlib.h>
#define SIZE_OF_HEAP 500

typedef struct meta_block
{
    size_t size;
    struct meta_block *next;
}M_Block;

M_Block *heap_start=NULL;
M_Block *free_list=NULL;

void init_heap()
{
    heap_start=malloc(SIZE_OF_HEAP); //size of my heap
    free_list=heap_start; //at the start free_list is pointing to heap(as heap is empty at the start)
    free_list->size = (SIZE_OF_HEAP) - sizeof(M_Block);
    free_list->next = NULL;
}

void* allocate_block(size_t size)
{
    if(heap_start==NULL)
       init_heap(); //initializing the heap and free_list

    M_Block *prev = NULL;
    M_Block *ptr = free_list;

    while(ptr!=NULL)
    {
        if(ptr->size >= size) // block of size grater than requested size is found 
        {
            printf("block found and memory allocated\n");
            /*splitting the free block since the requested size is small we cannot allocate the
            total free block to it(to avoid internal fragmentation)*/
            if((ptr->size)-size > sizeof(M_Block)) 
            {
                M_Block *new_splitted_block = (M_Block*)((void*)ptr+size+sizeof(M_Block));
                new_splitted_block->size = (ptr->size) - size - sizeof(M_Block);
                new_splitted_block->next = ptr->next;
                ptr->size = size;
                ptr->next = new_splitted_block;
                printf("block is split\n");
            }

            if(prev == NULL)
            {
                free_list = ptr->next;
            }
            else
            {
                prev->next = ptr->next;
            }

            return (void*)(ptr+1); //returning the pointer which points to the allocated space of heap and not to the meta_data
        }
        printf("one free block checked\n");
        prev = ptr;
        ptr = ptr->next;
    }
    //if any free block is not found in the free list which satisfies the given request it returns NULL
    printf("unable to allocate memory\n");
    return NULL;
}

void free_block(void *ptr)
{
    if(ptr==NULL)
      return;

    M_Block *block = ptr;
    --block; //pointing block to meta data
    M_Block *curr = free_list;
    M_Block *prev = NULL;
    
    /*since we need to find where the newly freed block should be inserted
      in the free_list we have to traverse the free_list*/
    while (curr != NULL && curr < block) 
    {
        prev = curr;
        curr = curr->next;
    }
    
    /* check if previous block and newly freed block are adjacent 
        if they are adjacent merge them both by updating the size */
    if(prev!=NULL && ((M_Block*)((void*)prev + sizeof(M_Block) + prev->size) == block))
    {
        printf("block is merged with previous block in free list\n");
        prev->size += sizeof(M_Block) + block->size;
        block = prev;
    }
    /* check if next block and newly freed block are adjacent 
        if they are adjacent merge them both by updating the size */
    if(curr != NULL && ((M_Block*)((void*)block + sizeof(M_Block) + block->size)) == curr)
    {
        printf("block is merged with the next block in the free list\n");
        block->size += sizeof(M_Block) + curr->size;
        block->next = curr->next;
        if( prev != NULL && prev != block)
        {
            prev->next = block;
        }
        if(prev==NULL)
           free_list=block;
    }
    else
    {
        //insert newly freed block in the free_list
        block->next = curr;
        if (prev == NULL)
            free_list = block;
        else
          if(prev!=block)
            prev->next = block;
    }
    printf("block is freed and added to free list\n");
}
void Display_free_list()
{
    M_Block *ptr=free_list;
    printf("BLOCK_SIZE\tADDRESS");
    while(ptr)
    {
        printf("\n%ld\t\t%p",ptr->size,ptr);
        ptr=ptr->next;
    }
    printf("\n");
}

int main()
{
    
    int *a = (int *)allocate_block(100);
    Display_free_list();
    int *b = (int *)allocate_block(100);
    Display_free_list();
    int *c = (int *)allocate_block(100);
    Display_free_list();
    int *d = (int *)allocate_block(100);
    Display_free_list();
    free_block(b);
    Display_free_list();
    free_block(c);
    Display_free_list();
    int *e=(int *)allocate_block(200);
    printf("the free list available for allocation is:\n");
    Display_free_list();
    free(heap_start);
    return 0;
}
