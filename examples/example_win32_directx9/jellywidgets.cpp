#include "jellywidgets.h"

/// <summary>
///     滑块开关控件
/// </summary>
/// <param name="label"> 开关名称 </param>
/// <param name="state"> bool引用传值 </param>
/// <param name="size"> 默认控件宽高 </param>
/// <returns></returns
struct SwitchAnimState {
    float sliderProgress = 0.3f;
    float colorProgress = 0.0f;
};

static std::unordered_map<ImGuiID, SwitchAnimState> g_switchAnimStates;

bool JellyGui::SlideSwitchLerp(const char* label, bool* state, ImVec2 size = ImVec2(40, 20))
{

    ImGui::PushID(label); 
    ImGuiID id = ImGui::GetID(label);

    // 初始化每个控件的动画状态
    auto& anim = g_switchAnimStates[id];

    // 设置控件区域的矩形
    ImVec2 p0 = ImGui::GetCursorScreenPos();
    ImVec2 p1 = ImVec2(p0.x + size.x, p0.y + size.y);

    // 使用插值平滑动画  平滑状态变量
    //static float sliderProgress = 0.3f;                    // 当前滑块的进度（0.3 - 1.0）                    
    //float targetProgress = *state ? 1.0f : 0.3f;    // 目标滑块位置
    //static float colorProgress = 0.0f;                    // 当前颜色插值进度
    //float targetColorProgress = *state ? 1.0f : 0.0f;    // 目标颜色插值                                                  
    //static float animationSpeed = 0.05f;                   // 动画的速度（越小越慢）

    float targetSlider = *state ? 1.0f : 0.3f;
    float targetColor = *state ? 1.0f : 0.0f;
    float animationSpeed = 0.1f;


     // 插值更新
    anim.sliderProgress += animationSpeed * (targetSlider - anim.sliderProgress);
    anim.colorProgress += animationSpeed * (targetColor - anim.colorProgress);

    int red = (int)(anim.colorProgress * 104);
    int green = (int)(anim.colorProgress * 129);
    int blue = (int)(anim.colorProgress * 237);
    ImU32 bgColor = IM_COL32(red, green, blue, 255);

    //绘制控件背景色
    ImGui::GetWindowDrawList()->AddRectFilled(p0, p1, bgColor, 10.0f);

    // 设置滑块位置插值（通过插值平滑过渡）
    float handlePosX = p0.x + anim.sliderProgress * (p1.x - p0.x - size.y + 10.0f);
    ImVec2 handlePos = ImVec2(handlePosX, p0.y + 10.0f);

    // 绘制滑块
    ImGui::GetWindowDrawList()->AddCircleFilled(handlePos, size.y * 0.35f, IM_COL32(255, 255, 255, 255));

    // 交互处理
    if (ImGui::IsMouseClicked(0) && ImGui::IsMouseHoveringRect(p0, p1)) {
        *state = !(*state);
    }

    // 显示标签
    ImGui::SetCursorScreenPos(ImVec2(p1.x + 5.0f, p0.y));
    ImGui::Text(label);

    ImGui::PopID(); // 恢复控件ID
    return false;
}
// 滑块开关控件（无平滑）
bool JellyGui::SlideSwitch(const char* label, bool* state, ImVec2 size = ImVec2(40, 20))
{
    ImGui::PushID(label);  // 每个控件需要独特的ID

    // 设置控件区域的矩形
    ImVec2 p0 = ImGui::GetCursorScreenPos();
    ImVec2 p1 = ImVec2(p0.x + size.x, p0.y + size.y);

    // 使用插值平滑动画效果
    static float sliderProgress = *state ? 1.0f : 0.0f;  // 当前滑块的进度（0.0 - 1.0）

    //ImGui::SetCursorPos(ImVec2(50.0f, 350.0f));
    //ImGui::BeginChild("1234"); {
    //    ImGui::SliderFloat("sliderProgress2", &sliderProgress, 0.0f, 1.0f, "%.3f", 0);
    //}
    //ImGui::EndChild();

    // 绘制开关背景
    ImU32 bgColor = *state ? IM_COL32(0, 180, 255, 255) : IM_COL32(0, 0, 0, 255); // 开启时蓝色，关闭时黑色
    ImGui::GetWindowDrawList()->AddRectFilled(p0, p1, bgColor, 10.0f);

    // 设置滑块位置（无过渡）       // 参1：滑块终止位置  参1：滑块起始位置
    float handlePosX = *state ? p1.x - size.y + 10.0f : p0.x + 10.0f;  // 滑块的位置，左边为关闭，右边为开启
    ImVec2 handlePos = ImVec2(handlePosX, p0.y + 9.5f);
    //参：滑块的上下位置
// 绘制滑块
    ImGui::GetWindowDrawList()->AddCircleFilled(handlePos, size.y * 0.35f, IM_COL32(255, 255, 255, 255));

    // 检测点击事件，并且 鼠标停留在 p0  对角  p1 的区域 为 true
    if (ImGui::IsMouseClicked(0) && ImGui::IsMouseHoveringRect(p0, p1))
    {
        *state = !(*state);// 切换开关状态
    }

    // 显示标签
    ImGui::SetCursorScreenPos(ImVec2(p1.x + 5.0f, p0.y));
    ImGui::Text(label);

    ImGui::PopID(); // 恢复控件ID
    return false;
}

