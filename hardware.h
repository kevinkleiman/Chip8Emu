#pragma once
#include <iostream>

namespace hardware {

	// Initializes all hardware
	void init();


	class CPU { 

		void(CPU::*Chip8Instr[16])();

		static void fetch();

		// Opcode functions
	public:
		// Constructor
		CPU();

		void cycle();
		void pollInput();
		void emulateInstr();

		// Instruction handlers
		void _0x1();
		void _0x2();
	};

	// Abstraction layer for RAM functions

	class LCD;
}
