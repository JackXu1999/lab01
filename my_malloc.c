#include <unistd.h>
#include <stddef.h>
#include "my_malloc.h"

struct freelistnode *my_list;

//my_list->size = 0;
//my_list->flink = NULL;

struct returnChunk {
    int size; //size of current chunk
    int check;
};

struct returnChunk *chunk;

int calcSize(size_t size);

int calcSize(size_t size) {
    int remainder = (size + 8) % 8;
    int actual_size = remainder + (size + 8);
    return actual_size;
}

//my_malloc: returns a pointer to a chunk of heap allocated memory
void *my_malloc(size_t size) {
    if (my_list == NULL) {
        // when it does not exist, add the first node to the linked list
        if (calcSize(size) <= 8192) {
            my_list = (FreeListNode)sbrk(8192);
            // the minimum size is 8192

            my_list->flink = NULL;

            // need to split the free space
            if (8912 - calcSize(size) > 16) {
                my_list->size = 8192 - calcSize(size);
            } else {
                // TODO Don't put anything on the freelist
                my_list->size = 0;
            }

            // put size and checksum before the return address
            int *return_size;
            return_size = (int*)((void*)my_list + my_list->size);
            *return_size = size; // size to go in to the address

            int *check;
            check = (int*)((void*)my_list + my_list->size + 4);
            *check = 100;

            return &my_list + my_list->size + 8;
        } else {
            my_list = (FreeListNode)sbrk(calcSize(size));

            my_list->size = 0;
            my_list->flink = NULL;

            // put size and checksum before the return address
            int *return_size;
            return_size = (int*)((void*)my_list + my_list->size);
            *return_size = size; // size to go in to the address

            int *check;
            check = (int*)((void*)my_list + my_list->size + 4);
            *check = 100;


            return &my_list + my_list->size + 8;
        }
    } else {
        // when it already exists, traverse the linked list, my_list != NULL
        while (my_list != NULL) {
            int size_of_node = my_list->size;
            int real_size = calcSize(size);

            if (size_of_node >= real_size) {
                // case 1: when we can find the node for memory, but we need to split
                // case 2: when we can find the node for memory, we do not need to split

                my_list->size = size_of_node - real_size;

                // put size and checksum before the return address
                int *return_size;
                return_size = (int*)((void*)my_list + my_list->size);
                *return_size = size; // size to go in to the address

                int *check;
                check = (int*)((void*)my_list + my_list->size + 4);
                *check = 100;


                //TODO Logic isn't right somewhere

                if (my_list->size < 16) {
                    // not enough space for the node to stay on the freelist
                    my_list->flink = my_list->flink->flink;
                }

                return &my_list + my_list->size + 8;

            } else {
                my_list = my_list->flink;
                // case 3: go to the next node
            }

        }
        // no free space, we have to create a new chunk and link it to the the list
        // similar to the NULL case

        if (size + 8 <= 8192) {
            my_list->flink = (FreeListNode)sbrk(8192);
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
            int *return_size;
            return_size = (int*)((void*)my_list + my_list->size);
            *return_size = size; // size to go in to the address

            int *check;
            check = (int*)((void*)my_list + my_list->size + 4);
            *check = 100;

            return &my_list + my_list->size + 8;
        } else {
            my_list->flink = (FreeListNode)sbrk(calcSize(size));
            my_list = my_list->flink;
            my_list->size = 0;
            my_list->flink = NULL;

            // put size and checksum before the return address
            int *return_size;
            return_size = (int*)((void*)my_list + my_list->size);
            *return_size = size; // size to go in to the address

            int *check;
            check = (int*)((void*)my_list + my_list->size + 4);
            *check = 100;


            return &my_list + my_list->size + 8;
        }

    }
}



//my_free: reclaims the previously allocated chunk referenced by ptr
void my_free(void *ptr) {

}


//free_list_begin(): returns pointer to first chunk in free list
FreeListNode free_list_begin(void) {
    return my_list;
}

//coalesce_free_list(): merge adjacent chunks on the free list
void coalesce_free_list(void) {
    // there is nothing in the list
    if (my_list == NULL) {
        return;
    }

    // free if and only if they are next to each other

    while (my_list->flink != NULL) {
        // stop until there are no nodes on the list can be merged
        int current_size = my_list->size;
        int next_size = my_list->flink->size;

        // add up the size
        my_list->size = current_size + sizeof(my_list->flink);

        // set the pointer so that the next node is removed
        if (my_list->flink->flink != NULL) {
            my_list->flink = my_list->flink->flink;
        } else {
            my_list->flink = NULL;
        }

    }
}

