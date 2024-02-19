#pragma once
#include "dataTypes.hpp"
#include "defines.hpp"
#include "platformWindows.hpp"
#include "graphicsVulkan.hpp"
#include "parserWavFile.hpp"

START_SCOPE(mainWindow)

struct State
{
    void* vulkan;
    uint objectIndex;
    uint width;
};

void create();

END_SCOPE

