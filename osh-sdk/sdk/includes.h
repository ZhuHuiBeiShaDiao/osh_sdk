#pragma once

#pragma warning( disable : 4307 ) // warning C4307: '*': integral constant overflow
#pragma warning( disable : 4244 ) // warning C4244: '=': conversion possible loss of data

using ulong_t = unsigned long;

#define WIN32_LEAN_AND_MEAN
#define NOCOMM

#include <windows.h>
#include <intrin.h>
#include <cstdint>
#include <string>
#include <stdio.h>
#include <array>
#include <vector>
#include <map>
#include <memory>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cctype>
#include <random>
#include <thread>
#include <chrono>

// Direct X
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )

// oshgui.
#include "../../oshgui/OSHGui.hpp"
#include "../../oshgui/Drawing/Direct3D9/Direct3D9Renderer.hpp"
#include "../../oshgui/Drawing/RenderTarget.hpp"
#include "../../oshgui/Input/Input.hpp"

#include "../renderer/renderer.h"
#include "../utils/pattern.h"

using namespace renderer;
extern c_renderer g_renderer;

using OSHColor = OSHGui::Drawing::Color;

#include "../hooks/hooks.h"