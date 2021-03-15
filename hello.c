
/*
A simple "hello world" example.
Set the screen background color and palette colors.
Then write a message to the nametable.
Finally, turn on the PPU to display video.
*/

#include "neslib.h"
#include "vrambuf.h"
#include "bcd.h"

// link the pattern table into CHR ROM
//#link "chr_generic.s"
//#link "vrambuf.c"

#define CHARACTER 0xD8
#define ATTR 0x0
#define FLIPPED 0X40

int x = 8;
int dir = 0;

// define a 2x2 metasprite
const unsigned char metasprite[]={
  	0,	0,	CHARACTER,	ATTR,
  	0,	8,	CHARACTER+1,	ATTR,
  	8,	0,	CHARACTER+2,	ATTR,
  	8,	8,	CHARACTER+3,	ATTR,
  	128};
const unsigned char metasprite_flipped[]={
  	0,	0,	CHARACTER+2,	FLIPPED,
  	0,	8,	CHARACTER+3,	FLIPPED,
  	8,	0,	CHARACTER,	FLIPPED,
  	8,	8,	CHARACTER+1,	FLIPPED,
  	128};

// main function, run after console reset
void main(void) {
  char oam_id;

  // set palette colors
  pal_col(0,0x03);	// set screen to dark blue
  pal_col(1,0x14);	// fuchsia
  pal_col(2,0x0);	// grey
  pal_col(3,0x34);	// white
  
  pal_col(17, 0x18);
  pal_col(18, 0x36);
  pal_col(19, 0x24);

  // write text to name table
  vram_adr(NTADR_A(1,1));		// set address
  vram_write("This is", 7);	// write bytes to video RAM
  vram_adr(NTADR_A(1,2));
  vram_write("Dion Cullum's", 13);
  vram_adr(NTADR_A(1,3));
  vram_write("first NES 'Game'!", 17);
  
  vram_adr(NTADR_A(29, 18));
  vram_write("\xC4\xC6", 2);
  vram_adr(NTADR_A(29, 19));
  vram_write("\xC5\xC7", 2);
  vram_adr(NTADR_A(1, 20));
  vram_write("\xF4\XF6\xF4\XF6\xF4\XF6\xF4\XF6\xF4\XF6\xF4\XF6\xF4\XF6\xF4\XF6\xF4\XF6\xF4\XF6\xF4\XF6\xF4\XF6\xF4\XF6\xF4\XF6\xF4\XF6",30);
  vram_adr(NTADR_A(1, 21));
  vram_write("\xF5\XF7\xF5\XF7\xF5\XF7\xF5\XF7\xF5\XF7\xF5\XF7\xF5\XF7\xF5\XF7\xF5\XF7\xF5\XF7\xF5\XF7\xF5\XF7\xF5\XF7\xF5\XF7\xF5\XF7",30);
  // enable PPU rendering (turn on screen)
  ppu_on_all();
  
  vrambuf_clear();
  set_vram_update(updbuf);

  // infinite loop
  while (1)
  {
    char cur_oam = 0;
    if (dir == 0)
    {
      x += 1;
    }
    else
    {
      x -= 1;
    }
    
    if (x >= 232)
    {
      if (dir != 1)
      {
        dir = 1;
      }
    }
    else if (x <= 8)
    {
      if (dir != 0)
      {
        dir = 0;
      }
    }
    oam_id = 0;
    if (dir == 0)
      oam_id = oam_meta_spr(x, 143, oam_id, metasprite);
    else if (dir == 1)
      oam_id = oam_meta_spr(x, 143, oam_id, metasprite_flipped);
    
    if (x >= 216)
    {
      vrambuf_put(NTADR_A(1, 4), "On the door!", 12);
    }
    else
    {
      vrambuf_put(NTADR_A(1, 4), "            ", 12);
    }
    // ppu_wait_frame();
    vrambuf_flush();
  }
}
