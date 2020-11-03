#include"toolbox.h"

//keys

u16 key_curr = 0;
u16 key_prev = 0;

//functions

void oam_init(OBJ_ATTRS *obj, u32 count)
{
    u32 nn= count;
    u32 *dst= (u32*)obj;

    // Hide each object
    while(nn--)
    {
        *dst++= 0x0200;
        *dst++= 0;
    }
    // init oam
    oam_copy(oam_mem, obj, count);
}

void oam_copy(OBJ_ATTRS *dst, const OBJ_ATTRS *src, u32 count)
{

// NOTE: while struct-copying is the Right Thing to do here, 
//   there's a strange bug in DKP that sometimes makes it not work
//   If you see problems, just use the word-copy version.
#if 1
    while(count--)
        *dst++ = *src++;
#else
    u32 *dstw= (u32*)dst, *srcw= (u32*)src;
    while(count--)
    {
        *dstw++ = *srcw++;
        *dstw++ = *srcw++;
    }
#endif

}