#include <SDL.h>
#include <iostream>
#include "../include/context.h"
#include "../include/common.h"

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

	void setColor(int r, int g, int b, int a)
	{
		auto displayState = ctx->mDisplayContext;

		SDL_SetRenderDrawColor(displayState->mRenderer, r, g, b, a);
	}

	void updateDisplay()
	{
		// 60hz refresh rate
		auto displayState = ctx->mDisplayContext;

		setColor(0, 0, 0, 255);
		SDL_RenderClear(displayState->mRenderer);

		setColor(255, 255, 255, 255);

		// Initialize pixel with w, h being set to scale (i.e scaling the pixels)
		SDL_Rect rect;
		rect.w = RESOLUTION_SCALE;
		rect.h = RESOLUTION_SCALE;

		unsigned int x, y = 0;

		// Loop through each pixel in mLcd[]
		for (int i = 0; i < 2048; i++)
		{
			// Calculate x and y using some fancy math...
			// Someone please tell me if this is stupid...
			x = (i % RESOLUTION_WIDTH) * RESOLUTION_SCALE;
			if ((i % RESOLUTION_WIDTH == 0) && (i != 0)) y += RESOLUTION_SCALE;

			// If the pixel is on (true) then set the scaled pixel to x, y
			if (ctx->mEmuContext->mLcd[i] == true) {
				rect.x = x;
				rect.y = y;

				// Finally, render pixel
				SDL_RenderFillRect(displayState->mRenderer, &rect);
			}
		}


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
