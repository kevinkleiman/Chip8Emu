#include <iostream>
#include "../include/context.h"
#include "../include/display.h"
#include "../include/common.h"
#include "../include/cpu.h"
#include "../include/memory.h"


std::shared_ptr<context::ContextSingleton_t> ctx = std::make_shared<context::ContextSingleton_t>();

// Parses args (ex. width, height) from CLI
void configFromArgs(int argc, char** argv)
{
    DEBUG_LOG(argv[1]);
    ctx->mEmuContext->mRomFileName = argv[1];
}

void init()
{
    // Initialize context
    ctx->mDisplayContext = std::make_shared<context::DisplayContext_t>();
    ctx->mEmuContext = std::make_shared<context::EmuContext_t>();

    // Initialize hardware at startup
    ctx->mEmuContext->mCpu = std::make_shared<CPU>();
    ctx->mEmuContext->mMemory = std::make_shared<Memory>();

	// Inititalize hardware and set up hardware state
	ctx->mEmuContext->mStatus = context::RUNNING;
	ctx->mEmuContext->mCpu->mPC = ROM_ENTRY;
}

int main(int argc, char **argv)
{
    // Initialize hardware and context
    init();

    // Set up config form cli arg
    configFromArgs(argc, argv);

    // Check for SDL initialization
    if (!display::init()) exit(EXIT_FAILURE);

    // Load font and ROM into memory
    ctx->mEmuContext->mMemory->LoadFont();
    ctx->mEmuContext->mMemory->LoadRom();

    // Start CPU cycle
	ctx->mEmuContext->mCpu->Cycle();

    // Cleanup and destroy SDL windows
    display::cleanup();

    return 0;
}