bool JellyGui::CustomButton(const char* label, const ImVec2& size,float radius)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiID id = window->GetID(label);
    ImVec2 pos = window->DC.CursorPos;
    ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));  // 修正 ImVec2 计算

    ImGui::ItemSize(bb);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    // 获取 IO 以检测鼠标点击
    ImGuiContext& g = *ImGui::GetCurrentContext();
    bool hovered = ImGui::IsItemHovered();
    bool clicked = ImGui::IsItemClicked();

    // 颜色存储
    static std::map<ImGuiID, ImVec4> button_colors;
    if (button_colors.find(id) == button_colors.end()) {
        button_colors[id] = ImVec4(0, 0, 0, 1);  // 初始黑色
    }

    // 目标颜色                             rgb(104, 129, 237)
    ImVec4 target_color = hovered ? ImVec4(0.35f, 0.49f, 0.87f, 1) : ImVec4(0.12f, 0.12f, 0.12f, 1);
    if (clicked) target_color = ImVec4(0.25f, 0.29f, 1.0f, 1.0f);  // 按下时蓝色

    float damping = 0.09f;  // 颜色平滑过渡速度

    // **线性阻尼颜色插值**
    ImVec4& current_color = button_colors[id];
    current_color.x = (1 - damping) * current_color.x + damping * target_color.x;
    current_color.y = (1 - damping) * current_color.y + damping * target_color.y;
    current_color.z = (1 - damping) * current_color.z + damping * target_color.z;
    current_color.w = (1 - damping) * current_color.w + damping * target_color.w;

    // 颜色转换
    ImU32 col = ImGui::GetColorU32(current_color);
    ImU32 text_col = ImGui::GetColorU32(ImVec4(0.8f, 0.8f, 0.8f, 1));

    // 绘制按钮
    window->DrawList->AddRectFilled(bb.Min, bb.Max, col, radius);  // 圆角 5.0 
    ImGui::RenderTextClipped(bb.Min, bb.Max, label, NULL, NULL, ImVec2(0.5f, 0.5f), &bb);

    if (clicked != 0) 
    {
        std::cout << "按钮状态 " << clicked << std::endl; 
    }
    return clicked;
}






//定义 imgui.h
//IMGUI_API void          BeginGroup();
//IMGUI_API void          EndGroup();

//实现 imgui.cpp

