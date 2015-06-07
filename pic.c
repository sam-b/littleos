#include "io.h"

#define PIC1_PORT_A 0x20
#define PIC2_PORT_A 0xA0

/* The PIC interrupts have been remapped */
#define PIC1_START_INTERRUPT 0x20
#define PIC2_START_INTERRUPT 0x28
#define PIC2_END_INTERRUPT   PIC2_START_INTERRUPT + 7

#define PIC_ACK     0x20

#define KBD_DATA_PORT   0x60

/** pic_acknowledge:
 *  Acknowledges an interrupt from either PIC 1 or PIC 2.
 *
 *  @param num The number of the interrupt
 */
void pic_acknowledge(unsigned int interrupt)
{
	if (interrupt < PIC1_START_INTERRUPT || interrupt > PIC2_END_INTERRUPT) {
		return;
	}

	if (interrupt < PIC2_START_INTERRUPT) {
		outb(PIC1_PORT_A, PIC_ACK);
	} else {
		outb(PIC2_PORT_A, PIC_ACK);
	}
}

/** read_scan_code:
*  Reads a scan code from the keyboard
*
*  @return The scan code (NOT an ASCII character!)
*/
unsigned char read_scan_code(void)
{
	return inb(KBD_DATA_PORT);
}