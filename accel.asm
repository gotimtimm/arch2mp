default rel

section .data
    align 8
    CST_1000:       dq 1000.0
    CST_3600:       dq 3600.0

section .text
    global calc_accel

calc_accel:
    test    r8d, r8d
    jle     .end_loop

    mov     r9, rcx
    mov     r10, rdx
    mov     ecx, r8d

    movsd   xmm3, [rel CST_1000]
    movsd   xmm4, [rel CST_3600]

.calc_loop:
    movsd   xmm0, [r9]
    movsd   xmm1, [r9 + 8]
    movsd   xmm2, [r9 + 16]

    subsd   xmm1, xmm0

    mulsd   xmm1, xmm3
    divsd   xmm1, xmm4

    divsd   xmm1, xmm2

    cvtsd2si eax, xmm1

    mov     [r10], eax

    add     r9, 24
    add     r10, 4

    dec     ecx
    jnz     .calc_loop

.end_loop:
    ret
