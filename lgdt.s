global lgdt             ; make the label outb visible outside this file

; outb - send a byte to an I/O port
; stack: [esp + 4] gdt struct
;        [esp    ] return address
lgdt:
    mov eax, [esp + 4]   ; move the data to be sent into the al register
    lgdt [eax]           ; send the data to the I/O port
    ret                  ; return to the calling function