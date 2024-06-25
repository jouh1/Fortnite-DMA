#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <algorithm> 
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <fstream>
#include <ppltasks.h>
#include <windowsx.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <devguid.h>
#include <TlHelp32.h>
#include <thread>
#include <setupapi.h>
#include <filesystem>
#include <cctype>    
#include <tchar.h>
#include <iomanip>
#include <random>
#include <sstream>
#include <locale>
#include <cstdint>
#include <dwrite.h>
#include <dwmapi.h>
#include <ShellScalingApi.h>
#include <vmmdll.h>

#include "json.hpp"
using json = nlohmann::json;

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "vmm.lib")
#pragma comment(lib, "leechcore.lib")
#pragma comment(lib, "FTD3XX.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "Dwrite")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "Dwmapi.lib")
#pragma comment(lib, "Shcore.lib")

#include <spdlog/spdlog.h>

#define DEBUG_INFO
#ifdef DEBUG_INFO
#define LOG(fmt, ...) std::printf(fmt, ##__VA_ARGS__)
#define LOGW(fmt, ...) std::wprintf(fmt, ##__VA_ARGS__)
#else
#define LOG
#define LOGW
#endif

#define THROW_EXCEPTION
#ifdef THROW_EXCEPTION
#define THROW(fmt, ...) throw std::runtime_error(fmt, ##__VA_ARGS__)
#endif


#include "Memory.h"
#include "InputManager.h"
#include <Kmbox/kmbox.hpp>
#include <kmbox/kmboxNet.h>