void ImGui::BeginGroup()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    g.GroupStack.resize(g.GroupStack.Size + 1);
    ImGuiGroupData& group_data = g.GroupStack.back();
    group_data.WindowID = window->ID;
    group_data.BackupCursorPos = window->DC.CursorPos;
    group_data.BackupCursorMaxPos = window->DC.CursorMaxPos;
    group_data.BackupIndent = window->DC.Indent;
    group_data.BackupGroupOffset = window->DC.GroupOffset;
    group_data.BackupCurrLineSize = window->DC.CurrLineSize;
    group_data.BackupCurrLineTextBaseOffset = window->DC.CurrLineTextBaseOffset;
    group_data.BackupActiveIdIsAlive = g.ActiveIdIsAlive;
    group_data.BackupHoveredIdIsAlive = g.HoveredId != 0;
    group_data.BackupActiveIdPreviousFrameIsAlive = g.ActiveIdPreviousFrameIsAlive;
    group_data.EmitItem = true;

    window->DC.GroupOffset.x = window->DC.CursorPos.x - window->Pos.x - window->DC.ColumnsOffset.x;
    window->DC.Indent = window->DC.GroupOffset;
    window->DC.CursorMaxPos = window->DC.CursorPos;
    window->DC.CurrLineSize = ImVec2(0.0f, 0.0f);
    if (g.LogEnabled)
        g.LogLinePosY = -FLT_MAX; // To enforce a carriage return
}

void ImGui::EndGroup()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    IM_ASSERT(g.GroupStack.Size > 0); // Mismatched BeginGroup()/EndGroup() calls

    ImGuiGroupData& group_data = g.GroupStack.back();
    IM_ASSERT(group_data.WindowID == window->ID); // EndGroup() in wrong window?

    if (window->DC.IsSetPos)
        ErrorCheckUsingSetCursorPosToExtendParentBoundaries();

    ImRect group_bb(group_data.BackupCursorPos, ImMax(window->DC.CursorMaxPos, group_data.BackupCursorPos));

    window->DC.CursorPos = group_data.BackupCursorPos;
    window->DC.CursorMaxPos = ImMax(group_data.BackupCursorMaxPos, window->DC.CursorMaxPos);
    window->DC.Indent = group_data.BackupIndent;
    window->DC.GroupOffset = group_data.BackupGroupOffset;
    window->DC.CurrLineSize = group_data.BackupCurrLineSize;
    window->DC.CurrLineTextBaseOffset = group_data.BackupCurrLineTextBaseOffset;
    if (g.LogEnabled)
        g.LogLinePosY = -FLT_MAX; // To enforce a carriage return

    if (!group_data.EmitItem)
    {
        g.GroupStack.pop_back();
        return;
    }

    window->DC.CurrLineTextBaseOffset = ImMax(window->DC.PrevLineTextBaseOffset, group_data.BackupCurrLineTextBaseOffset);      // FIXME: Incorrect, we should grab the base offset from the *first line* of the group but it is hard to obtain now.
    ItemSize(group_bb.GetSize());
    ItemAdd(group_bb, 0, NULL, ImGuiItemFlags_NoTabStop);

    // If the current ActiveId was declared within the boundary of our group, we copy it to LastItemId so IsItemActive(), IsItemDeactivated() etc. will be functional on the entire group.
    // It would be neater if we replaced window.DC.LastItemId by e.g. 'bool LastItemIsActive', but would put a little more burden on individual widgets.
    // Also if you grep for LastItemId you'll notice it is only used in that context.
    // (The two tests not the same because ActiveIdIsAlive is an ID itself, in order to be able to handle ActiveId being overwritten during the frame.)
    const bool group_contains_curr_active_id = (group_data.BackupActiveIdIsAlive != g.ActiveId) && (g.ActiveIdIsAlive == g.ActiveId) && g.ActiveId;
    const bool group_contains_prev_active_id = (group_data.BackupActiveIdPreviousFrameIsAlive == false) && (g.ActiveIdPreviousFrameIsAlive == true);
    if (group_contains_curr_active_id)
        g.LastItemData.ID = g.ActiveId;
    else if (group_contains_prev_active_id)
        g.LastItemData.ID = g.ActiveIdPreviousFrame;
    g.LastItemData.Rect = group_bb;

    // Forward Hovered flag
    const bool group_contains_curr_hovered_id = (group_data.BackupHoveredIdIsAlive == false) && g.HoveredId != 0;
    if (group_contains_curr_hovered_id)
        g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HoveredWindow;

    // Forward Edited flag
    if (group_contains_curr_active_id && g.ActiveIdHasBeenEditedThisFrame)
        g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_Edited;

    // Forward Deactivated flag
    g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HasDeactivated;
    if (group_contains_prev_active_id && g.ActiveId != g.ActiveIdPreviousFrame)
        g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_Deactivated;

    g.GroupStack.pop_back();
    //window->DrawList->AddRect(group_bb.Min, group_bb.Max, IM_COL32(255,0,255,255));   // [Debug]
}











