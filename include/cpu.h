#pragma once

class CPU { 

	/*

	This is a table (array) which holds pointers
	to functions that handle opcodes and their
	respective variations. It is faster than a 
	switch statement and allows for better
	readability.

	*/
	void(CPU::* Chip8Instr[16])() = {
		&CPU::_0x0, &CPU::_0x1, &CPU::_0x2,
		&CPU::_0x3, &CPU::_0x4, &CPU::_0x5,
		&CPU::_0x6, &CPU::_0x7, &CPU::_0x8,
		&CPU::_0x9, &CPU::_0xA, &CPU::_0xB,
		&CPU::_0xC, &CPU::_0xD, &CPU::_0xE,
		&CPU::_0xF
	};

public:
	// Constructor
	CPU();

	union Instr_t {
		struct {
			unsigned int opcode : 4;
			unsigned int x : 4;
			unsigned int y : 4;
			unsigned int n : 4;
		};
		struct {
			unsigned int opcode : 4;
			unsigned int nnn : 12;
		};
		struct {
			unsigned int opcode : 4;
			unsigned int x : 4;
			unsigned int kk : 8;
		};
		uint16_t raw;
	} mInstr;

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
	void _0x9();
	void _0xA();
	void _0xB();
	void _0xC();
	void _0xD();
	void _0xE();
	void _0xF();
};
