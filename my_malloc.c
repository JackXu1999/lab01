This is my own code -- Shengkai Xu

#include <unistd.h>
#include <stddef.h>
#include "my_malloc.h"

FreeListNode my_list = NULL;

MyErrorNo my_errno;

int calcSize(size_t size);

int calcSize(size_t size) {
    int remainder = (size + 8) % 8;
    int actual_size = remainder + (size + 8);
    return actual_size;
}

//my_malloc: returns a pointer to a chunk of heap allocated memory
void *my_malloc(size_t size) {


    if (size < 0) {
        my_errno = MYENOMEM;
        return NULL;
    }

    if (my_list == NULL) {
        // when it does not exist, add the first node to the linked list
        if (calcSize(size) <= 8192) {
            my_list = (FreeListNode)sbrk(8192);
            // the minimum size is 8192

            my_list->flink = NULL;

            // need to split the free space
            if (8192 - calcSize(size) >= 16) {
                my_list->size = 8192 - calcSize(size);

                // put size and checksum before the return address
                int *return_size;
                return_size = (int*)((void*)my_list + my_list->size);
                *return_size = calcSize(size); // size to go in to the address

                int *check;
                check = (int*)((void*)my_list + my_list->size + 4);
                *check = 100;

                return (char *)my_list + my_list->size + 8;

            } else {
                // don't put anything on the freelist

                my_list->size = 0;

                // put size and checksum before the return address
                int *return_size;
                return_size = (int*)((void*)my_list + my_list->size);
                *return_size = calcSize(size); // size to go in to the address

                int *check;
                check = (int*)((void*)my_list + my_list->size + 4);
                *check = 100;

                my_list = NULL;
                return (char *)sbrk(0) + 8;
            }

        } else {

            my_list = (FreeListNode)sbrk(calcSize(size));

            my_list->size = 0;
            my_list->flink = NULL;

            // put size and checksum before the return address
            int *return_size;
            return_size = (int*)((void*)my_list + my_list->size);
            *return_size = calcSize(size); // size to go in to the address

            int *check;
            check = (int*)((void*)my_list + my_list->size + 4);
            *check = 100;

            // don't put anything on the freelist
            my_list = NULL;
            return (char*)sbrk(0) + 8;
        }
    } else {
        // when it already exists, traverse the linked list, my_list != NULL

        // create a node to traverse the linked list

        struct freelistnode *node;
        struct  freelistnode *prev;

        node = my_list;
        prev = NULL;

        while (node != NULL) {
            int size_of_node = node->size;
            int real_size = calcSize(size);

            if (size_of_node - real_size >= 16) {
                // case 1: when we can find the node for memory, but we need to split
                node->size = size_of_node - real_size;

                // put size and checksum before the return address
                int *return_size;
                return_size = (int*)((void*)node + node->size);
                *return_size = calcSize(size); // size to go in to the address

                int *check;
                check = (int*)((void*)node + node->size + 4);
                *check = 100;

                return (char *)my_list + my_list->size + 8;

            } else if (size_of_node - real_size > 0) {
                // case 2: we return the whole chuck to the user and remove it on the list

                node->size = 0;

                // put size and checksum before the return address
                int *return_size;
                return_size = (int*)((void*)node + node->size);
                *return_size = calcSize(size); // size to go in to the address

                int *check;
                check = (int*)((void*)node + node->size + 4);
                *check = 100;

                // node is not the first node, remove node from the list
                prev->flink = node->flink;

                node = NULL;
                return (char *)sbrk(0) + 8;

            }
                prev = node;
                node = node->flink;
                // go to the next node


        }
        // no free space, we have to create a new chunk and link it to the the list
        // similar to the NULL case

        if (calcSize(size) <= 8192) {
            node = (FreeListNode) sbrk(8192);
            // the minimum size is 8192

            node->flink = NULL;

            // need to split the free space
            if (8912 - calcSize(size) >= 16) {
                node->size = 8192 - calcSize(size);


                // put size and checksum before the return address
                int *return_size;
                return_size = (int *) ((void *) node + node->size);
                *return_size = calcSize(size); // size to go in to the address

                int *check;
                check = (int *) ((void *) node + node->size + 4);
                *check = 100;

                prev->flink = node;
                return (char *)node + node->size + 8;

            } else {
                // don't put anything on the freelist

                node->size = 0;

                // put size and checksum before the return address
                int *return_size;
                return_size = (int *) ((void *) node + node->size);
                *return_size = calcSize(size); // size to go in to the address

                int *check;
                check = (int *) ((void *) node + node->size + 4);
                *check = 100;

                node = NULL;
                prev->flink = NULL;
                return (char *)sbrk(0) + 8;
            }
        } else {
            node = (FreeListNode)sbrk(calcSize(size));

            node->size = 0;
            node->flink = NULL;

            // put size and checksum before the return address
            int *return_size;
            return_size = (int *) ((void *) node + node->size);
            *return_size = calcSize(size); // size to go in to the address

            int *check;
            check = (int *) ((void *) node + node->size + 4);
            *check = 100;

            node = NULL;
            // don't put anything on the freelist
            prev->flink = NULL;
            return sbrk(0) + 8;
        }
    }
}