//bool begin_child_ex(const char* name, ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags flags)
//{
//    ImGuiContext& g = *GImGui;
//    ImGuiWindow* parent_window = g.CurrentWindow;
//
//    flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_ChildWindow;
//    flags |= (parent_window->Flags & ImGuiWindowFlags_NoMove);  // Inherit the NoMove flag
//
//    // Size
//    const ImVec2 content_avail = ImGui::GetContentRegionAvail(); 
//    ImVec2 size = ImFloor(size_arg);
//    const int auto_fit_axises = ((size.x == 0.0f) ? (1 << ImGuiAxis_X) : 0x00) | ((size.y == 0.0f) ? (1 << ImGuiAxis_Y) : 0x00);
//    if (size.x <= 0.0f)
//        size.x = ImMax(content_avail.x + size.x, 4.0f); // Arbitrary minimum child size (0.0f causing too much issues)
//    if (size.y <= 0.0f)
//        size.y = ImMax(content_avail.y + size.y, 4.0f);
//
//    SetNextWindowPos(ImVec2(parent_window->DC.CursorPos.x, parent_window->DC.CursorPos.y + 34.0f));
//    SetNextWindowSize(size - ImVec2(0, 36));
//
//    parent_window->DrawList->AddRectFilled(parent_window->DC.CursorPos, parent_window->DC.CursorPos + ImVec2(size.x, size.y), ImColor(24, 24, 26), 4.0f);
//    parent_window->DrawList->AddRect(parent_window->DC.CursorPos, parent_window->DC.CursorPos + ImVec2(size.x, size.y), ImColor(1.0f, 1.0f, 1.0f, 0.03f), 4.0f);
//    parent_window->DrawList->AddLine(parent_window->DC.CursorPos + ImVec2(1, 32), parent_window->DC.CursorPos + ImVec2(size.x - 1, 32), ImColor(1.0f, 1.0f, 1.0f, 0.03f));
//    parent_window->DrawList->AddRectFilledMultiColorRounded(parent_window->DC.CursorPos, parent_window->DC.CursorPos + ImVec2(size.x - 115, 32), ImColor(24, 24, 26), ImColor(147, 190, 66, 50), ImColor(147, 190, 66, 0), ImColor(147, 190, 66, 0), ImColor(147, 190, 66, 50), 4.0f, ImDrawCornerFlags_TopLeft);
//    parent_window->DrawList->AddText(bold, 15.0f, parent_window->DC.CursorPos + ImVec2(16, 9), ImColor(147, 190, 66), name);
//
//    // Build up name. If you need to append to a same child from multiple location in the ID stack, use BeginChild(ImGuiID id) with a stable value.
//    const char* temp_window_name;
//    if (name)
//        ImFormatStringToTempBuffer(&temp_window_name, NULL, "%s/%s_%08X", parent_window->Name, name, id);
//    else
//        ImFormatStringToTempBuffer(&temp_window_name, NULL, "%s/%08X", parent_window->Name, id);
//
//    const float backup_border_size = g.Style.ChildBorderSize;
//    if (!border)
//        g.Style.ChildBorderSize = 0.0f;
//    bool ret = Begin(temp_window_name, NULL, flags);
//    g.Style.ChildBorderSize = backup_border_size;
//
//    ImGuiWindow* child_window = g.CurrentWindow;
//    child_window->ChildId = id;
//    child_window->AutoFitChildAxises = (ImS8)auto_fit_axises;
//
//    // Set the cursor to handle case where the user called SetNextWindowPos()+BeginChild() manually.
//    // While this is not really documented/defined, it seems that the expected thing to do.
//    if (child_window->BeginCount == 1)
//        parent_window->DC.CursorPos = child_window->Pos;
//
//    // Process navigation-in immediately so NavInit can run on first frame
//    if (g.NavActivateId == id && !(flags & ImGuiWindowFlags_NavFlattened) && (child_window->DC.NavLayersActiveMask != 0 || child_window->DC.NavHasScroll))
//    {
//        FocusWindow(child_window);
//        NavInitWindow(child_window, false);
//        SetActiveID(id + 1, child_window); // Steal ActiveId with another arbitrary id so that key-press won't activate child item
//        g.ActiveIdSource = ImGuiInputSource_Nav;
//    }
//    return ret;
//}
//
//bool begin_child(const char* str_id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags)
//{
//    ImGuiWindow* window = GetCurrentWindow();
//    PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 11));
//    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
//    return begin_child_ex(str_id, window->GetID(str_id), size_arg, border, extra_flags | ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoMove);
//}
//
//bool begin_child(ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags)
//{
//    IM_ASSERT(id != 0);
//    return begin_child_ex(NULL, id, size_arg, border, extra_flags);
//}
//
//void end_child()
//{
//    ImGuiContext& g = *GImGui;
//    ImGuiWindow* window = g.CurrentWindow;
//
//    IM_ASSERT(g.WithinEndChild == false);
//    IM_ASSERT(window->Flags & ImGuiWindowFlags_ChildWindow);   // Mismatched BeginChild()/EndChild() calls
//
//    g.WithinEndChild = true;
//    if (window->BeginCount > 1)
//    {
//        End();
//    }
//    else
//    {
//        ImVec2 sz = window->Size;
//        if (window->AutoFitChildAxises & (1 << ImGuiAxis_X)) // Arbitrary minimum zero-ish child size of 4.0f causes less trouble than a 0.0f
//            sz.x = ImMax(4.0f, sz.x);
//        if (window->AutoFitChildAxises & (1 << ImGuiAxis_Y))
//            sz.y = ImMax(4.0f, sz.y);
//        End();
//
//        ImGuiWindow* parent_window = g.CurrentWindow;
//        ImRect bb(parent_window->DC.CursorPos, parent_window->DC.CursorPos + sz);
//        ItemSize(sz);
//        if ((window->DC.NavLayersActiveMask != 0 || window->DC.NavHasScroll) && !(window->Flags & ImGuiWindowFlags_NavFlattened))
//        {
//            ItemAdd(bb, window->ChildId);
//            RenderNavHighlight(bb, window->ChildId);
//
//            // When browsing a window that has no activable items (scroll only) we keep a highlight on the child (pass g.NavId to trick into always displaying)
//            if (window->DC.NavLayersActiveMask == 0 && window == g.NavWindow)
//                RenderNavHighlight(ImRect(bb.Min - ImVec2(2, 2), bb.Max + ImVec2(2, 2)), g.NavId, ImGuiNavHighlightFlags_TypeThin);
//        }
//        else
//        {
//            // Not navigable into
//            ItemAdd(bb, 0);
//        }
//        if (g.HoveredWindow == window)
//            g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HoveredWindow;
//    }
//    ImGui::PopStyleVar(2);
//    g.WithinEndChild = false;
//    g.LogLinePosY = -FLT_MAX; // To enforce a carriage return
//}















