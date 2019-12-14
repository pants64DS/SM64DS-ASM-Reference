#ifndef SM64DS_NDSCORE_INCLUDED
#define SM64DS_NDSCORE_INCLUDED



namespace CP15 {

	void EnableDTCM();													//Enables the DTCM
	unsigned GetDTCMBaseAddress();										//Returns the DTCM base address
	void EnableMPU();													//Enables the MPU
	void DisableMPU();													//Disables the MPU
	void MPUDataRegion1(unsigned settings);								//Sets data/unified region 1 MPU settings
	void MPUDataRegion7(unsigned settings);								//Sets data/unified region 7 MPU settings
	unsigned MPUGetDataRegion7();										//Returns the MPU's region 7 settings
	void CleanDataCache();												//Cleans the data cache
	void CleanAndInvalidateDataCache();									//Cleans and invalidates the data cache
	void CleanDataCache(unsigned startVA, unsigned size);				//Cleans the data cache (virtual address)
	void InvalidateDataCache(unsigned startVA, unsigned size);			//Invalidates the data cache (virtual address)
	void InvalidateInstructionCache(unsigned startVA, unsigned size);	//Invalidates the instruction cache (virtual address)
	void CleanAndInvalidateDataCache(unsigned startVA, unsigned size);	//CLeans and invalidates the data cache (virtual address)
	void DrainWriteBuffer();											//Finish all bus transactions and end writes
	void WaitForInterrupt();											//Wait for interrupt
	void SystemSetup();													//Called right after __nds_start, sets up cache, MPU and memory

}


namespace IRQ {

	unsigned Disable();							//Disables interrupts and returns the previous state
	unsigned Enable();							//Enables interrupts and returns the previous state
	unsigned Restore(unsigned prevCPSR);		//Restores interrupts to the state given by prevCPSR and returns the previous state
	unsigned DisableAll();						//Disables IRQs and FIQs
	unsigned RestoreAll(unsigned prevCPSR);		//Restores IRQ and FIQ bits to the state given by prevCPSR
	bool SetIRQState(bool enable);				//Sets the IRQ state and returns wheter interrupts have been enabled before
	bool SetFIQState(bool enable);				//Sets the FIQ state and returns wheter fast interrupts have been enabled before
	unsigned ClearInterrupts(unsigned mask);	//Acknowledges masked IRQ flags and returns the previous IRQ flags
	unsigned DisableIRQs(unsigned mask);		//Disables interrupts according to mask and returns the previous IE state
	unsigned EnableIRQs(unsigned mask);			//Enables interrupts according to mask and returns the previous IE state

}


struct ARMMathState {							//Struct storing math processor state related data
	uint64_t divNumerator;
	uint64_t divDenominator;
	uint64_t sqrtParam;
	uint16_t divMode;
	uint16_t sqrtMode;
};



extern "C" {

	unsigned ARMOperatingMode();				//Returns the current ARM mode bits

	void OSReadROMArea(unsigned src, unsigned dest, unsigned size);		//Reads size bytes from the gamecard at src and writes to dest = 0x01FFDBD8
	//ROMCTRL_SETTINGS = 0x027FFE60

	void ARMMathSaveState(ARMMathState* location);		//Saves the ARM math state = 0x0205E988
	void ARMMathLoadState(ARMMathState* location);		//Loads the ARM math state = 0x0205E91C

	void DMAStartTransferFB(char channel, unsigned src, unsigned dest, unsigned ctrl);	//Starts DMA Transfer and waits two load cycles
	void DMAStartTransfer(char channel, unsigned src, unsigned dest, unsigned ctrl);	//Starts DMA Transfer

	void DMASyncWordTransfer(char channel, unsigned src, unsigned dest, unsigned len);	//Synchronous 32-bit DMA transfer (waits until completion)
	void DMASyncHalfTransfer(char channel, unsigned src, unsigned dest, unsigned len);	//Synchronous 16-bit DMA transfer (waits until completion)
	void DMASyncFillTransfer(char channel, unsigned dest, unsigned fill, unsigned len);	//Synchronous DMA fill transfer (fills dest with fill len bytes)


	int div(int numerator, int denominator);			//32 bit signed division = 0x02052f4c
	int mod(int numerator, int denominator);			//32 bit signed mod = 0x02052ef4
	Fix12i fdiv(Fix12i numerator, Fix12i denominator);	//Fix12i division (32 bit)
	int64_t ldiv(Fix12i numerator, Fix12i denominator); //Fix12i division (64 bit)
	unsigned sqrt(uint64_t x);							//64 bit unsigned sqrt
	Fix12i sqrtQ24(Fix12i lower, Fix12i upper) { return Fix12i(sqrt((static_cast<uint64_t>(upper.val) << 32) | lower.val), true); }

	int strcmp(const char* str1, const char* str2);		//Compares two strings and returns the difference (0 when equal)
	char* strncpy(char* dest, const char* src, unsigned count);	//Copies n bytes from src to dest and returns a pointer to dest
	char* strchr(const char* str, char c);				//Searches for c in str and returns a pointer to the first occurence, or 0 if c could not be found
	unsigned strlen(const char* str);					//Returns the length of the string or -1 if no null-terminator has been found

	void fdiv_async(Fix12i numerator, Fix12i denominator);
	Fix12i fdiv_result();								//Returns the division result
	int64_t ldiv_result();								//Returns the 64 bit division result (which type?)
	void reciprocal_async(Fix12i x);					//Computes 1/x

	[[noreturn]] void __nds_start();					//ROM entry point, resets the NDS on return = 0x02004800
	void __builtin_trap();								//Abort functionality. Triggers an undefined instruction (UDF)
	void __assert(const char* file, const char* line, const char* exp, int eval);	//Assertion that causes hangup if eval != 0

	unsigned sine_table;								//u16 per value, 4096 values, 2 bytes stride
	unsigned cosine_table;								//sine_table + 2
	unsigned atan_table;								//u16 per value, 1024 values

	int atan2(Fix12i y, Fix12i x);						//atan2 function, what about 0x020538b8?

	int abs(int x);										//Returns the absolute value of x


}


#endif	// SM64DS_NDSCORE_INCLUDED