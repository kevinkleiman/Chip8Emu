#include <SDL.h>
#include <iostream>
#include "../include/context.h"
#include "../include/common.h"

#undef main

namespace display {

	bool init()
	{
		auto displayContext = ctx->mDisplayContext;

		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0)
		{
			SDL_Log("Unable to initialize SDL window\n");
			return false;
		}

		displayContext->mWindow = SDL_CreateWindow("Chip8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				displayContext->mWindowWidth, displayContext->mWindowHeight, 0);
		displayContext->mRenderer = SDL_CreateRenderer(displayContext->mWindow, -1, SDL_RENDERER_ACCELERATED);

		if (!displayContext->mRenderer)
		{
			SDL_Log("Could not create SDL Renderer \n");
			return false;
		}

		SDL_RenderClear(displayContext->mRenderer);
		return true;
	}

	void setColor(int r, int g, int b, int a)
	{
		auto displayContext = ctx->mDisplayContext;

		SDL_SetRenderDrawColor(displayContext->mRenderer, r, g, b, a);
	}

	void updateDisplay()
	{
		// 60hz refresh rate
		auto displayContext = ctx->mDisplayContext;
		auto memory = ctx->mEmuContext->mMemory;

		setColor(0, 0, 0, 255);
		SDL_RenderClear(displayContext->mRenderer);

		setColor(255, 255, 255, 255);

		// Initialize pixel with w, h being set to scale (i.e scaling the pixels)
		SDL_Rect rect;
		rect.w = RESOLUTION_SCALE;
		rect.h = RESOLUTION_SCALE;

		unsigned int x, y = 0;
		int fullResolution = RESOLUTION_HEIGHT * RESOLUTION_WIDTH;

		// Loop through each pixel in mLcd[]
		for (int i = 0; i < fullResolution; ++i)
		{
			// Calculate x and y using some fancy math...
			// Someone please tell me if this is stupid...
			x = (i % RESOLUTION_WIDTH) * RESOLUTION_SCALE;
			if ((i % RESOLUTION_WIDTH == 0) && (i != 0)) y += RESOLUTION_SCALE;

			// If the pixel is on (true) then set the scaled pixel to x, y
			if (memory->mPixelBuffer[i] == true) {
				rect.x = x;
				rect.y = y;

				// Finally, render pixel
				SDL_RenderFillRect(displayContext->mRenderer, &rect);
			}
		}


		SDL_RenderPresent(displayContext->mRenderer);
	}

	void cleanup()
	{
		auto displayContext = ctx->mDisplayContext;

		SDL_DestroyRenderer(displayContext->mRenderer);
		SDL_DestroyWindow(displayContext->mWindow);
		SDL_Quit();
	}
}
