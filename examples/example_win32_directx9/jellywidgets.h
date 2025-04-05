#pragma once

#include "imgui.h"
#include "imgui_internal.h"
// System includes
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <ctype.h>      // toupper
#include <stddef.h>     // intptr_t
#include <stdint.h>     // intptr_t
#include <Windows.h>
#include <map>
#include <unordered_map>
#include <stdio.h>  
#include <stdint.h> 


namespace JellyGui {
    bool SlideSwitchLerp(const char* label, bool* state, ImVec2 size );             //滑块开关 （线性阻尼逼近平滑过渡） 

    bool SlideSwitch(const char* label, bool* state, ImVec2 size);                  //滑块开关 （无平滑过渡）

    bool CustomButton(const char* label, const ImVec2& size, float radius = 5.0f);  //常用按钮

}

