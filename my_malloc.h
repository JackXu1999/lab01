//
//  my_malloc.h
//  Lab1: Malloc
//
//  Created by Dorian Arnold on 7/17/20.
//  Copyright © 2020 Dorian Arnold. All rights reserved.
//

#ifndef my_malloc_h
#define my_malloc_h

//the size of the header for heap allocated memory chunks
#define CHUNKHEADERSIZE 8

//error signaling
typedef enum {MYNOERROR, MYENOMEM, MYBADFREEPTR} MyErrorNo;
extern MyErrorNo my_errno;

Freelistnode my_list = NULL;

//my_malloc: returns a pointer to a chunk of heap allocated memory
void *my_malloc(size_t size) {
    if (my_list == NULL) {
        // when it does not exist
        if (size + 8 <= 8192) {
            my_list = sbrk(8192);

            my_list->size = calcSize(size);
            my_list->flink = NULL;

            // need to split the free space

            return (my_list + 8);
        } else {
            my_list = sbrk(size);

            my_list->size = calcSize(size);
            my_list->flink = NULL;

            return (my_list + 8);
        }
    } else {
        // when it already exists

        // case 1: when we can find the node for memory
        while (my_list)


        // case 2: when we cannot find the node for memory
        
    }
}

void split(FreeListNode *pointer, size_t size) {
    
}

int calcSize(int size) {
    int remainder = (size + 8) % 8;
    int actual_size = remainder + (size + 8);
    return actual_size;
}

//my_free: reclaims the previously allocated chunk referenced by ptr
void my_free(void *ptr);

//struct freelistnode: node for linked list of 'free' chunks
typedef struct freelistnode {
    struct freelistnode *flink; //pointer to next free chunk node
    size_t size; //size of current chunk
} * FreeListNode;

//free_list_begin(): returns pointer to first chunk in free list
FreeListNode free_list_begin(void) {
    
}

//coalesce_free_list(): merge adjacent chunks on the free list
void coalesce_free_list(void);

#endif /* my_malloc_h */
