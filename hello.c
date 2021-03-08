
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

int x = 8;
int dir = 0;

// main function, run after console reset
void main(void) {

  // set palette colors
  pal_col(0,0x03);	// set screen to dark blue
  pal_col(1,0x14);	// fuchsia
  pal_col(2,0x0);	// grey
  pal_col(3,0x34);	// white
  
  pal_col(17, 0x19);
  pal_col(18, 0x03);
  pal_col(19, 0x1a);

  // write text to name table
  vram_adr(NTADR_A(1,1));		// set address
  vram_write("This is", 7);	// write bytes to video RAM
  vram_adr(NTADR_A(1,2));
  vram_write("Dion Cullum's", 13);
  vram_adr(NTADR_A(1,3));
  vram_write("first NES 'Game'!", 17);

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
    
    if (x >= 240)
    {
      dir = 1;
    }
    else if (x <= 8)
    {
      dir = 0;
    }
    
    oam_spr(x, 150, 0x19, 0x0, cur_oam);
    ppu_wait_frame();
    vrambuf_flush();
  }
}
