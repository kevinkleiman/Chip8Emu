#pragma once

class CPU { 

	void(CPU::* Chip8Instr[16])() = {
		nullptr, &CPU::_0x1, &CPU::_0x2
	};

public:
	// Constructor
	CPU();

	void cycle();
	void fetch();
	void pollInput();
	void emulateInstr();

	// Instruction handlers
	void _0x1();
	void _0x2();
	void _0x3();
	void _0x4();
	void _0x5();
	void _0x6();
	void _0x7();
	void _0x8();
};
