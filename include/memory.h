#pragma once

class Memory {
public:
	// Constructor
	Memory();

	uint16_t mStack[16];	// 16 levels of execution
	uint16_t mRam[4096];	// 4K of memory

	void loadFont();
	void loadRom();
};
