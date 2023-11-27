#pragma once

#include <iostream>
#include <stdint.h>
#include <SDL.h>
#include "cpu.h"
#include "memory.h"

namespace context {

	typedef enum {
		QUIT,
		RUNNING,
		PAUSED
	} EmuStatus_t;

	
	// Singleton for managing display state
	typedef struct DisplayContext_t {
		uint32_t mWindowWidth = 920;
		uint32_t mWindowHeight = 460;

		uint32_t mForegroundColor = 0xFFFFFFFF;	// WHITE RGBA888
		uint32_t mBackgroundColor = 0x000000FF; // BLACK RGBA888

		SDL_Window* mWindow;
		SDL_Renderer* mRenderer;
	} DisplayContext_t;

	// Singleton for managing emulator state
	typedef struct EmuContext_t {
		EmuStatus_t mStatus;

		std::string mRomName;

		std::shared_ptr<CPU> mCpu;
		std::shared_ptr<Memory> mMemory;

		std::shared_ptr<bool> mLcd;


		const char* mRomFileName;
		bool mKeypad[16];						// Hex keypad 0x0-0xF

	} EmuContext_t;

	// Singleton for managing global state
	typedef struct Context_t {
		std::shared_ptr<DisplayContext_t> mDisplayContext;
		std::shared_ptr<EmuContext_t> mEmuContext;
	} Context_t;
	
}

extern std::shared_ptr<context::Context_t> ctx;