#ifndef TOOLBOX
#define TOOLBOX

#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 160

#define MEM_IO   0x04000000
#define MEM_PAL  0x05000000
#define MEM_VRAM 0x06000000
#define MEM_OAM  0x07000000

#define REG_DISPCNT        (*((volatile u32 *)(MEM_IO)))
#define REG_DISPLAY_VCOUNT (*((volatile u32 *)(MEM_IO + 0x0006)))
#define REG_KEYINPUT      (*((volatile u32 *)(MEM_IO + 0x0130)))

#define REG_BGCNT          ((volatile u16*)(MEM_IO + 0x0008))
#define REG_BG_OFFS		   ((volatile BG_POINT*)(MEM_IO + 0x0010))

//dispplay control

#define DCNT_MODE0				 0	//!< Mode 0; bg 0-4: reg
#define DCNT_MODE1			0x0001	//!< Mode 1; bg 0-1: reg; bg 2: affine
#define DCNT_MODE2			0x0002	//!< Mode 2; bg 2-3: affine
#define DCNT_MODE3			0x0003	//!< Mode 3; bg2: 240x160\@16 bitmap
#define DCNT_MODE4			0x0004	//!< Mode 4; bg2: 240x160\@8 bitmap
#define DCNT_MODE5			0x0005	//!< Mode 5; bg2: 160x128\@16 bitmap
#define DCNT_GB				0x0008	//!< (R) GBC indicator
#define DCNT_PAGE			0x0010	//!< Page indicator
#define DCNT_OAM_HBL		0x0020	//!< Allow OAM updates in HBlank
#define DCNT_OBJ_2D				 0	//!< OBJ-VRAM as matrix
#define DCNT_OBJ_1D			0x0040	//!< OBJ-VRAM as array
#define DCNT_BLANK			0x0080	//!< Force screen blank
#define DCNT_BG0			0x0100	//!< Enable bg 0
#define DCNT_BG1			0x0200	//!< Enable bg 1
#define DCNT_BG2			0x0400	//!< Enable bg 2
#define DCNT_BG3			0x0800	//!< Enable bg 3
#define DCNT_OBJ			0x1000	//!< Enable objects
#define DCNT_WIN0			0x2000	//!< Enable window 0
#define DCNT_WIN1			0x4000	//!< Enable window 1
#define DCNT_WINOBJ			0x8000	//!< Enable object window

//keys

#define KEY_DOWN_NOW(key)  (~(REG_KEYINPUT) & key)
#define KEY_PRESSED(key)  ((key_curr & ~key_prev) & key)
#define KEY_RELEASED(key)  ((~key_curr & key_prev) & key)
#define KEY_HELD(key)  ((key_curr & key_prev) & key)

#define KEY_A        0x0001
#define KEY_B        0x0002
#define KEY_SELECT   0x0004
#define KEY_START    0x0008
#define KEY_RIGHT    0x0010
#define KEY_LEFT     0x0020
#define KEY_UP       0x0040
#define KEY_DOWN     0x0080
#define KEY_R        0x0100
#define KEY_L        0x0200
#define KEY_MASK     0x03FF

//object attrs

#define ATTR0_REG				 0	//!< Regular object
#define ATTR0_AFF			0x0100	//!< Affine object
#define ATTR0_HIDE			0x0200	//!< Inactive object
#define ATTR0_AFF_DBL		0x0300	//!< Double-size affine object
#define ATTR0_AFF_DBL_BIT	0x0200
#define ATTR0_BLEND			0x0400	//!< Enable blend
#define ATTR0_WINDOW		0x0800	//!< Use for object window
#define ATTR0_MOSAIC		0x1000	//!< Enable mosaic
#define ATTR0_4BPP				 0	//!< Use 4bpp (16 color) tiles
#define ATTR0_8BPP			0x2000	//!< Use 8bpp (256 color) tiles
#define ATTR0_SQUARE			 0	//!< Square shape
#define ATTR0_WIDE			0x4000	//!< Tall shape (height &gt; width)
#define ATTR0_TALL			0x8000	//!< Wide shape (height &lt; width)

#define ATTR0_Y_MASK		0x00FF
#define ATTR0_Y_SHIFT			 0
#define ATTR0_Y(n)			((n)<<ATTR0_Y_SHIFT)

#define ATTR0_MODE_MASK		0x0300
#define ATTR0_MODE_SHIFT		 8
#define ATTR0_MODE(n)		((n)<<ATTR0_MODE_SHIFT)

#define ATTR0_SHAPE_MASK	0xC000
#define ATTR0_SHAPE_SHIFT		14
#define ATTR0_SHAPE(n)		((n)<<ATTR0_SHAPE_SHIFT)

