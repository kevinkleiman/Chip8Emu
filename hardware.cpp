#include <iostream>
#include <stdint.h>
#include "hardware.h"
#include "display.h"
#include "context.h"
#include "common.h"


namespace hardware {

	void init()
	{
		// Inititalize hardware and set up hardware state
		auto emuCtx = ctx->mEmuContext;

		emuCtx->mStatus = context::RUNNING;
		emuCtx->mPC = ROM_ENTRY;

		emuCtx->mCpu = std::make_shared<CPU>();
		emuCtx->mMemory = std::make_shared<Memory>();

		emuCtx->mMemory->loadFont();
		emuCtx->mMemory->loadRom();
		emuCtx->mCpu->cycle();
	}

	CPU::CPU()
	{
		// Setup opcode table
		*this->Chip8Instr = {
			&CPU::_0x1
		};

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
			// emuCtx->mCpu->pollInput();

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
		
		// Decode and execute
		std::invoke(emuCtx->mCpu->Chip8Instr[emuCtx->mOpcode], emuCtx->mCpu);
	}

	void CPU::_0x1()
	{
		DEBUG_LOG("0x1");
	}

	void CPU::_0x2()
	{
		DEBUG_LOG("0x2");
	}

	Memory::Memory()
	{
	}

	void Memory::loadFont()
	{
		auto emuCtx = ctx->mEmuContext;

		const uint8_t font[] = {
			0xF0, 0x90, 0x90, 0x90, 0xF0,   // 0   
			0x20, 0x60, 0x20, 0x20, 0x70,   // 1  
			0xF0, 0x10, 0xF0, 0x80, 0xF0,   // 2 
			0xF0, 0x10, 0xF0, 0x10, 0xF0,   // 3
			0x90, 0x90, 0xF0, 0x10, 0x10,   // 4    
			0xF0, 0x80, 0xF0, 0x10, 0xF0,   // 5
			0xF0, 0x80, 0xF0, 0x90, 0xF0,   // 6
			0xF0, 0x10, 0x20, 0x40, 0x40,   // 7
			0xF0, 0x90, 0xF0, 0x90, 0xF0,   // 8
			0xF0, 0x90, 0xF0, 0x10, 0xF0,   // 9
			0xF0, 0x90, 0xF0, 0x90, 0x90,   // A
			0xE0, 0x90, 0xE0, 0x90, 0xE0,   // B
			0xF0, 0x80, 0x80, 0x80, 0xF0,   // C
			0xE0, 0x90, 0x90, 0x90, 0xE0,   // D
			0xF0, 0x80, 0xF0, 0x80, 0xF0,   // E
			0xF0, 0x80, 0xF0, 0x80, 0x80,   // F
		};

		memcpy(&emuCtx->mRam, font, sizeof(font));
	}

	void Memory::loadRom()
	{
		auto emuCtx = ctx->mEmuContext;

		FILE* pRom;
		FILE** pRomFile = &pRom;

		size_t romSize;
		size_t maxRomSize = sizeof(emuCtx->mRam) - ROM_ENTRY;

		fopen_s(pRomFile, emuCtx->mRomFileName, "rb");

		if (!pRom)
		{
			DEBUG_LOG("Failed to load ROM.");
			exit(1);
		}

		fseek(pRom, 0, SEEK_END);
		romSize = ftell(pRom);
		rewind(pRom);


		if (romSize > maxRomSize)
		{
			DEBUG_LOG("ROM size is too large.");
			exit(1);
		}

		if (!fread(&emuCtx->mRam[ROM_ENTRY], romSize, 1, pRom))
		{
			DEBUG_LOG("Could not read ROM file into memory");
			exit(1);
		}

		fclose(pRom);
	}
}
