
#include <windows.h>
#include "imgui.h"
#include <string>
#include <iostream>

void ExecuteCommand(const char* command) {
    // 使用 CreateProcess 启动cmd
    std::string cmd = "cmd.exe /K " + std::string(command); // /C 表示执行命令后退出
    FILE* fp = _popen(cmd.c_str(), "r");
    if (fp) {
        char buffer[128];
        while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
            // 你可以在此将输出展示到ImGui控制台的文本区域
            std::cout << buffer; // 输出到控制台
        }
        fclose(fp);
    }
}

void RenderConsole() {
            // 显示命令输出
            ImGui::BeginChild("Console Output", ImVec2(0, -30), true);
            ImGui::Text("Command output will appear here...");
            ImGui::EndChild();

            // 输入框
            static char command[256] = "";
            if (ImGui::InputText("Command", command, IM_ARRAYSIZE(command), ImGuiInputTextFlags_EnterReturnsTrue)) {
                // 处理命令
                ExecuteCommand(command);
                memset(command, 0, sizeof(command)); // 清空输入框
            }
}
