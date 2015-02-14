#include "output.h"
#include "lgdt.h"

struct idt_entry{
	unsigned int offset_high;
	unsigned char stuff;
	unsigned char more_stuff;
	unsigned int segment_selector;
	unsigned int offset_low;
} __attribute__((packed));

int kmain(){
	configure_serial();
	char *output = "Sup Terri derp derp derp";
	print(output,24);
	char *debug = "Finished - now looping forever :(";
	puts(DEBUG, debug,31);
	return 0;
}