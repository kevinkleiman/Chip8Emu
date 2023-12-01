#include <iostream>
#include <stdint.h>
#include <chrono>
#include <cstdlib>
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
        SDL_Delay(16);

        EmulateInstr();

        // Decrement timers
        --mDelaytimer;
        --mSoundTimer;

        // Updates SDL display, not Chip8 display render
        display::updateDisplay();

    }
}

bool CPU::PollInput()
{
    auto emuCtx = ctx->mEmuContext;

    SDL_Event event;
    mKeyBuffer[mLastKeyPress] = false;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
        {
            emuCtx->mStatus = context::QUIT;
            return false;
        }
        case SDL_KEYDOWN:
        {
            return KeyPress(event.key.keysym);
        }
        case SDL_KEYUP:
            return false;
        }
    }
}

bool CPU::KeyPress(SDL_Keysym key)
{
    auto emuCtx = ctx->mEmuContext;

    switch (key.sym)
    {
    case SDLK_ESCAPE:
    {
        emuCtx->mStatus = context::QUIT;
        return true;
    }
    case SDLK_SPACE:
    {
        if (emuCtx->mStatus == context::RUNNING) emuCtx->mStatus = context::PAUSED;
        else emuCtx->mStatus = context::RUNNING;

        return true;
    }
    default:
        if (key.sym >= 48 && key.sym <= 57)
        {
            mKeyBuffer[key.sym - 48] = true;
            mLastKeyPress = key.sym;
        }
        else if (key.sym >= 97 && key.sym <= 102)
        {
            mKeyBuffer[key.sym - 87] = true;
            mLastKeyPress = key.sym;
        }

        return true;
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

void CPU::PushRegs()
{
    auto memory = ctx->mEmuContext->mMemory;

    for (uint8_t i = 0; i <= mVRegisters[mInstr.x]; ++i)
    {
        memory->mRam[mI + i] = mVRegisters[i];
    }
}

void CPU::PopRegs()
{
    auto memory = ctx->mEmuContext->mMemory;

    for (uint16_t i = 0; i <= mVRegisters[mInstr.x]; ++i)
    {
        mVRegisters[i] = memory->mRam[mI + i];
    }
}

void CPU::_0x0()
{
    auto memory = ctx->mEmuContext->mMemory;

    switch (mInstr.n)
    {
    case 0x0:
    {
        memset(&memory->mPixelBuffer, 0, sizeof(memory->mPixelBuffer));
        break;
    }
    case 0xE:
    {
        mPC = memory->mStack[mSP];
        --mSP;
        break;
    }
    }

    mPC += 2;
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

    mPC += 2;
}

void CPU::_0x4()
{
    if (mVRegisters[mInstr.x] != mInstr.kk)
    {
        mPC += 2;
    }

    mPC += 2;
}

void CPU::_0x5()
{
    if (mVRegisters[mInstr.x] == mVRegisters[mInstr.y])
    {
        mPC += 2;
    }

    mPC += 2;
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
        if ((mVRegisters[mInstr.x] & 0x1) == 0x1)
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
        if ((mVRegisters[mInstr.x] & 0x80) == 0x8)
        {
            mVRegisters[0xF] = 0x1;
        }
        else
        {
            mVRegisters[0xF] = 0x0;
        }

        mVRegisters[mInstr.x] <<= 1;
        mPC += 2;
        break;
    }
    }
}

void CPU::_0x9()
{
    if (mVRegisters[mInstr.x] != mVRegisters[mInstr.y])
    {
        mPC += 2;
    }

    mPC += 2;
}

void CPU::_0xA()
{
    mI = mInstr.nnn;
    mPC += 2;
}

void CPU::_0xB()
{
    mPC = mInstr.nnn + mVRegisters[0x0];
}

void CPU::_0xC()
{
    uint8_t rByte = rand() % 256;

    mVRegisters[mInstr.x] &= rByte;
    mPC += 2;
}

void CPU::_0xD()
{
    auto memory = ctx->mEmuContext->mMemory;

    uint32_t sprite, pixelOffset, row, col, b;

    for (int i = 0; i < mInstr.n; ++i)
    {
        sprite = memory->mRam[mI + i];
        row = (mVRegisters[mInstr.y] + i) % 32;

        for (int iBit = 0; iBit < 8; ++iBit)
        {
            b = (sprite & 0x80) >> 7;
            col = (mVRegisters[mInstr.x] + iBit) % 64;
            pixelOffset = row * 64 + col;

            if (b == 1)
            {
                mVRegisters[0xF] = (memory->mPixelBuffer[pixelOffset]) ? 0x1 : 0x0;
                memory->mPixelBuffer[pixelOffset] = true;
            }

            sprite <<= 1;
        }
    }
    mPC += 2;
}

void CPU::_0xE()
{
    switch (mInstr.n)
    {
    case 0xE:
    {
        mPC += (mKeyBuffer[mVRegisters[mInstr.x]] == true) ? 4 : 2;
    }
    case 0x1:
    {
        mPC += (mKeyBuffer[mVRegisters[mInstr.x]] != true) ? 4 : 2;
    }
    }
}

void CPU::_0xF()
{
    auto memory = ctx->mEmuContext->mMemory;

    switch (mInstr.n)
    {
    case 0x7:
    {
        mVRegisters[mInstr.x] = mDelaytimer;
        break;
    }
    case 0xA:
    {
        bool pressed = PollInput();

        // Hang until key is pressed
        if (!pressed) mPC -= 2;

        mVRegisters[mInstr.x] = mLastKeyPress;
        break;
    }
    case 0x5:
    {
        switch (mInstr.y)
        {
        case 0x1:
        {
            mDelaytimer = mVRegisters[mInstr.x];
            break;
        }
        case 0x5:
        {
            PushRegs();
            break;
        }
        case 0x6:
        {
            PopRegs();
            break;
        }
        }

    }
    case 0x8:
    {
        mSoundTimer = mVRegisters[mInstr.x];
        break;
    }
    case 0xE:
    {
        mI += mVRegisters[mInstr.x];
        break;
    }
    case 0x9:
    {
        mI = memory->mRam[mVRegisters[mInstr.x] * 5];
        break;
    }
    case 0x3:
    {
        memory->mRam[mI] = mVRegisters[mInstr.x] / 100;
        memory->mRam[mI + 1] = (mVRegisters[mInstr.x] / 10) % 10;
        memory->mRam[mI + 2] = (mVRegisters[mInstr.x] % 100) % 10;
        break;
    }
    }

    mPC += 2;
}
