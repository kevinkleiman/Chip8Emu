#pragma once

class CPU { 

	void(CPU::* Chip8Instr[16])() = {
		nullptr, &CPU::_0x1, &CPU::_0x2
	};

public:
	// Constructor
	CPU();

	// CPU hardware/registers
	uint16_t mI;
	uint16_t mPC = 0x200;
	uint16_t mOpcode;
	uint8_t mVRegisters[16];
	
	// Timers
	uint8_t mDelaytimer;	// Decrements at 60hz when greater than 0
	uint8_t mSoundTimer;	// Decrements at 60hz and plays tone when greater than 0

	void cycle();
	void fetch();
	void pollInput();
	void emulateInstr();

	// Instruction handlers
	void _0x0();
	void _0x1();
	void _0x2();
	void _0x3();
	void _0x4();
	void _0x5();
	void _0x6();
	void _0x7();
	void _0x8();
};
