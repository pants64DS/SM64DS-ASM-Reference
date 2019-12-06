#include "SM64DS_2.h"
#include "NDSCore.h"

#define EN_INVERSION 0
#define EN_EXCHAR 0


#ifdef EN_EXCHAR

static SharedFilePtr waluigiBMDFile;
ModelAnim2* waluigiModel;



void hook_020E5D8C() {
 
	waluigiBMDFile.Construct(0x60F);
	char* modelData = Model::LoadFile(waluigiBMDFile);
	waluigiModel = new ModelAnim2;
	waluigiModel->SetFile(modelData, 1, 1);

	asm("add r9, r10, #0xDC\n"
		"str %0, [r9]\n" : : "r" (waluigiModel));

}


#endif





#ifdef EN_INVERSION

bool inversionFunc = false;

void hook_020e50ac() {
	inversionFunc = NumStars() % 2;
}


//3D inverse
void nsub_02056928() {
	asm("sub	r13, r13, #0x0C\n"			//Preserve the stack
		"ldr	r4, =0x0209CAA0\n"			//Use the character control flag from the savefile to determine when a valid save is loaded
		"ldrb	r4, [r4, #8]\n"
		"ands	r4, #0x80\n"
		"beq	0x0205692C\n"				//if not, branch back
		"movs	r4, %0\n"
		"beq	0x0205692C\n"
		"mov	r4, #0\n"
		"loop_02056928:\n"					//loop over all values (r0=palette ptr, r2=count)
		"ldr	r5, [r0, r4]\n"
		"neg	r5, r5\n"					//neg is a pseudo-instruction, assembles to rsbs r5, r5, #0
		"str	r5, [r0, r4]\n"
		"add	r4, r4, #4\n"
		"cmp	r4, r2\n"
		"bcc	loop_02056928\n"
		"end_02056928:\n"					//branch back
		"b		0x0205692C\n" : : "r" (inversionFunc));
}



//Minimap inverse
void nsub_02055F20() {
	asm("sub	r13, r13, #0xC\n"
		"ldr	r3, =0x0209CAA0\n"				//We are in the right area
		"ldrb	r3, [r3, #8]\n"
		"ands	r3, #0x80\n"
		"beq	0x02055F24\n"
		"movs	r4, %0\n"
		"beq	0x02055F24\n"
		"mov	r4, #0\n"
		"loop_02055F20:\n"
		"ldr	r5, [r0, r4]\n"
		"neg	r5, r5\n"					//neg is a pseudo-instruction, assembles to rsbs r5, r5, #0
		"str	r5, [r0, r4]\n"
		"add	r4, r4, #4\n"
		"cmp	r4, r2\n"
		"bcc	loop_02055F20\n"
		"b		0x02055F24\n" : : "r" (inversionFunc));
}
#endif
