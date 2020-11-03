#include<string.h>
#include"toolbox.h"
#include"boxmenu.h"
#include"levels.h"
#include"totem.h"
#include"box.h"

typedef enum direction { UP, DOWN, LEFT, RIGHT } DIRECTION;

typedef struct point {
	int x;
	int y;
} point;

#define colmap_index 24
#define map_index 16
#define height 16
#define width 16
#define pusher_velocity 2
#define COL_FLAG 1
#define GOAL_FLAG 2
#define BOX_FLAG 3
#define PUSHER_FLAG 4
#define hold_time 20

OBJ_ATTRS obj_buffer[128];

point tile_index(int x, int y){
	point index;
	index.x = ((x + 8) % 256)/8;
	index.y = ((y + 8) % 256)/8;
	return index;
}

int se_index(int tx, int ty, int pitch){
    int sbb= (ty/32)*(pitch/32) + (tx/32);
    return sbb*1024 + (ty%32)*32 + tx%32;
}

void find(int *x, int *y, int flag){
	int i = 0;
	while(i<1024 && se_mem[colmap_index][i] != flag) i++;
	*x = (i%32 - 1)*8;
	*y = (i/32 - 1)*8;
}

int isCollision(volatile OBJ_ATTRS obj ,DIRECTION direction){
	int i;

	if(direction == UP){
		for(i = 0;i <= 2; i++){
			if(se_mem[colmap_index][se_index(tile_index(get_obj_x(obj)+8*i-i/2, get_obj_y(obj)-1).x, tile_index(get_obj_x(obj)+8*i-i/2, get_obj_y(obj)-1).y, 64)] == 1) return 1;
		}
	}else if(direction == RIGHT){
		for(i = 0;i <= 2; i++){
			if(se_mem[colmap_index][se_index(tile_index(get_obj_x(obj)+width+1, get_obj_y(obj)+8*i-i/2).x, tile_index(get_obj_x(obj)+width+1, get_obj_y(obj)+8*i-i/2).y, 64)] == 1) return 1;
		}
	}else if(direction == DOWN){
		for(i = 0;i <= 2; i++){
			if(se_mem[colmap_index][se_index(tile_index(get_obj_x(obj)+8*i-i/2, get_obj_y(obj)+height+1).x, tile_index(get_obj_x(obj)+8*i-i/2, get_obj_y(obj)+height+1).y, 64)] == 1) return 1;
		}
	}else if(direction == LEFT){
		for(i = 0;i <= 2; i++){
			if(se_mem[colmap_index][se_index(tile_index(get_obj_x(obj)-1, get_obj_y(obj)+8*i-i/2).x, tile_index(get_obj_x(obj)-1, get_obj_y(obj)+8*i-i/2).y, 64)] == 1) return 1;
		}
	}
	
	return 0;
}

int isPushing(volatile OBJ_ATTRS pusher, volatile OBJ_ATTRS box, DIRECTION direction){

	if(direction == UP){
		if(get_obj_x(pusher) < get_obj_x(box) + width && get_obj_x(pusher) + width > get_obj_x(box) && get_obj_y(pusher) - 1 < get_obj_y(box) + height && get_obj_y(pusher) + height > get_obj_y(box)) return 1;
	}else if(direction == RIGHT){
		if(get_obj_x(pusher) < get_obj_x(box) + width && get_obj_x(pusher) + width + 1 > get_obj_x(box) && get_obj_y(pusher) < get_obj_y(box) + height && get_obj_y(pusher) + height > get_obj_y(box)) return 1;
	}else if(direction == DOWN){
		if(get_obj_x(pusher) < get_obj_x(box) + width && get_obj_x(pusher) + width > get_obj_x(box) && get_obj_y(pusher) < get_obj_y(box) + height && get_obj_y(pusher) + height + 1 > get_obj_y(box)) return 1;
	}else if(direction == LEFT){
		if(get_obj_x(pusher) - 1 < get_obj_x(box) + width && get_obj_x(pusher) + width > get_obj_x(box) && get_obj_y(pusher) < get_obj_y(box) + height && get_obj_y(pusher) + height > get_obj_y(box)) return 1;
	}
	
	return 0;
}

