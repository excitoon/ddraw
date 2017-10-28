#pragma once

#include "LogLevel.h"


struct Constants
{
    static const bool Emulate16BitsPerPixel = true;

    static const bool EnablePrimarySurfaceBackgroundBuffering = true;
    static const bool ReadFromSurfaceMemoryOncePerBuffer = true;
    static const bool InitializeBuffersWithZeros = true;
    static const unsigned MaxPrimarySurfaceBufferWidth = 1024;
    static const unsigned MaxPrimarySurfaceBufferHeight = 768;

    static const bool AllowTrashInSurfaceUnlockRect = true;

    static const bool DisableExclusiveCooperativeLevel = true;

    static const LogLevel LogFilterLevel = LogLevel::Trace;
};

const bool Constants::Emulate16BitsPerPixel;
const bool Constants::EnablePrimarySurfaceBackgroundBuffering;
const bool Constants::ReadFromSurfaceMemoryOncePerBuffer;
const bool Constants::InitializeBuffersWithZeros;
const unsigned Constants::MaxPrimarySurfaceBufferWidth;
const unsigned Constants::MaxPrimarySurfaceBufferHeight;
const bool Constants::AllowTrashInSurfaceUnlockRect;
const bool Constants::DisableExclusiveCooperativeLevel;
const LogLevel Constants::LogFilterLevel;
