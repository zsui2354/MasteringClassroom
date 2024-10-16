//#pragma once

#ifndef LIB_FUNCTION_H
#define LIB_FUNCTION_H

#include <Windows.h>
#include <tchar.h>
#include <TlHelp32.h>
#include <exception>
#include <psapi.h>


#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

LONG_PTR GetPID(const TCHAR* processName);//获取进程PID

LONG_PTR GetModuleBase(const TCHAR* moduleName, HANDLE handle);//获取模块句柄  ___弃用

ImVec4 color_Changes(int value);    //判断数值返回对应颜色

#endif // ADDRESS_OFFSET_H
