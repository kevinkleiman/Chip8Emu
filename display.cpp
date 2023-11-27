#include <SDL.h>
#include <iostream>
#include "context.h"
#include "common.h"

#undef main

namespace display {

	bool init()
	{
		auto displayState = ctx->mDisplayContext;

		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0)
		{
			SDL_Log("Unable to initialize SDL window\n");
			
			return false;
		}

		displayState->mWindow = SDL_CreateWindow("Chip8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				displayState->mWindowWidth, displayState->mWindowHeight, 0);

		displayState->mRenderer = SDL_CreateRenderer(displayState->mWindow, -1, SDL_RENDERER_ACCELERATED);

		if (!displayState->mRenderer)
		{
			SDL_Log("Could not create SDL Renderer \n");

			return false;
		}

		SDL_RenderClear(displayState->mRenderer);

		return true;
	}

	void setFBColor(uint32_t rgba, int layer)
	{
		auto displayState = ctx->mDisplayContext;

		// Bitwise to get RGBA
		uint8_t r = (rgba >> 24) & 0xFF;
		uint8_t g = (rgba >> 16) & 0xFF;
		uint8_t b = (rgba >> 8) & 0xFF;
		uint8_t a = (rgba >> 0) & 0xFF;

		SDL_SetRenderDrawColor(displayState->mRenderer, r, g, b, a);
		SDL_RenderClear(displayState->mRenderer);
	}

	void updateDisplay()
	{
		// 60hz refresh rate
		auto displayState = ctx->mDisplayContext;

		setFBColor(displayState->mBackgroundColor, BACKGROUND_LAYER);

		SDL_Delay(16);
		SDL_RenderPresent(displayState->mRenderer);
	}

	void cleanup()
	{
		auto displayState = ctx->mDisplayContext;

		SDL_DestroyRenderer(displayState->mRenderer);
		SDL_DestroyWindow(displayState->mWindow);
		SDL_Quit();
	}
}
