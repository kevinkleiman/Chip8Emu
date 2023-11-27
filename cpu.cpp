
#include <iostream>
#include <stdint.h>
#include "cpu.h"
#include "display.h"
#include "context.h"
#include "common.h"


CPU::CPU()
{
}

void CPU::fetch()
{
	auto emuCtx = ctx->mEmuContext;

	emuCtx->mOpcode = (emuCtx->mRam[emuCtx->mPC] << 8) | emuCtx->mRam[emuCtx->mPC + 1];
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
	auto emuCtx = ctx->mEmuContext;

	// Fetch
	fetch();

	PRINT_HEX(emuCtx->mOpcode);
	
	// Decode and execute
	std::invoke(emuCtx->mCpu->Chip8Instr[0x1], emuCtx->mCpu);

	// Increment PC by two for 2 byte fetch
	emuCtx->mPC += 2;
}

void CPU::_0x1()
{
	DEBUG_LOG("0x1");
}

void CPU::_0x2()
{
	DEBUG_LOG("0x2");
}

void CPU::_0x3()
{
	DEBUG_LOG("0x2");
}

void CPU::_0x4()
{
	DEBUG_LOG("0x2");
}

void CPU::_0x5()
{
	DEBUG_LOG("0x2");
}

void CPU::_0x6()
{
	DEBUG_LOG("0x2");
}

void CPU::_0x7()
{
	DEBUG_LOG("0x2");
}

void CPU::_0x8()
{
	DEBUG_LOG("0x2");
}