// Note: p_data, p_min and p_max are _pointers_ to a memory address holding the data. For a slider, they are all required.
// Read code of e.g. SliderFloat(), SliderInt() etc. or examples in 'Demo->Widgets->Data Types' to understand how to use this function directly.
/*



struct slider_element {
    float opacity;
};

static const char* PatchFormatStringFloatToInt(const char* fmt)
{
    if (fmt[0] == '%' && fmt[1] == '.' && fmt[2] == '0' && fmt[3] == 'f' && fmt[4] == 0) // Fast legacy path for "%.0f" which is expected to be the most common case.
        return "%d";
    const char* fmt_start = ImParseFormatFindStart(fmt);    // Find % (if any, and ignore %%)
    const char* fmt_end = ImParseFormatFindEnd(fmt_start);  // Find end of format specifier, which itself is an exercise of confidence/recklessness (because snprintf is dependent on libc or user).
    if (fmt_end > fmt_start && fmt_end[-1] == 'f')
    {
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
        if (fmt_start == fmt && fmt_end[0] == 0)
            return "%d";
        const char* tmp_format;
        ImFormatStringToTempBuffer(&tmp_format, NULL, "%.*s%%d%s", (int)(fmt_start - fmt), fmt, fmt_end); // Honor leading and trailing decorations, but lose alignment/precision.
        return tmp_format;
#else
        IM_ASSERT(0 && "DragInt(): Invalid format string!"); // Old versions used a default parameter of "%.0f", please replace with e.g. "%d"
#endif
    }
    return fmt;
}

bool ImGui::SliderScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const float w = GetWindowWidth() - 30;

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImRect total_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + 16));
    const ImRect frame_bb(total_bb.Min + ImVec2(0, label_size.y + 10), total_bb.Max);

    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id, &frame_bb))
        return false;

    // Default format string when passing NULL
    if (format == NULL)
        format = DataTypeGetInfo(data_type)->PrintFmt;
    else if (data_type == ImGuiDataType_S32 && strcmp(format, "%d") != 0) // (FIXME-LEGACY: Patch old "%.0f" format string to use "%d", read function more details.)
        format = PatchFormatStringFloatToInt(format);

    // Tabbing or CTRL-clicking on Slider turns it into an input box
    const bool hovered = ItemHoverable(frame_bb, id,0);
    const bool clicked = (hovered && g.IO.MouseClicked[0]);
    if (clicked)
    {
        SetActiveID(id, window);
        SetFocusID(id, window);
        FocusWindow(window);
        g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
    }

    static std::map <ImGuiID, slider_element> anim;
    auto it_anim = anim.find(id);
    if (it_anim == anim.end())
    {
        anim.insert({ id, { 0.0f } });
        it_anim = anim.find(id);
    }

    it_anim->second.opacity = ImLerp(it_anim->second.opacity, IsItemActive() ? 1.0f : 0.4f, 0.08f * (1.0f - ImGui::GetIO().DeltaTime));

    ImRect grab_bb;
    const bool value_changed = SliderBehavior(frame_bb, id, data_type, p_data, p_min, p_max, format, flags, &grab_bb);
    if (value_changed)
        MarkItemEdited(id);

    char value_buf[64];
    const char* value_buf_end = value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);

    window->DrawList->AddRectFilled(frame_bb.Min, frame_bb.Max, ImColor(15, 15, 16), 5.0f);
    window->DrawList->AddRectFilled(frame_bb.Min, ImVec2(grab_bb.Min.x + 3, frame_bb.Max.y), ImColor(104, 129, 237), 5.0f);
    window->DrawList->AddCircleFilled(ImVec2(grab_bb.Min.x - 1, grab_bb.Min.y + 1), 6.0f, ImColor(1.0f, 1.0f, 1.0f), 30);

    RenderText(total_bb.Min, label);

    PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, it_anim->second.opacity)); {
        RenderTextClipped(total_bb.Min, total_bb.Max, value_buf, value_buf_end, NULL, ImVec2(1.f, 0.f));
    } PopStyleColor();

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags);
    return value_changed;
}

bool ImGui::SliderInt(const char* label, int* v, int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
{
    return SliderScalar(label, ImGuiDataType_S32, v, &v_min, &v_max, format, flags);
}
bool ImGui::SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags)
{
    return SliderScalar(label, ImGuiDataType_Float, v, &v_min, &v_max, format, flags);
}

*/





///

