.global Div64
.arm
Div64:
    push    {r4}
    ldr     r4, =#0x04000280
    mov     r12, #2
    strh    r12,[r4]
    str     r0,[r4, #0x10]
    str     r1,[r4, #0x14]
    str     r2,[r4, #0x18]
    str     r3,[r4, #0x1c]
wait:
    ldrh    r0,[r4]
    ands    r0, r0, #0x8000
    bne     wait
    ldr     r0,[r4, #0x20]
    ldr     r1,[r4, #0x24]
    pop     {r4}
    bx      r14

.global DivQ24
DivQ24:
    ldr     r2, =#0x04000280
    mov     r12, #1
    strh    r12,[r2]
    mov     r12, #0
    str     r12,[r2, #0x10]
    str     r0, [r2, #0x14]
    str     r1, [r2, #0x18]
    str     r12,[r2, #0x1c]
wait0:
    ldrh    r0,[r2]
    ands    r0, r0, #0x8000
    bne     wait0
    add     r1, r2, #0x20
    mov     r0, #0x80
    ldr     r2,[r1]
    ldr     r1,[r1, #4]
    adds    r0, r2, r0
    adc     r1, r1, #0
    mov     r0, r0, lsr #8
    orr     r0, r0, r1, lsl #24
    bx      r14