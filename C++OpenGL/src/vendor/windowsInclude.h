#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <commdlg.h>

#pragma comment(lib, "Comdlg32.lib")

#ifdef DELETE
#undef DELETE
#endif