#define ATTR1_HFLIP			0x1000	//!< Horizontal flip (reg obj only)
#define ATTR1_VFLIP			0x2000	//!< Vertical flip (reg obj only)
#define ATTR1_SIZE_0			 0
#define ATTR1_SIZE_1		0x4000
#define ATTR1_SIZE_2		0x8000
#define ATTR1_SIZE_3		0xC000
// Square sizes
#define ATTR1_SIZE_8x8			 0	//!< Size flag for  8x8 px object
#define ATTR1_SIZE_16x16	0x4000	//!< Size flag for 16x16 px object
#define ATTR1_SIZE_32x32	0x8000	//!< Size flag for 32x32 px object
#define ATTR1_SIZE_64x64	0xC000	//!< Size flag for 64x64 px object
// Tall sizes
#define ATTR1_SIZE_8x16			 0	//!< Size flag for  8x16 px object
#define ATTR1_SIZE_8x32		0x4000	//!< Size flag for  8x32 px object
#define ATTR1_SIZE_16x32	0x8000	//!< Size flag for 16x32 px object
#define ATTR1_SIZE_32x64	0xC000	//!< Size flag for 32x64 px object
// Wide sizes
#define ATTR1_SIZE_16x8			 0	//!< Size flag for 16x8 px object
#define ATTR1_SIZE_32x8		0x4000	//!< Size flag for 32x8 px object
#define ATTR1_SIZE_32x16	0x8000	//!< Size flag for 32x16 px object
#define ATTR1_SIZE_64x32	0xC000	//!< Size flag for 64x64 px object

#define ATTR1_X_MASK		0x01FF
#define ATTR1_X_SHIFT			 0
#define ATTR1_X(n)			((n)<<ATTR1_X_SHIFT)

#define ATTR1_AFF_ID_MASK	0x3E00
#define ATTR1_AFF_ID_SHIFT		 9
#define ATTR1_AFF_ID(n)		((n)<<ATTR1_AFF_ID_SHIFT)

#define ATTR1_FLIP_MASK		0x3000
#define ATTR1_FLIP_SHIFT		12
#define ATTR1_FLIP(n)		((n)<<ATTR1_FLIP_SHIFT)

#define ATTR1_SIZE_MASK		0xC000
#define ATTR1_SIZE_SHIFT		14
#define ATTR1_SIZE(n)		((n)<<ATTR1_SIZE_SHIFT)

#define ATTR2_ID_MASK		0x03FF
#define ATTR2_ID_SHIFT			 0
#define ATTR2_ID(n)			((n)<<ATTR2_ID_SHIFT)

#define ATTR2_PRIO_MASK		0x0C00
#define ATTR2_PRIO_SHIFT		10
#define ATTR2_PRIO(n)		((n)<<ATTR2_PRIO_SHIFT)

#define ATTR2_PALBANK_MASK	0xF000
#define ATTR2_PALBANK_SHIFT		12
#define ATTR2_PALBANK(n)	((n)<<ATTR2_PALBANK_SHIFT)

//background

#define BG_MOSAIC		0x0040	//!< Enable Mosaic
#define BG_4BPP				 0	//!< 4bpp (16 color) bg (no effect on affine bg)
#define BG_8BPP			0x0080	//!< 8bpp (256 color) bg (no effect on affine bg)
#define BG_WRAP			0x2000	//!< Wrap around edges of affine bgs
#define BG_SIZE0			 0
#define BG_SIZE1		0x4000
#define BG_SIZE2		0x8000
#define BG_SIZE3		0xC000
#define BG_REG_32x32		 0	//!< reg bg, 32x32 (256x256 px)
#define BG_REG_64x32	0x4000	//!< reg bg, 64x32 (512x256 px)
#define BG_REG_32x64	0x8000	//!< reg bg, 32x64 (256x512 px)
#define BG_REG_64x64	0xC000	//!< reg bg, 64x64 (512x512 px)
#define BG_AFF_16x16		 0	//!< affine bg, 16x16 (128x128 px)
#define BG_AFF_32x32	0x4000	//!< affine bg, 32x32 (256x256 px)
#define BG_AFF_64x64	0x8000	//!< affine bg, 64x64 (512x512 px)
#define BG_AFF_128x128	0xC000	//!< affine bg, 128x128 (1024x1024 px)

#define BG_PRIO_MASK	0x0003
#define BG_PRIO_SHIFT		 0
#define BG_PRIO(n)		((n)<<BG_PRIO_SHIFT)

#define BG_CBB_MASK		0x000C
#define BG_CBB_SHIFT		 2
#define BG_CBB(n)		((n)<<BG_CBB_SHIFT)

