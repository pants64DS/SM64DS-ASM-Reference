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

.global Div64
Div64:
	push    {r4}
	ldr     r4, =#0x04000280
	mov     r12, #2
	strh    r12,[r4]
	str     r0,[r4, #0x10]
	str     r1,[r4, #0x14]
	str     r2,[r4, #0x18]
	str     r3,[r4, #0x1c]
wait1:
	ldrh    r0,[r4]
	ands    r0, r0, #0x8000
	bne     wait1
	ldr     r0,[r4, #0x20]
	ldr     r1,[r4, #0x24]
	pop     {r4}
	bx      r14

.global _Z4sqrt6Fix24i
_Z4sqrt6Fix24i:
	push   {r4, r14}
	ldr     r4,=#0x04000208
	ldrh    r14,[r4]
	mov     r1, #0
	ldr     r12,=#0x040002B0
	strh    r1,[r4]
	mov     r3, #1
	ldr     r1,=#0x040002B8
	strh    r3,[r12]
	mov     r3, r0, lsl #26
	str     r3,[r1]
	mov     r2, r0, asr #6
	str     r2,[r1, #4]
	ldrh    r0,[r4]
	strh    r14,[r4]
wait2:
	ldrh    r0,[r12]
	ands    r0, r0, #0x8000
	bne     wait2
	ldr     r0,=#0x040002B4
	ldr     r0,[r0]
	add     r0, r0, #1
	mov     r0, r0, asr #1
	pop    {r4,r14}
	bx      r14