//my_free: reclaims the previously allocated chunk referenced by ptr
void my_free(void *ptr) {
    size_t size_of_chunk;
    struct freelistnode *new_node;
    struct freelistnode *node;
    struct  freelistnode *prev;

    node = my_list;
    prev = NULL;

    // TODO the pointers needs to be fixed
    if ((*(char*)(ptr - 4)) == 100) {

        size_of_chunk = *(size_t*)(char*)(ptr - 8);

        // ptr is between first node address and second node address

        // TODO what if node->flink does not exist

        // nothing on the list
        if (my_list == NULL) {
            my_list = (FreeListNode) ptr;
            my_list->size = size_of_chunk;
            my_list->flink = NULL;
            return;

        } else if (my_list->flink == NULL) {
            // only head on the list

            // insert as second node
            if ((void*)ptr >= (void *)my_list) {
                new_node = (FreeListNode) ptr;
                new_node->size = size_of_chunk;
                my_list->flink = new_node;
                new_node->flink = NULL;
                return;
            } else {
                // insert as first node;
                new_node = my_list;
                my_list = (FreeListNode) ptr;
                my_list->size = size_of_chunk;
                my_list->flink = new_node;
                new_node->flink = NULL;
                return;
            }


        }

        while (node != NULL) {
            prev = node;
            node = node->flink;

            // address of ptr is in between the address of prev and node
            if ((void*)ptr >= (void *)prev && ptr <= (void *)node) {
                new_node = (FreeListNode) ptr;
                new_node->size = size_of_chunk;

                new_node->flink = node;
                prev->flink = new_node;
                return;
            }
        }

        // now, node = NULL
        new_node = (FreeListNode) ptr;
        new_node->size = size_of_chunk;

        new_node->flink = NULL;
        prev->flink = new_node;
        return;


    } else {
        my_errno = MYBADFREEPTR;
        return;
    }
}


//free_list_begin(): returns pointer to first chunk in free list
FreeListNode free_list_begin(void) {
    return my_list;
}

//coalesce_free_list(): merge adjacent chunks on the free list

void coalesce_free_list(void) {
    // there is nothing in the list

    struct freelistnode *node;

    if (my_list == NULL) {
        return;
    }

    node = my_list;

    while (node->flink != NULL) {
        // stop until there are no nodes on the list can be merged
        int current_size = node->size;
        int next_size = node->flink->size;

        if (node + node->size == node->flink) {
            // add up the size
            node->size = current_size + next_size;

            // set the pointer so that the next node is removed
            if (node->flink->flink != NULL) {
                node->flink = node->flink->flink;
            } else {
                node->flink = NULL;
            }
        } else {
            node = node->flink;
        }



    }
    return;

}

