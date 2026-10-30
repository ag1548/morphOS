#include "string.h"

#include "types.h"

/*

DESCRIPTION
       The memset() function fills the first n bytes of the memory area pointed to by s with the constant byte c.

RETURN VALUE
       The memset() function returns a pointer to the memory area s.

*/
void *memset(void *s, int c, size_t n) {

    unsigned char *ptr        = static_cast<unsigned char *>(s);
    unsigned char  byte_value = static_cast<unsigned char>(c);

    for (size_t i = 0; i < n; i++) {
        ptr[i] = byte_value;
    }

    return s;
}

/*

DESCRIPTION
       The memcpy() function copies n bytes from memory area src to memory area dest. The memory areas must not overlap. Use memmove(3) if the memory areas do overlap.

RETURN VALUE
       The memcpy() function returns a pointer to dest.

*/
void *memcpy(void *dest, const void *src, size_t n) {

    const unsigned char *ptr_src  = static_cast<const unsigned char *>(src);
    unsigned char       *ptr_dest = static_cast<unsigned char *>(dest);

    for (size_t i = 0; i < n; i++) {
        ptr_dest[i] = ptr_src[i];
    }

    return dest;
}

/*

DESCRIPTION
       The memcmp() function compares the first n bytes (each interpreted as unsigned char) of the memory areas s1 and s2.

RETURN VALUE
       The memcmp() function returns an integer less than, equal to, or greater than zero if the first n bytes of s1 is found, respectively, to be less than, to match, or be greater than the first n bytes of s2.

       For a nonzero return value, the sign is determined by the sign of the difference between the first pair of bytes (interpreted as unsigned char) that differ in s1 and s2.

       If n is zero, the return value is zero.

*/
int memcmp(const void *s1, const void *s2, size_t n) {

    if (!n) {
        return 0;
    }

    const unsigned char *ptr_s1 = static_cast<const unsigned char *>(s1);
    const unsigned char *ptr_s2 = static_cast<const unsigned char *>(s2);

    size_t c = 0;
    while ((c < n) && (*ptr_s1 == *ptr_s2)) {
        c++;
        ptr_s1++;
        ptr_s2++;
    }

    if (c == n) {
        return 0;
    }

    const unsigned char v1 = *ptr_s1;
    const unsigned char v2 = *ptr_s2;

    return v1 < v2;
}

/*

DESCRIPTION
       The memmove() function copies n bytes from memory area src to memory area dest. The memory areas may overlap: copying takes place as though the bytes in src are first copied into a temporary array that does not overlap src or dest, and the bytes are then copied from the temporary array to dest.

RETURN VALUE
        The memmove() function returns a pointer to dest.

*/
void *memmove(void *dest, const void *src, size_t n) {

    if (dest == src) {
        return dest;
    }

    const unsigned char *ptr_src  = static_cast<const unsigned char *>(src);
    unsigned char       *ptr_dest = static_cast<unsigned char *>(dest);

    if (dest < src) {
        for (size_t i = 0; i < n; i++) {
            ptr_dest[i] = ptr_src[i];
        }
    } else /* if (dest > src) */ {
        for (size_t i = n - 1; i >= 0; i--) {
            ptr_dest[i] = ptr_src[i];
        }
    }

    return dest;
}
