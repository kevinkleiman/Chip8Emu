
#include <iostream>
#include <stdint.h>
#include "../include/cpu.h"
#include "../include/display.h"
#include "../include/context.h"
#include "../include/common.h"


CPU::CPU()
{
}

void CPU::fetch()
{
	// Using two variables for readability
	auto cpu = ctx->mEmuContext->mCpu;
	auto memory = ctx->mEmuContext->mMemory;

	cpu->mOpcode = (memory->mRam[cpu->mPC] << 8) | memory->mRam[cpu->mPC + 1];
}

void CPU::cycle()
{
	auto emuCtx = ctx->mEmuContext;

	while (emuCtx->mStatus != context::QUIT)
	{
		emuCtx->mCpu->pollInput();

		if (emuCtx->mStatus == context::PAUSED) continue;

		emulateInstr();

		// Updates SDL display, not Chip8 display render
		display::updateDisplay();
	}
}

void CPU::pollInput()
{
	auto emuCtx = ctx->mEmuContext;

	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type) {
			case SDL_QUIT:
				emuCtx->mStatus = context::QUIT;

				return;
			
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
						emuCtx->mStatus = context::QUIT;
					case SDLK_SPACE:
						if (emuCtx->mStatus == context::RUNNING)
						{
							emuCtx->mStatus = context::PAUSED;
							DEBUG_LOG("Emulator Paused.");
						}
						else
						{
							emuCtx->mStatus = context::RUNNING;
							DEBUG_LOG("Emulator Resuming.");
						}

						return;
					default:
						break;
				}
				break;

		case SDL_KEYUP:
			break;
		}
	}
}

void CPU::emulateInstr()
{
	auto cpu = ctx->mEmuContext->mCpu;

	// Fetch
	fetch();

	PRINT_HEX(cpu->mOpcode);
	
	// Decode and execute
	std::invoke(cpu->Chip8Instr[0x1], cpu);

	// Increment PC by two for 2 byte fetch
	cpu->mPC += 2;
}

/*
	Opcode functions implementations referenced in 
	opcode function pointer table

	These functions act as handlers for all opcodes 0-F
*/

void CPU::_0x0()
{

}

void CPU::_0x1()
{
	NO_IMPL("0x1");
}

void CPU::_0x2()
{
	NO_IMPL("0x2");
}

void CPU::_0x3()
{
	NO_IMPL("0x2");
}

void CPU::_0x4()
{
	NO_IMPL("0x2");
}

void CPU::_0x5()
{
	NO_IMPL("0x2");
}

void CPU::_0x6()
{
	NO_IMPL("0x2");
}

void CPU::_0x7()
{
	NO_IMPL("0x2");
}

void CPU::_0x8()
{
	NO_IMPL("0x2");
}
