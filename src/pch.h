#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <iomanip>
#include <array>
#include <vector>
#include <queue>
#include <list>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

#define _USE_MATH_DEFINES
#include <cmath>

#ifdef _WIN32
#include <Windows.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Core/Log.h"
#include "Core/WindowEvent.h"

#ifdef PDEBUG
#define LOG_ASSERT(x, ...) { if(!(x)) { ::Core::Log::Error("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define LOG_ASSERT(x, ...)
#endif