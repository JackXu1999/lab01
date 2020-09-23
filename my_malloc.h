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

*Freelistnode my_list;

//my_malloc: returns a pointer to a chunk of heap allocated memory
void *my_malloc(size_t size) {
    if (my_list == NULL) {
        // when it does not exist, add the first node to the linked list
        if (size + 8 <= 8192) {
            my_list = (Freelistnode)sbrk(8192);
            // the minimum size is 8192

            my_list->flink = NULL;

            // need to split the free space
            if (8912 - size > 16) {
                my_list->size = 8192 - calcSize(size);
            } else {
                my_list->size = 0;
            }

            // put size and checksum before the return address
            int *return_size = &my_list + my_list->size;
            return_size = size;
            char *check = &my_list + my_list->size + 4;
            check = 'a';

            return &my_list + my_list->size + 8;
        } else {
            my_list = sbrk(calcSize(size));

            my_list->size = 0;
            my_list->flink = NULL;

            int *return_size = &my_list + my_list->size;
            return_size = size;
            char *check = &my_list + my_list->size + 4;
            check = 'a';

            return &my_list + my_list->size + 8;
        }
    } else {
        // when it already exists, traverse the linked list
        while (my_list->flink != NULL) {
            int size_of_node = my_list->size;
            int real_size = calcSize(size);

            if (size_of_node >= real_size) {
                // case 1: when we can find the node for memory, but we need to split
                // case 2: when we can find the node for memory, we do not need to split
                // int next = size_of_node - real_size;
                my_list->size = size_of_node - real_size;

                int *return_size = &my_list + my_list->size;
                return_size = size;
                char *check = &my_list + my_list->size + 4;
                check = 'a';

                return &my_list + my_list->size + 8;

            } else {
                my_list = my_list->flink;
                // case 3: go to the next node
            }

        }
        // no free space, we have to create a new chunk and link it to the the list
        // similar to the NULL case

        if (size + 8 <= 8192) {
            my_list->flink = (Freelistnode)sbrk(8192);
            my_list = my_list->flink;
            // the minimum size is 8192

            my_list->flink = NULL;

            // need to split the free space
            if (8912 - size > 16) {
                my_list->size = 8192 - calcSize(size);
            } else {
                my_list->size = 0;
            }

            // put size and checksum before the return address
            int *return_size = &my_list + my_list->size;
            return_size = size;
            char *check = &my_list + my_list->size + 4;
            check = 'a';

            return &my_list + my_list->size + 8;
        } else {
            my_list->flink = sbrk(calcSize(size));
            my_list = my_list->flink;
            my_list->size = 0;
            my_list->flink = NULL;

            int *return_size = &my_list + my_list->size;
            return_size = size;
            char *check = &my_list + my_list->size + 4;
            check = 'a';

            return &my_list + my_list->size + 8;
        }
        
    }
}

int calcSize(size_t size) {
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