#define BG_SBB_MASK		0x1F00
#define BG_SBB_SHIFT		 8
#define BG_SBB(n)		((n)<<BG_SBB_SHIFT)

#define BG_SIZE_MASK	0xC000
#define BG_SIZE_SHIFT		14
#define BG_SIZE(n)		((n)<<BG_SIZE_SHIFT)

//colors

#define CLR_RED      0x001F
#define CLR_GREEN    0x03E0
#define CLR_BLUE     0x7C00
#define CLR_PURPLE   0x7C1F
#define CLR_YELLOW   0x03FF
#define CLR_CYAN	 0x7FE0
#define CLR_WHITE    0x7FFF

#define BIT_MASK(len)       ( (1<<(len))-1 )

#define ALIGN4  __attribute__((aligned(4)))

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

typedef signed char    s8;
typedef signed short   s16;
typedef signed int     s32;

typedef u32 TILE[8];
typedef u32 TILE8[16];

typedef TILE TILEBLOCK[512];
typedef TILE8 TILEBLOCK8[256];

typedef u16 SCR_ENTRY;

typedef SCR_ENTRY   SCREENBLOCK[1024];

typedef struct bg_point
{
    s16 x, y;
} ALIGN4 BG_POINT;

typedef struct obj_attrs {
	u16 attr0;
	u16 attr1;
	u16 attr2;
	u16 pad;
} ALIGN4 OBJ_ATTRS;

#define oam_mem            ((volatile OBJ_ATTRS *)MEM_OAM)
#define tile_mem		   ((TILEBLOCK *)(MEM_VRAM))
#define tile8_mem		   ((TILEBLOCK8 *)(MEM_VRAM))
#define se_mem			   ((SCREENBLOCK *)(MEM_VRAM))
#define vid_mem			   ((volatile u16 *)(MEM_VRAM))
#define vid_mem_back       ((volatile u16 *)(MEM_VRAM + 0xA000))
#define pal_mem        ((volatile u16 *)(MEM_PAL))
#define obj_pal_mem    ((volatile u16 *)(MEM_PAL + 0x200))

extern u16 key_curr, key_prev;

void oam_init(OBJ_ATTRS *obj, u32 count);
void oam_copy(OBJ_ATTRS *dst, const OBJ_ATTRS *src, u32 count);

//inlines

static inline void set_obj_attrs(volatile OBJ_ATTRS *object, u16 a0, u16 a1, u16 a2){
	object->attr0 = a0;
	object->attr1 = a1;
	object->attr2 = a2;
}

static inline void set_obj_pos(volatile OBJ_ATTRS *object, int x,
                                       int y)
{
	object->attr0 = (object->attr0 & ~ATTR0_Y_MASK) | (y & ATTR0_Y_MASK);
	object->attr1 = (object->attr1 & ~ATTR1_X_MASK) | (x & ATTR1_X_MASK);
}

static inline void set_obj_x(volatile OBJ_ATTRS *object, int x)
{
	object->attr1 = (object->attr1 & ~ATTR1_X_MASK) | (x & ATTR1_X_MASK);
}

static inline void set_obj_y(volatile OBJ_ATTRS *object, int y)
{
	object->attr0 = (object->attr0 & ~ATTR0_Y_MASK) | (y & ATTR0_Y_MASK);
}

static inline int get_obj_x(volatile OBJ_ATTRS object){
	return (int)object.attr1 & ATTR1_X_MASK;
}

static inline int get_obj_y(volatile OBJ_ATTRS object){
	return (int)object.attr0 & ATTR0_Y_MASK;
}

static inline void set_obj_basetile(volatile OBJ_ATTRS *object, int basetile){
	object->attr2 = (object->attr2 & ~ATTR2_ID_MASK) | (basetile & ATTR2_ID_MASK);
}

static inline int get_obj_basetile(volatile OBJ_ATTRS object){
	return (int)object.attr2 & ATTR2_ID_MASK;
}

static inline int clamp(int value, int min, int max)
{
	return (value < min ? min
	                    : (value > max ? max : value));
}

static inline void key_poll(){
	key_prev = key_curr;
	key_curr = ~(REG_KEYINPUT) & KEY_MASK;
}

static inline void vsync(){
	while(REG_DISPLAY_VCOUNT >= 160);
	while(REG_DISPLAY_VCOUNT < 160);
}

static inline void set_bg_offset(int bg, int x, int y){
	REG_BG_OFFS[bg].x = x;
	REG_BG_OFFS[bg].y = y;
}

static inline u16 rgb15(int r, int g, int b){
	return r | (g << 5) | (b << 10);
}

#endif