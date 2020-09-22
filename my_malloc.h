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

Freelistnode my_list;

//my_malloc: returns a pointer to a chunk of heap allocated memory
void *my_malloc(size_t size) {
    if (my_list == NULL) {
        // when it does not exist, add the first node to the linked list
        if (size + 8 <= 8192) {
            my_list = sbrk(8192);
            // the minimum size is 8192

            my_list->size = calcSize(size);
            my_list->flink = NULL;

            // need to split the free space
            int next = split(size);
            if (size > 0) {
                my_list->flink = &my_list + next;
                my_list->flink->size = 8192 - my_list->size;
            }
            return (&my_list + 8);
        } else {
            my_list = sbrk(calcSize(size));

            my_list->size = calcSize(size);
            my_list->flink = NULL;

            return (&my_list + 8);
        }
    } else {
        // when it already exists, traverse the linked list
        while (my_list->flink != NULL) {
            int size_of_node = my_list->size;

            if (size_of_node > size) {
                // case 1: when we can find the node for memory, but we need to split

            } else if (size_of_node == size) {
                // case 2: when we can find the node for memory, we do not need to split
            } else {
                my_list = my_list->flink;
                // case 3: go to the next node
            }

        }
        // no free space, we have to create a new chunk



        
    }
}

int split(size_t size) {
    int diff = 8192 - size;
    if (diff < 16) {
        return 0;
    } else {
        return 8192 - size;
    }

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
