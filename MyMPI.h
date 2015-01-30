#define BLOCK_FIRST 3 /* first odd prime number */
#define BLOCK_STEP 2  /* loop step to iterate only for odd numbers */

#define MIN(a, b) ((a) < (b)? (a): (b))

#define BLOCK_LOW(id, p, n) \
        ((id) * (n) / (p) / BLOCK_STEP)

#define BLOCK_HIGH(id, p, n) \
        (BLOCK_LOW((id) + 1, p, n) - 1)

#define BLOCK_SIZE(id, p, n) \
        (BLOCK_LOW((id) + 1, p, n) - BLOCK_LOW((id), p, n))

#define BLOCK_OWNER(index, p, n) \
        (((p) * ((index) + 1) - 1) / (n))

#define BLOCK_VALUE_TO_INDEX(val, id, p, n) \
        (val - BLOCK_FIRST) / BLOCK_STEP - BLOCK_LOW(id, p, n - 1)
