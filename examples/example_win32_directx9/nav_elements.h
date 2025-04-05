#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS 
#include "imgui.h"
#include "imgui_internal.h"

#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <map>


//extern ImFont* tab_icons;

enum heads {
    rage, antiaim, visuals, settings, skins, configs, scripts
};

enum sub_heads {
    general, accuracy, exploits, _general, advanced
};

static heads tab{ rage }; 
static sub_heads subtab{ general };

namespace elements {
    bool tab(const char* icon, bool boolean);
    bool subtab(const char* name, bool boolean);

    bool begin_child_ex(const char* name, ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags flags);
    bool begin_child(const char* str_id, const ImVec2& size = ImVec2(0, 0), bool border = false, ImGuiWindowFlags flags = 0);
    bool begin_child(ImGuiID id, const ImVec2& size = ImVec2(0, 0), bool border = false, ImGuiWindowFlags flags = 0);
    void end_child();
}
