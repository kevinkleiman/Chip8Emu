#pragma once

class Memory {
public:
	// Constructor
	Memory() {}

	uint16_t mStack[17];	// 16 levels of execution
	uint8_t mRam[4096];		// 4K of memory

	void LoadFont();
	void LoadRom();
};
