#include "nav_elements.h"











struct tab_element {
    float element_opacity, rect_opacity, text_opacity;
};

bool elements::tab(const char* name, bool boolean)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(name);
   // const ImVec2 label_size = tab_icons->CalcTextSizeA(15.0f, FLT_MAX, 0.0f, name);
    ImVec2 pos = window->DC.CursorPos;

    const ImRect rect(pos, ImVec2(pos.x + 31, pos.y + 31));
    ImGui::ItemSize(ImVec4(rect.Min.x, rect.Min.y, rect.Max.x, rect.Max.y + 5), style.FramePadding.y);
    if (!ImGui::ItemAdd(rect, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held, NULL);

    static std::map <ImGuiID, tab_element> anim;
    auto it_anim = anim.find(id);
    if (it_anim == anim.end()) {
        anim.insert({ id, { 0.0f, 0.0f, 0.0f } });
        it_anim = anim.find(id);
    }

    it_anim->second.element_opacity = ImLerp(it_anim->second.element_opacity, (boolean ? 0.04f : hovered ? 0.01f : 0.0f), 0.07f * (1.0f - ImGui::GetIO().DeltaTime));
    it_anim->second.rect_opacity = ImLerp(it_anim->second.rect_opacity, (boolean ? 1.0f : 0.0f), 0.15f * (1.0f - ImGui::GetIO().DeltaTime));
    it_anim->second.text_opacity = ImLerp(it_anim->second.text_opacity, (boolean ? 1.0f : hovered ? 0.5f : 0.3f), 0.07f * (1.0f - ImGui::GetIO().DeltaTime));

    window->DrawList->AddRectFilled(rect.Min, rect.Max, ImColor(1.0f, 1.0f, 1.0f, it_anim->second.element_opacity), 3.0f);
 //   window->DrawList->AddText(tab_icons, 15.0f, (rect.Min + rect.Max - label_size) / 2, ImColor(1.0f, 1.0f, 1.0f, it_anim->second.text_opacity), name);

    window->DrawList->AddRectFilled(ImVec2(rect.Max.x + 4, rect.Min.y + 6), ImVec2(rect.Max.x + 8, rect.Max.y - 6), ImColor(147 / 255.0f, 190 / 255.0f, 66 / 255.0f, it_anim->second.rect_opacity), 7.0f, ImDrawFlags_RoundCornersLeft);

    return pressed;
}

struct subtab_element {
    float element_opacity, rect_opacity, text_opacity;
};

bool elements::subtab(const char* name, bool boolean)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(name);
    const ImVec2 label_size = ImGui::CalcTextSize(name);
    ImVec2 pos = window->DC.CursorPos;

    const ImRect rect(pos, ImVec2(pos.x + 145, pos.y + 32));
    ImGui::ItemSize(ImVec4(rect.Min.x, rect.Min.y, rect.Max.x, rect.Max.y + 3), style.FramePadding.y);
    if (!ImGui::ItemAdd(rect, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held, NULL);

    static std::map <ImGuiID, subtab_element> anim;
    auto it_anim = anim.find(id);
    if (it_anim == anim.end()) {
        anim.insert({ id, { 0.0f, 0.0f, 0.0f } });
        it_anim = anim.find(id);
    }

    it_anim->second.element_opacity = ImLerp(it_anim->second.element_opacity, (boolean ? 0.04f : 0.0f), 0.09f * (1.0f - ImGui::GetIO().DeltaTime));
    it_anim->second.rect_opacity = ImLerp(it_anim->second.rect_opacity, (boolean ? 1.0f : 0.0f), 0.20f * (1.0f - ImGui::GetIO().DeltaTime));
    it_anim->second.text_opacity = ImLerp(it_anim->second.text_opacity, (boolean ? 1.0f : 0.3f), 0.07f * (1.0f - ImGui::GetIO().DeltaTime));

    window->DrawList->AddRectFilled(rect.Min, rect.Max, ImColor(1.0f, 1.0f, 1.0f, it_anim->second.element_opacity), 3.0f);
    window->DrawList->AddText(ImVec2(rect.Min.x + 15, (rect.Min.y + rect.Max.y) / 2 - label_size.y / 2), ImColor(1.0f, 1.0f, 1.0f, it_anim->second.text_opacity), name);
                                                                                                                            //rgb(104, 129, 237)
    window->DrawList->AddRectFilled(ImVec2(rect.Max.x + 5, rect.Min.y + 9), ImVec2(rect.Max.x + 8, rect.Max.y - 9), ImColor(104 / 255.0f, 129 / 255.0f, 237 / 255.0f, it_anim->second.rect_opacity), 10.0f, ImDrawFlags_RoundCornersLeft);

    return pressed;
}








