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

}


#endif	// SM64DS_NDSCORE_INCLUDED