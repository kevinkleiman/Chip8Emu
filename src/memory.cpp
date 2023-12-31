#include <iostream>
#include <stdint.h>
#include "../include/memory.h"
#include "../include/context.h"
#include "../include/common.h"


void Memory::LoadFont()
{
    auto memory = ctx->mEmuContext->mMemory;

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

    memcpy(&memory->mRam, font, sizeof(font));
}

void Memory::LoadRom()
{
    auto emuCtx = ctx->mEmuContext;
    auto memory = ctx->mEmuContext->mMemory;

    FILE* pRom;
    FILE** pRomFile = &pRom;

    size_t romSize;
    size_t maxRomSize = sizeof(memory->mRam) - ROM_ENTRY;

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

    if (!fread(&memory->mRam[ROM_ENTRY], romSize, 1, pRom))
    {
        DEBUG_LOG("Could not read ROM file into memory");
        exit(1);
    }

    fclose(pRom);
}