global lgdt             				; make the label lgdt visible outside this file
global load_segment_selector_registers

; outb - send a byte to an I/O port
; stack: [esp + 4] gdt struct
;        [esp    ] return address
lgdt:
    mov eax, [esp + 4]   ; move the data to be sent into the al register
    lgdt [eax]           ; send the data to the I/O port
    ret                  ; return to the calling function
load_segment_selector_registers:
	;mov ds, 0x10
    ;mov ss, 0x10
    ;mov es, 0x10
    ret