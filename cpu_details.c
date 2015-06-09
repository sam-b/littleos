#include "cpu_details.h"
#include "output.h"

#define cpuid(in, a, b, c, d) __asm__("cpuid": "=a" (a), "=b" (b), "=c" (c), "=d" (d) : "a" (in));

unsigned short intel_cpu_type(void);
unsigned short intel_cpu_family(void); 
unsigned short intel_cpu_model(void); 
unsigned short intel_cpu_stepping(void); 

unsigned long detect_cpu(){
	unsigned long ebx, unused;
	cpuid(0,unused,ebx,unused,unused);
	return ebx;
}

void intel_list_features(){
	print("Listing CPU features!");
	print("Type: ");
	unsigned short type = intel_cpu_type();
	switch(type){
		case 0:
			print("Original OEM");
			break;
		case 1:
			print("Overdrive");
			break;
		case 2:
			print("Dual-capable");
			break;
		case 3:
			print("Reserved");
			break;
		default:
			print("Unknown CPU type!");
	}
	print("Family: ");
	unsigned short family = intel_cpu_family();
	switch(family){
		case 3:
			print("i386");
			break;
		case 4:
			print("i486");
			break;
		case 5:
			print("Pentium");
			break;
		case 6:
			print("Pentium Pro");
			break;
		case 15:
			print("Pentium 4");
			break;
		default:
			print("Unrecognised CPU Family!");
	}
	print("Model: ");
	unsigned short model = intel_cpu_model();
	switch(family){
		case 3:
			break;
		case 4:
			switch(model){
				case 0: //intentional passthrough
				case 1:
					print("DX");
					break;
				case 2:
					print("SX");
					break;
				case 3:
					print("487/DX2");
					break;
				case 4:
					print("SL");
					break;
				case 5:
					print("SX2");
					break;
				case 7:
					print("Write-back enhanced DX2");
					break;
				case 8:
					print("DX4");
					break;
				default:
					print("Unrecognised model!");
			}
			break;
		case 5:
			switch(model) {
				case 1:
					print("60/66");
					break;
				case 2:
					print("75-200");
					break;
				case 3:
					print("for 486 system");
					break;
				case 4:
					print("MMX");
					break;
				default:
					print("Unrecognised model!");
			}
			break;
		case 6:
			switch(model) {
				case 1:
					print("Pentium Pro");
					break;
				case 3:
					print("Pentium II Model 3");
					break;
				case 5:
					print("Pentium II Model 5/Xeon/Celeron");
					break;
				case 6:
					print("Celeron");
					break;
				case 7:
					print("Pentium III/Pentium III Xeon - external L2 cache");
					break;
				case 8:
					print("Pentium III/Pentium III Xeon - internal L2 cache");
					break;
			}
			break;
		case 15:
			break;
	}
	unsigned short stepping = intel_cpu_stepping();
	if(stepping){
		print("Stepping read...can't print numbers yet :( ");
	}
	return;
}

void amd_list_features(){
	print("AMD Feature listing not yet implemented!");
	return;
}

unsigned short intel_cpu_type(){
	unsigned long eax, unused;
	unsigned short type;
	cpuid(1,eax,unused,unused, unused);
	type = (eax >> 12) & 0x3;
	return type;
}

unsigned short intel_cpu_family(){
	unsigned long eax, unused;
	unsigned short family;
	cpuid(1,eax,unused,unused, unused);
	family = (eax >> 8) & 0xf;
	return family;
}

unsigned short intel_cpu_model(){
	unsigned long eax, unused;
	unsigned short model;
	cpuid(1,eax,unused,unused, unused);
	model = (eax >> 4) & 0xf;
	return model;
}

unsigned short intel_cpu_stepping(){
	unsigned long eax, unused;
	unsigned short stepping;
	cpuid(1,eax,unused,unused, unused);
	stepping = eax & 0xf;
	return stepping;
}