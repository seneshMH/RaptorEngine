#pragma once

#include "Raptor/Core/PlatformDetecttion.h"

#ifdef RT_PLATFORM_WINDOWS
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif
#endif

#include<iostream>
#include <ostream>
#include<memory>
#include<algorithm>
#include<utility>
#include<functional>

#include<string>
#include<sstream>
#include<vector>
#include <array>
#include<unordered_map>
#include<unordered_set>

#include "Raptor/Core/Log.h"
#include "Raptor/Debug/Instrumentor.h"
#include "Raptor/Core/Base.h"

#ifdef RT_PLATFORM_WINDOWS
	#include<Windows.h>
#endif

