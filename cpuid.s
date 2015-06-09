global cpuid_support

;check if cpu supports cpuid instruction as this was only added with the 486
;returns a non-zero value if the ID bit can be set in EFLAGS
cpuid_support:
	pushfd                               ;Save EFLAGS
	pushfd                               ;Store EFLAGS
	xor dword [esp],0x00200000           ;Invert the ID bit in stored EFLAGS
	popfd                                ;Load stored EFLAGS (with ID bit inverted)
	pushfd                               ;Store EFLAGS again (ID bit may or may not be inverted)
	pop eax                              ;eax = modified EFLAGS (ID bit may or may not be inverted)
	xor eax,[esp]                        ;eax = whichever bits were changed
	popfd                                ;Restore original EFLAGS
	and eax,0x00200000                   ;eax = zero if ID bit can't be changed, else non-zero
	ret