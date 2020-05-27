.global _ZN7ostream5flushEv
.global _ZN7ostream5flushEy
.global _ZN7ostream5flushEmmm
.global _ZN7ostream5flushEmmmm
.global _ZN7ostream6bufferE

_ZN7ostream5flushEv:
_ZN7ostream5flushEy:
_ZN7ostream5flushEmmm:
_ZN7ostream5flushEmmmm:
	mov   r12,r12
	b     skip
	.word 0x00006464
_ZN7ostream6bufferE:
	.fill 120
	.word 0
skip:
	bx    lr