///  分组标签

bool elements::begin_child_ex(const char* name, ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags flags)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* parent_window = g.CurrentWindow;

    // 设置标志位，合并一些标志
    flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_ChildWindow;
    flags |= (parent_window->Flags & ImGuiWindowFlags_NoMove);  // 继承父窗口的 NoMove 标志

    // 计算子窗口大小
    const ImVec2 content_avail = ImGui::GetContentRegionAvail();
    ImVec2 size = size_arg;
    if (size.x <= 0.0f)
        size.x = ImMax(content_avail.x + size.x, 4.0f); // 最小子窗口大小
    if (size.y <= 0.0f)
        size.y = ImMax(content_avail.y + size.y, 4.0f);

    // 设置子窗口的下一个位置
    ImGui::SetNextWindowPos(ImVec2(parent_window->DC.CursorPos.x, parent_window->DC.CursorPos.y + 34.0f)); // 根据需求调整偏移
    ImGui::SetNextWindowSize(size - ImVec2(0, 36));  // 保持一定的上部空间

     //设置窗口名称
    const char* temp_window_name;
    if (name)
        ImFormatStringToTempBuffer(&temp_window_name, NULL, "%s/%s_%08X", parent_window->Name, name, id); // 格式化名称
    else
        ImFormatStringToTempBuffer(&temp_window_name, NULL, "%s/%08X", parent_window->Name, id); // 格式化名称

    // 临时禁用边框大小
    const float backup_border_size = g.Style.ChildBorderSize;
    if (!border)
        g.Style.ChildBorderSize = 0.0f;

    // 创建子窗口
    bool ret = ImGui::Begin(temp_window_name, NULL, flags);
    g.Style.ChildBorderSize = backup_border_size;  // 恢复原始边框大小

    // 添加圆角背景
    ImDrawList* draw_list = parent_window->DrawList;
    ImVec2 pos = parent_window->DC.CursorPos;
    ImVec2 size_with_border = pos + size;
    float rounding = g.Style.WindowRounding;  // 获取圆角大小
    ImU32 col = IM_COL32(24, 24, 26, 255);   // 背景颜色

    // 绘制圆角背景
    draw_list->AddRectFilled(pos, size_with_border, col, rounding);

    // 绘制边框（如果有）
    if (border)
    {
        ImU32 border_col = IM_COL32(255, 255, 255, 30);
        draw_list->AddRect(pos, size_with_border, border_col, rounding, ImDrawFlags_RoundCornersAll, 1.0f);
    }

    // 设置子窗口的 ID
    ImGuiWindow* child_window = g.CurrentWindow;
    child_window->ChildId = id;

    return ret;
}

bool elements::begin_child(const char* str_id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();

    // 修改子窗口的样式
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 11));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16)); 

    // 调用 `begin_child_ex` 创建子窗口
    return begin_child_ex(str_id, window->GetID(str_id), size_arg, border, extra_flags | ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoMove);
}

void elements::end_child()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    IM_ASSERT(g.WithinEndChild == false);
    IM_ASSERT(window->Flags & ImGuiWindowFlags_ChildWindow);   // Mismatched BeginChild()/EndChild() calls

    g.WithinEndChild = true;

    // 如果 BeginCount > 1，表示嵌套的 Begin/End
    if (window->BeginCount > 1)
    {
        ImGui::End();
    }
    else
    {
        // 子窗口的最终大小
        ImVec2 sz = window->Size;

        // 处理自动调整子窗口大小的逻辑
        const ImVec2 avail = ImGui::GetContentRegionAvail();
        if (sz.x <= 0.0f)
            sz.x = ImMax(4.0f, avail.x);  // 使用父窗口的剩余区域
        if (sz.y <= 0.0f)
            sz.y = ImMax(4.0f, avail.y);  // 使用父窗口的剩余区域

        ImGui::End();

        // 获取父窗口
        ImGuiWindow* parent_window = g.CurrentWindow;

        // 计算子窗口的边界
        ImRect bb(parent_window->DC.CursorPos, parent_window->DC.CursorPos + sz);

        // 记录子窗口的大小
        ImGui::ItemSize(sz);
    }

    // 恢复样式和状态
    ImGui::PopStyleVar(2);
    g.WithinEndChild = false;
    g.LogLinePosY = -FLT_MAX; // 强制换行
}



