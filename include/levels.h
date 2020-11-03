
//{{BLOCK(level1)

//======================================================================
//
//	level1, 256x256@4, 
//	+ palette 16 entries, not compressed
//	+ 4 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 32 + 128 + 2048 = 2208
//
//	Time-stamp: 2017-08-04, 23:05:36
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_LEVEL1_H
#define GRIT_LEVEL1_H

#define levelTilesLen 128
extern const unsigned short levelTiles[64];

#define levelMapLen 2048
#define levelcolMapLen 2048

extern const unsigned short level1Map[1024];
extern const unsigned short level1colMap[1024];

extern const unsigned short level2Map[1024];
extern const unsigned short level2colMap[1024];

#define levelPalLen 32
extern const unsigned short levelPal[16];

#endif // GRIT_LEVEL1_H

//}}BLOCK(level1)
