
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

	cpu->mInstr.raw = (memory->mRam[cpu->mPC] << 8) | memory->mRam[cpu->mPC + 1];
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

		break;
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

	// Decode and execute
	std::invoke(cpu->Chip8Instr[0x0], cpu);

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
	PRINT_HEX(mInstr.raw);
	PRINT_HEX(mInstr.y);
}

void CPU::_0x1()
{
	NO_IMPL(this->mOpcode);
}

void CPU::_0x2()
{
	NO_IMPL(this->mOpcode);
}

void CPU::_0x3()
{
	NO_IMPL(this->mOpcode);
}

void CPU::_0x4()
{
	NO_IMPL(this->mOpcode);
}

void CPU::_0x5()
{
	NO_IMPL(this->mOpcode);
}

void CPU::_0x6()
{
	NO_IMPL(this->mOpcode);
}

void CPU::_0x7()
{
	NO_IMPL(this->mOpcode);
}

void CPU::_0x8()
{
	NO_IMPL(this->mOpcode);
}

void CPU::_0x9()
{
	NO_IMPL(this->mOpcode);
}

void CPU::_0xA()
{
	NO_IMPL(this->mOpcode);
}

void CPU::_0xB()
{
	NO_IMPL(this->mOpcode);
}

void CPU::_0xC()
{
	NO_IMPL(this->mOpcode);
}

void CPU::_0xD()
{
	NO_IMPL(this->mOpcode);
}

void CPU::_0xE()
{
	NO_IMPL(this->mOpcode);
}

void CPU::_0xF()
{
	NO_IMPL(this->mOpcode);
}
