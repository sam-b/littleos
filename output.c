#include "output.h"
#include "io.h"
#include "string.h"

/* The Output color values */
#define FB_BLACK				0
#define FB_BLUE 				1
#define FB_GREEN     			2
#define FB_CYAN 				3
#define FB_RED 					4
#define FB_MAGENTA 				5
#define FB_BROWN 				6
#define FB_LIGHT_GREY 			7
#define FB_DARK_GREY 			8
#define FB_LIGHT_BLUE 			9
#define FB_LIGHT_GREEN 			10
#define FB_LIGHT_CYAN 			11
#define FB_LIGHT_RED 			12
#define FB_LIGHT_MAGENTA		13
#define FB_LIGHT_BROWN			14
#define FB_WHITE 				15

/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

/* All the I/O ports are calculated relative to the data port. This is because
 * all serial ports (COM1, COM2, COM3, COM4) have their ports in the same
 * order, but they start at different values.
 */

#define SERIAL_COM1_BASE                0x3F8      /* COM1 base port */

#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

/* The I/O port commands */

/* SERIAL_LINE_ENABLE_DLAB:
 * Tells the serial port to expect first the highest 8 bits on the data port,
 * then the lowest 8 bits will follow
 */
#define SERIAL_LINE_ENABLE_DLAB         0x80

void fb_write_cell(unsigned int, char, unsigned char, unsigned char);
void fb_move_cursor(void);
int fb_write(char *,unsigned int);
int serial_write(char *);
int serial_is_transmit_fifo_empty(unsigned int);
void serial_configure_modem(unsigned short);
void serial_configure_baud_rate(unsigned short, unsigned short);
void serial_configure_line(unsigned short);
void serial_configure_buffer(unsigned short);
void serial_write_char(char);
unsigned short cursor_pos(void);
/** fb_write_cell:
 *  Writes a character with the given foreground and background to position i
 *  in the framebuffer.
 *
 *  @param i  The location in the framebuffer
 *  @param c  The character
 *  @param fg The foreground color
 *  @param bg The background color
 */

static unsigned short col = 0;
static unsigned short row = 0;

void print(char * buf){
	fb_write(buf,strlen(buf));
}

void puts(unsigned short target, char * buf){
	switch(target){
		case 0:
			print(buf);
			break;
		case 1:
			serial_write(buf);
			break;
		default:
			print(buf);
	}
}

void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
 	char *fb = (char *) 0x000B8000;
    fb[i] = c;
    fb[i + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}

int fb_write(char *buf, unsigned int len){
	unsigned int i;
    unsigned short pos = cursor_pos();
	for(i = 0; i< len; i++){
		fb_write_cell(pos+(2*i),buf[i],FB_BLACK,FB_GREEN);
	}
    if(col + (short) 2 * len > 80){
        row++;
        col = (col + (short) 2 * len) % 80;
    } else {
        col += (short) 2 * len;
    }
	fb_move_cursor();
	return len;
}

int serial_write(char *buf){
    unsigned int len = strlen(buf);
	unsigned int i;
	for(i = 0;i < len;i++){
        serial_write_char(buf[i]);
	}
	return len;
}

void serial_write_char(char a) {
   while (serial_is_transmit_fifo_empty(SERIAL_COM1_BASE) == 0);
 
   outb(SERIAL_COM1_BASE,a);
}

unsigned short cursor_pos(){
    return (row * 80) + col;
}

/** fb_move_cursor:
 *  Moves the cursor of the framebuffer to the given position
 *
 *  @param pos The new position of the cursor
 */
void fb_move_cursor()
{
    unsigned short pos = cursor_pos() / 2;
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT,    pos & 0x00FF);
}

void configure_serial(){
    serial_configure_baud_rate(SERIAL_COM1_BASE,0x03);
    serial_configure_line(SERIAL_COM1_BASE);
    serial_configure_buffer(SERIAL_COM1_BASE);
    serial_configure_modem(SERIAL_COM1_BASE);
}

/** serial_configure_baud_rate:
 *  Sets the speed of the data being sent. The default speed of a serial
 *  port is 115200 bits/s. The argument is a divisor of that number, hence
 *  the resulting speed becomes (115200 / divisor) bits/s.
 *
 *  @param com      The COM port to configure
 *  @param divisor  The divisor
 */
void serial_configure_baud_rate(unsigned short com, unsigned short divisor)
{
    outb(SERIAL_LINE_COMMAND_PORT(com),
         SERIAL_LINE_ENABLE_DLAB);
    outb(SERIAL_DATA_PORT(com),
         (divisor >> 8) & 0x00FF);
    outb(SERIAL_DATA_PORT(com),
         divisor & 0x00FF);
}

/** serial_configure_line:
 *  Configures the line of the given serial port. The port is set to have a
 *  data length of 8 bits, no parity bits, one stop bit and break control
 *  disabled.
 *
 *  @param com  The serial port to configure
 */
void serial_configure_line(unsigned short com)
{
    /* Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
     * Content: | d | b | prty  | s | dl  |
     * Value:   | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
     */
    outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

void serial_configure_buffer(unsigned short com)
{
    /* Bit:     
     * Content: 
     * Value:   
     */
    outb(SERIAL_FIFO_COMMAND_PORT(com), 0xC7);
}

void serial_configure_modem(unsigned short com)
{
    /* Bit:     
     * Content: 
     * Value:   
     */
    outb(SERIAL_MODEM_COMMAND_PORT(com), 0x03);
}

/** serial_is_transmit_fifo_empty:
 *  Checks whether the transmit FIFO queue is empty or not for the given COM
 *  port.
 *
 *  @param  com The COM port
 *  @return 0 if the transmit FIFO queue is not empty
 *          1 if the transmit FIFO queue is empty
 */
int serial_is_transmit_fifo_empty(unsigned int com)
{
    /* 0x20 = 0010 0000 */
    return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}