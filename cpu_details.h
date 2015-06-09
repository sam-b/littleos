#ifndef INCLUDE_CPU_DETAILS_H
#define INCLUDE_CPU_DETAILS_H

#define INTEL_MAGIC_CODE 0x756e6547
#define AMD_MAGIC_CODE 0x68747541

unsigned long detect_cpu(void);
void intel_list_features(void);
void amd_list_features(void);

#endif