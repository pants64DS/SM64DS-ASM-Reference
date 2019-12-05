#ifndef SM64DS_NDSCORE_INCLUDED
#define SM64DS_NDSCORE_INCLUDED


extern "C" {

	bool IRQDisable();							//Disables interrupts and returns the previous state
	bool IRQEnable();							//Enables interrupts and returns the previous state
	bool IRQRestore(unsigned prevCPSR);			//Restores interrupts to the state given by prevCPSR and returns the previous state

	void DMAStartTransferFB(char channel, unsigned src, unsigned dest, unsigned ctrl);	//Starts DMA Transfer and waits two load cycles
	void DMAStartTransfer(char channel, unsigned src, unsigned dest, unsigned ctrl);	//Starts DMA Transfer

	void DMASyncWordTransfer(char channel, unsigned src, unsigned dest, unsigned len);	//Synchronous 32-bit DMA transfer (waits until completion)
	void DMASyncHalfTransfer(char channel, unsigned src, unsigned dest, unsigned len);	//Synchronous 16-bit DMA transfer (waits until completion)
	void DMASyncFillTransfer(char channel, unsigned dest, unsigned fill, unsigned len);	//Synchronous DMA fill transfer (fills dest with fill len bytes)

	int div(int numerator, int denominator);			//32 bit division = 0x02052f4c
	int mod(int numerator, int denominator);			//32 bit mod = 0x02052ef4
	Fix12i fdiv(Fix12i numerator, Fix12i denominator);	//Fix12i division with increased precision
	Fix12i sqrtQ24(Fix12i lower, Fix12i upper);			//64 bit sqrt with Q24 resolution on input

	int strcmp(const char* str1, const char* str2);		//Compares two strings and returns the difference (0 when equal)
	char* strncpy(char* dest, const char* src, unsigned count);	//Copies n bytes from src to dest and returns a pointer to dest
	char* strchr(const char* str, char c);				//Searches for c in str and returns a pointer to the first occurence, or 0 if c could not be found

	void fdiv_execute(Fix12i numerator, Fix12i denominator);
	Fix12i fdiv_query();

}


#endif	// SM64DS_NDSCORE_INCLUDED