int menu(){
	REG_DISPCNT = DCNT_MODE4 |  DCNT_BG2 | DCNT_OBJ | DCNT_OBJ_1D;

	memcpy(vid_mem, boxmenuBitmap, boxmenuBitmapLen);
	memcpy(pal_mem, boxmenuPal, boxmenuPalLen);

	memcpy(&tile_mem[5][0], pointerBitmap, pointerBitmapLen);
	memcpy(obj_pal_mem, boxmenuPal, boxmenuPalLen);

	volatile OBJ_ATTRS * pointer = &oam_mem[0];
	set_obj_attrs(pointer, 0, ATTR1_SIZE_8x8, ATTR2_ID(512));

	int x = 72;
	int y = 40;

	set_obj_pos(pointer, x, y);

	int index = 1;

	while(1){

		vsync();
		key_poll();

		//navigation

		if(KEY_PRESSED(KEY_DOWN)) index++;
		else if(KEY_PRESSED(KEY_UP)) index--;

		index = clamp(index, 1, 1);

		set_obj_y(pointer, y + (index - 1)*16);

		if(KEY_PRESSED(KEY_A)) return index;
		
	}
}

void game(){
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG2 | DCNT_OBJ | DCNT_OBJ_1D;

	oam_init(obj_buffer, 128);

	memcpy(&tile_mem[4][0], totemBitmap, totemBitmapLen);
	memcpy(obj_pal_mem, totemPal, totemPalLen);

	memcpy(&tile_mem[4][4], boxBitmap, boxBitmapLen);
	memcpy(&obj_pal_mem[16], boxPal, boxPalLen);

	memcpy(tile_mem, levelTiles, levelTilesLen);
	memcpy(pal_mem, levelPal, levelPalLen);
	memcpy(&se_mem[map_index][0], level2Map, levelMapLen);
	memcpy(&se_mem[colmap_index][0], level2colMap, levelcolMapLen);

	REG_BGCNT[2] = BG_CBB(0) | BG_SBB(16) | BG_REG_32x32;

	set_bg_offset(2, 8, 8);

	volatile OBJ_ATTRS* pusher = &obj_buffer[0];
	set_obj_attrs(pusher, ATTR0_SQUARE, ATTR1_SIZE_16x16, ATTR2_ID(0));

	volatile OBJ_ATTRS* box = &obj_buffer[1];
	set_obj_attrs(box, ATTR0_SQUARE, ATTR1_SIZE_16x16, ATTR2_ID(4) | ATTR2_PALBANK(1));

	int x, y;

	int bx, by;

	int gx, gy;

	find(&x, &y, PUSHER_FLAG);
	find(&bx, &by, BOX_FLAG);
	find(&gx, &gy, GOAL_FLAG);

	set_obj_pos(pusher, x, y);
	set_obj_pos(box, bx, by);

	int counter = 0;

	while(1){
		vsync();
		key_poll();

		//move

		if(KEY_PRESSED(KEY_UP) && !isCollision(*pusher, UP) && !isPushing(*pusher, *box, UP)){ y -= pusher_velocity;}
		if(KEY_PRESSED(KEY_DOWN) && !isCollision(*pusher, DOWN) && !isPushing(*pusher, *box, DOWN)){ y += pusher_velocity;}
		if(KEY_PRESSED(KEY_RIGHT) && !isCollision(*pusher, RIGHT) && !isPushing(*pusher, *box, RIGHT)){ x += pusher_velocity;}
		if(KEY_PRESSED(KEY_LEFT) && !isCollision(*pusher, LEFT) && !isPushing(*pusher, *box, LEFT)){ x -= pusher_velocity;}

		if(KEY_DOWN_NOW(KEY_UP) && !isCollision(*pusher, UP) && !isPushing(*pusher, *box, UP))if(counter > hold_time){ y -= pusher_velocity;}else counter++;
		if(KEY_DOWN_NOW(KEY_DOWN) && !isCollision(*pusher, DOWN) && !isPushing(*pusher, *box, DOWN))if(counter > hold_time){ y += pusher_velocity;}else counter++;
		if(KEY_DOWN_NOW(KEY_RIGHT) && !isCollision(*pusher, RIGHT) && !isPushing(*pusher, *box, RIGHT))if(counter > hold_time){ x += pusher_velocity;}else counter++;
		if(KEY_DOWN_NOW(KEY_LEFT) && !isCollision(*pusher, LEFT) && !isPushing(*pusher, *box, LEFT))if(counter > hold_time){ x -= pusher_velocity;}else counter++;

		x = clamp(x, 0, SCREEN_WIDTH - width);
		y = clamp(y, 0, SCREEN_HEIGHT - height);

		//push

		if(KEY_PRESSED(KEY_UP) && !isCollision(*pusher, UP) && isPushing(*pusher, *box, UP) && !isCollision(*box, UP)){ y -= pusher_velocity; by -= pusher_velocity;}
		if(KEY_PRESSED(KEY_DOWN) && !isCollision(*pusher, DOWN) && isPushing(*pusher, *box, DOWN) && !isCollision(*box, DOWN)){ y += pusher_velocity; by += pusher_velocity;}
		if(KEY_PRESSED(KEY_RIGHT) && !isCollision(*pusher, RIGHT) && isPushing(*pusher, *box, RIGHT) && !isCollision(*box, RIGHT)){ x += pusher_velocity; bx += pusher_velocity;}
		if(KEY_PRESSED(KEY_LEFT) && !isCollision(*pusher, LEFT) && isPushing(*pusher, *box, LEFT) && !isCollision(*box, LEFT)){ x -= pusher_velocity; bx -= pusher_velocity;}

		if(KEY_DOWN_NOW(KEY_UP) && !isCollision(*pusher, UP) && isPushing(*pusher, *box, UP) && !isCollision(*box, UP))if(counter > hold_time){ y -= pusher_velocity; by -= pusher_velocity;}else counter++;
		if(KEY_DOWN_NOW(KEY_DOWN) && !isCollision(*pusher, DOWN) && isPushing(*pusher, *box, DOWN) && !isCollision(*box, DOWN))if(counter > hold_time){ y += pusher_velocity; by += pusher_velocity;}else counter++;
		if(KEY_DOWN_NOW(KEY_RIGHT) && !isCollision(*pusher, RIGHT) && isPushing(*pusher, *box, RIGHT) && !isCollision(*box, RIGHT))if(counter > hold_time){ x += pusher_velocity; bx += pusher_velocity;}else counter++;
		if(KEY_DOWN_NOW(KEY_LEFT) && !isCollision(*pusher, LEFT) && isPushing(*pusher, *box, LEFT) && !isCollision(*box, LEFT))if(counter > hold_time){ x -= pusher_velocity; bx -= pusher_velocity;}else counter++;

		//reset counter

		if(KEY_RELEASED(KEY_UP) || KEY_RELEASED(KEY_DOWN) || KEY_RELEASED(KEY_RIGHT) || KEY_RELEASED(KEY_LEFT)) counter = 0;

		//update positions

		set_obj_pos(pusher, x, y);
		set_obj_pos(box, bx, by);

		//goal

		if(bx == gx && by == gy) return;

		//copy to oam

		oam_copy(oam_mem, obj_buffer, 2);
	}
}

int main()
{
    while(1){
		switch(menu()){
			case 1: game();
				break;
		}
	}

    return 0;
}


