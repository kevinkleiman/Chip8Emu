#include <iostream>
#include <stdint.h>
#include "../include/cpu.h"
#include "../include/display.h"
#include "../include/context.h"
#include "../include/common.h"


void CPU::Fetch()
{
	// Using two variables for readability
	auto memory = ctx->mEmuContext->mMemory;

	mInstr.raw = (memory->mRam[mPC] << 8) | memory->mRam[mPC + 1];
}

void CPU::Cycle()
{
	auto emuCtx = ctx->mEmuContext;

	while (emuCtx->mStatus != context::QUIT)
	{
		PollInput();

		if (emuCtx->mStatus == context::PAUSED) continue;

		EmulateInstr();

		// Updates SDL display, not Chip8 display render
		display::updateDisplay();
	}
}

void CPU::PollInput()
{
	auto emuCtx = ctx->mEmuContext;

	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type) 
		{
			case SDL_QUIT:
			{
				emuCtx->mStatus = context::QUIT;
			
				return;
			}
			case SDL_KEYDOWN:
			{
				// Get keypad input
				switch (event.key.keysym.sym) 
				{
					case SDLK_ESCAPE:
					{
						emuCtx->mStatus = context::QUIT;
					}
					case SDLK_SPACE:
					{
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
					}
				default:
						break;
				}
				// End keypad input

				break;
			}
			case SDL_KEYUP:
			{
				break;
			}
		}
	}
}

void CPU::EmulateInstr()
{
	// Fetch
	Fetch();

	PRINT_HEX(mInstr.raw);
	// Decode and execute
	std::invoke(this->Chip8Instr[mInstr.opcode], this);
}

void CPU::_0x0()
{
	auto emuCtx = ctx->mEmuContext;

	switch (mInstr.n) 
	{
		case 0x0:
		{
			memset(&emuCtx->mLcd, 0, sizeof(emuCtx->mLcd));
			mPC += 2;
			break;
		}
		case 0xE:
		{
			mPC = emuCtx->mMemory->mStack[mSP];
			--mSP;
			mPC += 2;
			break;
		}
	}
}

void CPU::_0x1()
{
	mPC = mInstr.nnn;
}

void CPU::_0x2()
{
	auto memory = ctx->mEmuContext->mMemory;

	++mSP;
	memory->mStack[mSP] = mPC;

	mPC = mInstr.nnn;
}

void CPU::_0x3()
{
	if (mVRegisters[mInstr.x] == mInstr.kk)
	{
		mPC += 2;
	}
}

void CPU::_0x4()
{
	if (mVRegisters[mInstr.x] != mInstr.kk)
	{
		mPC += 2;
	}
}

void CPU::_0x5()
{
	if (mVRegisters[mInstr.x] != mVRegisters[mInstr.y])
	{
		mPC += 2;
	}
}

void CPU::_0x6()
{
	mVRegisters[mInstr.x] = mInstr.kk;
	mPC += 2;
}

void CPU::_0x7()
{
	mVRegisters[mInstr.x] += mInstr.kk;
	mPC += 2;
}

void CPU::_0x8()
{
	switch (mInstr.n)
	{
		case 0x0:
		{
			mVRegisters[mInstr.x] = mVRegisters[mInstr.y];
			mPC += 2;
			break;
		}
		case 0x1:
		{
			mVRegisters[mInstr.x] |= mVRegisters[mInstr.y];
			mPC += 2;
			break;
		}
		case 0x2:
		{
			mVRegisters[mInstr.x] &= mVRegisters[mInstr.y];
			mPC += 2;
			break;
		}
		case 0x3:
		{
			mVRegisters[mInstr.x] ^= mVRegisters[mInstr.y];
			mPC += 2;
			break;
		}
		case 0x4:
		{
			uint16_t sum = mVRegisters[mInstr.x] + mVRegisters[mInstr.y];

			if (sum > 255)
			{
				mVRegisters[0xF] = 0x1;
			}

			mVRegisters[mInstr.x] = sum & 0xFF;
			mPC += 2;
			break;
		}
		case 0x5:
		{
			uint8_t diff = mVRegisters[mInstr.x] - mVRegisters[mInstr.y];

			if (mVRegisters[mInstr.x] > mVRegisters[mInstr.y])
			{
				mVRegisters[0xF] = 0x1;
			}
			else
			{
				mVRegisters[0xF] = 0x0;
			}

			mVRegisters[mInstr.x] = diff;
			mPC += 2;
			break;
		}
		case 0x6:
		{
			if ((mVRegisters[mInstr.x] & 0b1) == 0x1)
			{
				mVRegisters[0xF] = 0x1;
			}
			else
			{
				mVRegisters[0xF] = 0x0;
			}

			mVRegisters[mInstr.x] = mVRegisters[mInstr.x] / 2;
			mPC += 2;
			break;
		}
		case 0x7:
		{
			uint8_t diff = mVRegisters[mInstr.y] - mVRegisters[mInstr.x];

			if (mVRegisters[mInstr.y] > mVRegisters[mInstr.x])
			{
				mVRegisters[0xF] = 0x1;
			}
			else
			{
				mVRegisters[0xF] = 0x0;
			}

			mVRegisters[mInstr.x] = diff;
			mPC += 2;
			break;
		}
		case 0xE:
		{
			if ((mVRegisters[mInstr.x] & 0x80) == 0x1)
			{
				mVRegisters[0xF] = 0x1;
			}
			else
			{
				mVRegisters[0xF] = 0x0;
			}

			mVRegisters[mInstr.x] *= 2;
			mPC += 2;
			break;
		}
	}
}

void CPU::_0x9()
{
	NO_IMPL(mInstr.raw);
}

void CPU::_0xA()
{
	NO_IMPL(this->mInstr.raw);
}

void CPU::_0xB()
{
	NO_IMPL(this->mInstr.raw);
}

void CPU::_0xC()
{
	NO_IMPL(this->mInstr.raw);
}

void CPU::_0xD()
{
	NO_IMPL(this->mInstr.raw);
}

void CPU::_0xE()
{
	NO_IMPL(this->mInstr.raw);
}

void CPU::_0xF()
{
	NO_IMPL(this->mInstr.raw);
}
