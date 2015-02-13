#include "io.h"

#define FB_BLACK			0
#define FB_BLUE 			1
#define FB_GREEN     		2
#define FB_CYAN 			3
#define FB_RED 				4
#define FB_MAGENTA 			5
#define FB_BROWN 			6
#define FB_LIGHT_GREY 		7
#define FB_DARK_GREY 		8
#define FB_LIGHT_BLUE 		9
#define FB_LIGHT_GREEN 		10
#define FB_LIGHT_CYAN 		11
#define FB_LIGHT_RED 		12
#define FB_LIGHT_MAGENTA	13
#define FB_LIGHT_BROWN		14
#define FB_WHITE 			15

/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

void fb_write_cell(unsigned int, char, unsigned char, unsigned char);
void fb_move_cursor(unsigned short);
int write(char *, unsigned int);

int kmain(){
	char *output = "Sup Terri derp derp derp";
	int cursor_pos = write(output,strlen(output));
	return 0;
}

/** fb_write_cell:
 *  Writes a character with the given foreground and background to position i
 *  in the framebuffer.
 *
 *  @param i  The location in the framebuffer
 *  @param c  The character
 *  @param fg The foreground color
 *  @param bg The background color
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
 	char *fb = (char *) 0x000B8000;
    fb[i] = c;
    fb[i + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}

int write(char *buf, unsigned int len){
	unsigned int i;
	for(i = 0; i< len; i++){
		fb_write_cell(2*i,buf[i],FB_BLACK,FB_GREEN);
	}
	fb_move_cursor((short) len);
	return len;
}

/** fb_move_cursor:
 *  Moves the cursor of the framebuffer to the given position
 *
 *  @param pos The new position of the cursor
 */
void fb_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT,    pos & 0x00FF);
}