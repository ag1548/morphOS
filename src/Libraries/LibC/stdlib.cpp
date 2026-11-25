#include <stdlib.h>

#include <sys/types.h>

void *malloc(size_t size) {
    (void)size;
    return 0;
}
