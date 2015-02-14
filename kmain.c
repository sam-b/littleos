#include "output.h"
#include "lgdt.h"

int kmain(){
	configure_serial();
	char *output = "Sup Terri derp derp derp";
	print(output,24);
	char *debug = "Finished - now looping forever :(";
	puts(DEBUG, debug,31);
	return 0;
}