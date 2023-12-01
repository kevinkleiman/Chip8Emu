#pragma once

#include <iostream>
#include <stdint.h>
#include <SDL.h>
#include "cpu.h"
#include "memory.h"

namespace context {

	typedef enum EmuStatus_t {
		QUIT,
		RUNNING,
		PAUSED
	} EmuStatus;

	
	// Singleton for managing display state
	typedef struct DisplayContext_t {
		uint32_t mWindowWidth = 640;
		uint32_t mWindowHeight = 320;

		uint32_t mForegroundColor = 0xFFFFFFFF;	// WHITE RGBA888
		uint32_t mBackgroundColor = 0x000000FF; // BLACK RGBA888

		SDL_Window* mWindow;
		SDL_Renderer* mRenderer;
	} DisplayContext;

	// Singleton for managing emulator state
	typedef struct EmuContext_t {
		EmuStatus_t mStatus;

		std::string mRomName;

		std::shared_ptr<CPU> mCpu;
		std::shared_ptr<Memory> mMemory;

		const char* mRomFileName;
		bool mKeypad[16];						// Hex keypad 0x0-0xF

	} EmuContext;

	// Singleton for managing global state
	typedef struct ContextSingleton_t {
		std::shared_ptr<DisplayContext_t> mDisplayContext;
		std::shared_ptr<EmuContext_t> mEmuContext;
	} ContextSingleton;
	
}

extern std::unique_ptr<context::ContextSingleton_t> ctx;
