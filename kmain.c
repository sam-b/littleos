#include "output.h"
#include "lgdt.h"
#include "pic.h"
#include "cpu_details.h"
#include "cpuid.h"

struct idt_entry{
	unsigned int offset_high;
	unsigned char stuff;
	unsigned char more_stuff;
	unsigned int segment_selector;
	unsigned int offset_low;
} __attribute__((packed));

int kmain(){
	configure_serial();
	if(cpuid_support()){
		print("CPUID is supported! ");
		unsigned long cpu = detect_cpu();
		if(cpu == INTEL_MAGIC_CODE){
			print("Intel CPU!");
			intel_list_features();
		} else if (cpu == AMD_MAGIC_CODE) {
			print("AMD CPU!");
			amd_list_features();
		} else {
			print("CPU make is not supported");
		}
	} else {
		print("CPUID is not supported, get a new CPU peasent.");
	}
	char *debug = "Finished - now looping forever :(";
	puts(DEBUG, debug);
	return 0;
}