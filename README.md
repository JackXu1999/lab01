# My Malloc Library

- C: enum, type casting, pointer arithmetic, fprint(), stdout, stderr
- system call: sbrk()


```
#include “my_malloc.h”
void * my_malloc(size_t size);

void my_free(void *ptr);

void coalesce_free_list(void);

FreeListNode * free_list_begin( void );

typedef struct freelistnode {
    struct freelistnode *flink;
    size_t size;
} * FreeListNode;
```

my_malloc() \
allocates size bytes of memory

my_free() \
deallocates memory allocation pointed to by ptr, previously allocated
by my_malloc()

coalesce_free_list() \
merges logically adjacent chunks on the free list into single larger
chunks

free_list_begin() \
retrieves the first node of the free list


RETURN VALUES AND ERRORS \
On success, my_malloc() returns an 8-byte aligned pointer to the
allocated memory.

On failure, my_malloc()sets my_errno to MYENOMEM and returns NULL.

On success, my_free() returns nothing.

On failure, my_free() sets my_errno to MYEBADFREEPTR when passed a
non-malloc’d pointer,

free_list_begin() returns the first free list node or NULL if the list is
empty.