#pragma once


struct Constants
{
    static const bool Emulate16BitsPerPixel = true;

    static const bool AllowToUseUnlockedSurfaceMemory = true;
    static const bool ReadFromSurfaceMemoryOncePerBuffer = true;
    static const bool InitializeBuffersWithZeros = true;
    static const unsigned MaxPrimarySurfaceBufferWidth = 1024;

    static const bool AllowTrashInSurfaceUnlockRect = true;

    static const bool DisableExclusiveCooperativeLevel = true;
};
