#include <stdio.h>
#include <stdint.h>
#include "bitmap.h"
#include "block_store.h"
// include more if you need

// You might find this handy.  I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)

//Declared struct to not allow users to use object and its contents
typedef struct block_store {
    char* block_data[BLOCK_STORE_AVAIL_BLOCKS][BLOCK_SIZE_BYTES];
    bitmap_t* bitmap;
} block_store_t;

//Creates new block store containing a bitmap
block_store_t *block_store_create()
{
    block_store_t* blockStore = malloc(sizeof(block_store_t));
    if(blockStore != NULL){
        blockStore->bitmap = bitmap_create(BLOCK_STORE_AVAIL_BLOCKS);
        return blockStore;
    }
    
    return NULL;
}

//Destroys the given block storage device
void block_store_destroy(block_store_t *const bs)
{
    if(bs == NULL || bs->bitmap == NULL){
        return;
    }
    bitmap_destroy(bs->bitmap);
        free(bs);
}

size_t block_store_allocate(block_store_t *const bs)
{
    if(bs == NULL){
        return SIZE_MAX;
    }
    
    size_t firstZero = bitmap_ffz(bs->bitmap); //finds the first zero in the bitmap
    if(firstZero > SIZE_MAX || firstZero > BLOCK_STORE_AVAIL_BLOCKS){
        return SIZE_MAX;
    }
    
    bitmap_set(bs->bitmap, firstZero);//set bit in bitmap
    return firstZero;
}

bool block_store_request(block_store_t *const bs, const size_t block_id)
{
    if(bs == NULL || block_id <=0 || block_id > BLOCK_STORE_NUM_BLOCKS - 1){
        return false;
    }
    
    if(bitmap_test(bs->bitmap, block_id)){//checks if bit is already set
        return false;
    }
    
    bitmap_set(bs->bitmap, block_id);
    
    if(bitmap_test(bs->bitmap, block_id) == 0){//checks if bit is not set
        return false;
    }
    
    return true;
    
}

void block_store_release(block_store_t *const bs, const size_t block_id)
{
    if(bs == NULL || block_id >= BLOCK_STORE_NUM_BLOCKS || bitmap_test(bs->bitmap, block_id) == 0){
        return;
    }
    
    bitmap_reset(bs->bitmap, block_id);
    return;
    
}

size_t block_store_get_used_blocks(const block_store_t *const bs)
{
    if(bs == NULL){
        return SIZE_MAX;
    }
    
    return bitmap_total_set(bs->bitmap);//return number of bits set
}

size_t block_store_get_free_blocks(const block_store_t *const bs)
{
    if(bs == NULL){
        return SIZE_MAX;
    }
    
    bitmap_invert(bs->bitmap);
    size_t freeBit = 0;
    freeBit = bitmap_total_set(bs->bitmap);//gets free bits from inverted
    bitmap_invert(bs->bitmap);
    return freeBit;
}

size_t block_store_get_total_blocks()
{
    return 0;
}

size_t block_store_read(const block_store_t *const bs, const size_t block_id, void *buffer)
{
    UNUSED(bs);
    UNUSED(block_id);
    UNUSED(buffer);
    return 0;
}

size_t block_store_write(block_store_t *const bs, const size_t block_id, const void *buffer)
{
    UNUSED(bs);
    UNUSED(block_id);
    UNUSED(buffer);
    return 0;
}

block_store_t *block_store_deserialize(const char *const filename)
{
    UNUSED(filename);
    return NULL;
}

size_t block_store_serialize(const block_store_t *const bs, const char *const filename)
{
    UNUSED(bs);
    UNUSED(filename);
    return 0;
}