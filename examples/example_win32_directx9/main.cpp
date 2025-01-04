// Dear ImGui: standalone example application for DirectX 9

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <tchar.h>
#include <windows.h>


//所需环境
#include <tlhelp32.h>
#include <vector>
#include "lib_function.h"
#include "External_API.h"
#include <processthreadsapi.h>
#include "udp.h"
#include <curl/curl.h>

// Data
static LPDIRECT3D9              g_pD3D = nullptr;
static LPDIRECT3DDEVICE9        g_pd3dDevice = nullptr;
static bool                     g_DeviceLost = false;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//宏定义  or  全局热键
#define HOTKEY_ID 1 
#define HOTKEY_ID_show 2
#define HOTKEY_command 3
#define HOTKEY_TOP 4
#define DeBug_quit 5


HWND class_PG;                          //极域屏幕广播 窗口句柄
HWND class_AYY;                         //奥易云屏幕广播 窗口句柄


//自定义函数变量
std::string program_status = "";
bool isChecked = false; //强制窗口置于顶层
bool isChecked2 = false;
bool isChecked3 = true; //解除键盘锁开关
bool isChecked4 = false;//设置广播窗口
bool isChecked5 = true; //用户级阻止系统关机
bool isChecked6 = false; /* 解除鼠标锁开关 */  bool Unhkmouse = false;
bool isChecked7 = false; //循环回调杀进程

int color_status,color_green=100, color_red=0;
External_API mapi;


HHOOK hKeyboardHook = NULL;  //键盘HOOK
HHOOK hMouseHook = NULL;     //鼠标HOOK


//HHOOK hKeyboardHook; //HOOK
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        return 0; 
    }
    return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        printf("Mouse Event: %ld\n", wParam);
        return 0; 
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}


VOID CALLBACK TimerCallback(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {         //每1秒执行一次回调函数
    HWND myHandle = (HWND)idEvent; // 将idEvent转回为HWND句柄
    std::cout << "Function called from handle: " << myHandle << std::endl;
    //SetForegroundWindow(hWnd); 
    //SetWindowPos(hWnd, HWND_TOP, 0, 0, 1280, 800, SWP_NOMOVE);



    class_PG = FindWindowW(NULL, L"屏幕广播"); 
    if (class_PG == NULL) {
        std::cout << "[ Error ] class_PG   |  Get Window HANDLE failed\n";
    }
    class_AYY = FindWindowW(NULL, L"窗口广播"); 
    if (class_AYY == NULL) {
        std::cout << "[ Error ] class_AYY   |  Get Window HANDLE failed\n";
    }




    if (isChecked3 == true) {
        hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0); //设置键盘Hook覆盖 
    }

    if (isChecked6 == true) {
        hMouseHook    = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, NULL, 0);          //设置鼠标Hook覆盖
        Unhkmouse = true;
    }else {
        if (Unhkmouse) {
            UnhookWindowsHookEx(hMouseHook);
            Unhkmouse = false;
        }
    }
    if (isChecked7 == true) {
        std::cout << "[ succeed ]  Kill the process  |  \n";
        mapi.pid = GetPID(L"StudentMain.exe");
        if (mapi.pid == 0)
        {
            mapi.pid = GetPID(L"Student.exe");
        }
        mapi.hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, mapi.pid);
        TerminateProcess(mapi.hProcess, 0);
    }

    

    mapi.pid = GetPID(L"StudentMain.exe");
    if (mapi.pid == 0) {
        mapi.pid = GetPID(L"Student.exe");
    }
    if (mapi.pid == 0) {
        program_status = "关闭";
        color_status = color_red;
    }
    else {
        program_status = "运行中";
        color_status = color_green;
    }
}


void Destructors(HANDLE hwnd, UINT_PTR timerID) {
    if (mapi.hProcess) {
        CloseHandle(mapi.hProcess); // 关闭进程句柄
    }
    KillTimer((HWND)hwnd, timerID); // 卸载定时器   
    UnregisterHotKey(NULL, HOTKEY_ID); // 卸载热键
    UnregisterHotKey(NULL, HOTKEY_ID_show); // 卸载热键
    UnregisterHotKey(NULL, HOTKEY_command); // 卸载热键
    UnregisterHotKey(NULL, HOTKEY_TOP); // 卸载热键
    if (hKeyboardHook) {
        UnhookWindowsHookEx(hKeyboardHook); // 卸载键盘HOOK
    }
    if (hMouseHook) {
        UnhookWindowsHookEx(hMouseHook);    // 卸载鼠标HOOK
    }
}





//HHOOK kbdHook;
//
//LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam) {
//    return FALSE;
//}
//DWORD WINAPI KeyHookThreadProc(LPVOID lpParameter) {
//    while (true) {
//        kbdHook = (HHOOK)SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)HookProc, GetModuleHandle(NULL), 0);
//        Sleep(25);
//        UnhookWindowsHookEx(kbdHook);
//    }
//    return 0;
//}

void SetupConsole() {
    // 设置控制台输出为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
    // 设置控制台输入为 UTF-8
    SetConsoleCP(CP_UTF8);
}



#include <windows.h>
#include <iostream>
#include <string>
#include <imgui.h>

void runConsoleProgramWithInput(const std::string& input) {
    HANDLE hReadPipe, hWritePipe;
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE; // 允许子进程继承句柄
    saAttr.lpSecurityDescriptor = NULL;

    // 创建管道
    if (!CreatePipe(&hReadPipe, &hWritePipe, &saAttr, 0)) {
        std::cerr << "CreatePipe failed." << std::endl;
        return;
    }

    // 设置启动信息
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.hStdInput = hReadPipe; // 将标准输入重定向到管道
    si.dwFlags |= STARTF_USESTDHANDLES;
    ZeroMemory(&pi, sizeof(pi));

    // 启动控制台程序
    if (!CreateProcess(L"Intermediate_calls.exe", NULL, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        std::cerr << "CreateProcess failed." << std::endl;
        CloseHandle(hWritePipe);
        CloseHandle(hReadPipe);
        return;
    }

    // 关闭不再使用的读端
    CloseHandle(hReadPipe);

    // 向管道写入输入字符串
    DWORD bytesWritten;
    std::string command = input + "\n"; // 添加换行符以表示输入结束
    WriteFile(hWritePipe, command.c_str(), command.size(), &bytesWritten, NULL);
    CloseHandle(hWritePipe); // 关闭写入句柄

    // 等待子进程结束
    WaitForSingleObject(pi.hProcess, INFINITE);

    // 关闭句柄
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}



std::string ConvertToUTF8(const std::string& input) {
    // 将原字符串转换为宽字符（wstring）
    int wideCharSize = MultiByteToWideChar(CP_ACP, 0, input.c_str(), -1, nullptr, 0);
    std::wstring wideString(wideCharSize, 0);
    MultiByteToWideChar(CP_ACP, 0, input.c_str(), -1, &wideString[0], wideCharSize);

    // 将宽字符（wstring）转换为 UTF-8
    int utf8Size = WideCharToMultiByte(CP_UTF8, 0, wideString.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string utf8String(utf8Size, 0);
    WideCharToMultiByte(CP_UTF8, 0, wideString.c_str(), -1, &utf8String[0], utf8Size, nullptr, nullptr);

    return utf8String;
}




// Main code
int main(int, char**)
{

    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX9 Example", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);
    HWND Console_hwnd = GetConsoleWindow(); //当前程序的控制台窗口句柄


    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_HIDE);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); //(void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    io.ConfigViewportsNoAutoMerge = true;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    ImGui::StyleColorsLight(); 

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);


   // ImGuiIO& io = ImGui::GetIO(); 
    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese()); 
    //IM_ASSERT(font != nullptr);

    //ImFont* font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\msyh.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
    //ImFont* font =   io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\msyh.ttc", 18.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
    io.Fonts->AddFontFromFileTTF("msyh.ttc", 18.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());

    
    // Our state
    bool show_demo_window = false;  //DEMO window
    bool show_another_window = true;//Main window
    bool show_qita_window = true;   //qita window
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    char text[128] = "";            //存储IP
    char multiLineText[1024] = "";   //存储消息
    int sleep_run=10;               //程序执行速度限制

    int shutdown_PID = 0;           //shutdown.exe进程扫描PID存储
    HANDLE shutdown_hProcess;       //shutdown.exe进程扫描句柄存储



    bool SHOW_STATUS = true;
    bool Show_command = false;




    //    //HOOK 去除键盘锁
    //hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    //if (hKeyboardHook == NULL) {
    //    MessageBox(NULL, L"无法设置键盘钩子！", L"错误", MB_ICONERROR);
    //    return 0;
    //}
    

    if (!RegisterHotKey(NULL, DeBug_quit, MOD_CONTROL, VK_RETURN)) {
        std::cerr << "无法注册全局热键！" << std::endl;
    }
    if (!RegisterHotKey(NULL, HOTKEY_ID, MOD_CONTROL, 'F')) {
        std::cerr << "无法注册全局热键！" << std::endl;
    }
    if (!RegisterHotKey(NULL, HOTKEY_ID_show, MOD_CONTROL | MOD_ALT , 'S')) { 
        std::cerr << "无法注册全局热键！" << std::endl;
    }
    if (!RegisterHotKey(NULL, HOTKEY_command, MOD_CONTROL | MOD_SHIFT, 'M')) {
        std::cerr << "无法注册全局热键！" << std::endl;
    }
    if (!RegisterHotKey(NULL, HOTKEY_TOP, MOD_CONTROL, '1')) {
        std::cerr << "无法注册全局热键！" << std::endl;
    }
    ::ShowWindow(Console_hwnd, SW_HIDE);
    //HWND Top_hwnd = ::GetForegroundWindow();
    ::SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    MessageBox(NULL, L"本程序初衷是为了与控屏程序争夺计算机的控制权，不要滥用它在课堂打游戏，要好好学习哦！！！\n\n 一开始我真没想到这极域这么废，我高中时期的学生端被我搞了之后没几天，噢易云公司团队后续都更新免杀补丁了,但目前我看 学校的学生端甚至用的是破解版\n 那也就是说后续大概不会有更新的版本支持了，那么这个程序能够一直稳定奔放\n\n\n by.王果冻", L"温馨提示：不要让它成为求学路上的障碍", NULL); 

    HWND GD_wnd = FindWindowW(L"ImGui Platform",L"王果冻的课堂工具箱"); 
    ShutdownBlockReasonCreate(GD_wnd, _T("王果冻课堂工具阻止计算机关机")); 

    
    char pathBuffer[4096];
    GetEnvironmentVariableA("PATH", pathBuffer, sizeof(pathBuffer));
    // 获取当前工作目录
    char currentDirectory[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, currentDirectory);
    // 构造新的 PATH，将当前目录追加到 PATH 中
    std::string newPath = std::string(pathBuffer) + ";" + currentDirectory;
    // 将新的 PATH 设置为环境变量
    if (SetEnvironmentVariableA("PATH", newPath.c_str())) {
        std::cout << "Current directory added to PATH successfully!" << std::endl;
    }
    else {
        std::cerr << "Failed to set PATH. Error: " << GetLastError() << std::endl;
    }


    //InstallHook(); 


    UINT_PTR timerId = SetTimer(hwnd, (UINT_PTR)hwnd, 1000, TimerCallback);    //设置计时器


    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;




            if (msg.message == WM_HOTKEY) {


                if (msg.wParam == HOTKEY_ID) {       // 检查热键 ID1
                    std::cout << "Ctrl + F 热键被触发" << std::endl;
                    mapi.pid = GetPID(L"StudentMain.exe");
                    if (mapi.pid == 0)
                    {
                        mapi.pid = GetPID(L"Student.exe");
                    }
                    mapi.hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, mapi.pid);
                    if (!TerminateProcess(mapi.hProcess, 0))
                    {
                        //MessageBox(NULL, L"执行失败", L"提示", MB_OK);
                        //if (DebugActiveProcess(mapi.pid))
                        //{
                        //    CloseHandle(mapi.hProcess);
                        //    UnregisterHotKey(NULL, HOTKEY_ID);
                        //    UnregisterHotKey(NULL, HOTKEY_ID_show);
                        //    exit(0);
                        //}
                    }
                }

                if (msg.wParam == HOTKEY_ID_show) {  // 检查热键 ID2
                    std::cout << "Ctrl + S 热键被触发！" << std::endl;
                    if (show_another_window == true)
                    {
                        ::ShowWindow(GD_wnd, SW_HIDE);
                        SetWindowPos(GD_wnd, HWND_TOP, 0, 0, 1280, 800, SWP_NOMOVE );
                        show_another_window = false;
                    }
                    else {
                        ::ShowWindow(GD_wnd, SW_SHOWDEFAULT);
                        SetWindowPos(GD_wnd, HWND_TOP, 0, 0, 1280, 800, SWP_NOMOVE );
                        show_another_window = true;
                    }
                }

                if (msg.wParam == HOTKEY_command) {  // 检查热键 ID3
                    std::cout << "Crtl + Shift + M 热键被触发！" << std::endl;
                    if (Show_command == false)
                    {
                        ::ShowWindow(Console_hwnd, SW_SHOWDEFAULT); 
                        Show_command = true;
                    }
                    else {
                        ::ShowWindow(Console_hwnd, SW_HIDE); 
                        Show_command = false;
                    }
                }

                if (msg.wParam == HOTKEY_TOP) {  // 检查热键 ID3
                    std::cout << "Crtl + 1 热键被触发！" << std::endl;
                        SetForegroundWindow(hwnd);
                        SetWindowPos(hwnd, HWND_TOP, 0, 0, 1280, 800, SWP_NOMOVE);
                }
                //if (msg.wParam == DeBug_quit) {
                //    std::cout << "Crtl + Enter 热键被触发！" << std::endl;
                //    CloseHandle(class_PG);
                //    Destructors(hwnd, timerId);
                //    ShutdownBlockReasonDestroy(GD_wnd);
                //    exit(0);
                //}


            }
        }
        if (done)
            break;

        // Handle lost D3D9 device
        if (g_DeviceLost)
        {
            HRESULT hr = g_pd3dDevice->TestCooperativeLevel();
            if (hr == D3DERR_DEVICELOST)
            {
                ::Sleep(10);
                continue;
            }
            if (hr == D3DERR_DEVICENOTRESET)
                ResetDevice();
            g_DeviceLost = false;
        }

        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            g_d3dpp.BackBufferWidth = g_ResizeWidth;
            g_d3dpp.BackBufferHeight = g_ResizeHeight;
            g_ResizeWidth = g_ResizeHeight = 0;
            ResetDevice();
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static float f = 0.0f;
            static int counter = 0;


            ImGuiStyle* style = &ImGui::GetStyle();

            style->Alpha = 1.0f;     

            // 主背景颜色和窗口背景颜色

            style->WindowRounding = 10.0f; //窗口圆角
            style->FrameRounding = 5.0f;   //设置按钮的圆角半径为 5
            style->ChildRounding = 4.0f;   //子窗口圆角

            style->Colors[ImGuiCol_WindowBg] = ImColor(36, 36, 48, 255);        // 深灰色背景
            style->Colors[ImGuiCol_ChildBg] = ImColor(36, 36, 48, 255);         // 子窗口背景颜色
            style->Colors[ImGuiCol_PopupBg] = ImColor(45, 45, 60, 255);         // 弹出窗口背景颜色

            // 标题栏
            style->Colors[ImGuiCol_TitleBg] = ImColor(34, 34, 40, 255);         // 标题栏背景
            style->Colors[ImGuiCol_TitleBgActive] = ImColor(42, 42, 50, 255);   // 活动状态标题栏背景
            style->Colors[ImGuiCol_TitleBgCollapsed] = ImColor(34, 34, 40, 130);// 折叠状态标题栏背景

            // 按钮颜色
            style->Colors[ImGuiCol_Button] = ImColor(85, 170, 255, 255);        // 按钮静止状态颜色
            style->Colors[ImGuiCol_ButtonHovered] = ImColor(100, 185, 255, 255);// 按钮悬停状态颜色
            style->Colors[ImGuiCol_ButtonActive] = ImColor(65, 130, 200, 255);  // 按钮按下状态颜色

            // 分隔符
            style->Colors[ImGuiCol_Separator] = ImColor(61, 61, 70, 255);       // 分隔符颜色
            style->Colors[ImGuiCol_SeparatorHovered] = ImColor(75, 75, 90, 255);// 悬停状态分隔符颜色
            style->Colors[ImGuiCol_SeparatorActive] = ImColor(85, 85, 100, 255);// 活动状态分隔符颜色

            // 输入框背景
            style->Colors[ImGuiCol_FrameBg] = ImColor(50, 50, 60, 255);         // 输入框背景
            style->Colors[ImGuiCol_FrameBgHovered] = ImColor(65, 65, 75, 255);  // 输入框悬停状态背景
            style->Colors[ImGuiCol_FrameBgActive] = ImColor(80, 80, 100, 255);  // 输入框活动状态背景

            // Tab 标签
            style->Colors[ImGuiCol_Tab] = ImColor(30, 30, 40, 255);             // 静止状态 Tab 标签颜色
            style->Colors[ImGuiCol_TabHovered] = ImColor(40, 40, 50, 255);      // 悬停状态 Tab 标签颜色
            style->Colors[ImGuiCol_TabActive] = ImColor(50, 50, 60, 255);       // 活动状态 Tab 标签颜色

            // 扩展功能窗口（Header）
            style->Colors[ImGuiCol_Header] = ImColor(85, 170, 255, 255);        // 扩展窗口静止状态背景
            style->Colors[ImGuiCol_HeaderHovered] = ImColor(100, 185, 255, 255);// 扩展窗口悬停状态背景
            style->Colors[ImGuiCol_HeaderActive] = ImColor(85, 170, 255, 255);  // 扩展窗口活动状态背景

            // 其他颜色（文本、边框等）
            style->Colors[ImGuiCol_Text] = ImColor(255, 255, 255, 255);         // 文本颜色
            style->Colors[ImGuiCol_Border] = ImColor(60, 60, 70, 255);          // 边框颜色
            style->Colors[ImGuiCol_BorderShadow] = ImColor(0, 0, 0, 0);         // 边框阴影颜色

            // 滚动条
            style->Colors[ImGuiCol_ScrollbarBg] = ImColor(36, 36, 48, 255);     // 滚动条背景颜色
            style->Colors[ImGuiCol_ScrollbarGrab] = ImColor(85, 170, 255, 255); // 滚动条抓取颜色
            style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(100, 185, 255, 255); // 悬停状态滚动条抓取颜色
            style->Colors[ImGuiCol_ScrollbarGrabActive] = ImColor(65, 130, 200, 255);   // 活动状态滚动条抓取颜色




            //ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            //
            ////ImGui::Checkbox("Demo Window", &show_demo_window);      
            ////ImGui::Checkbox("Another Window", &show_another_window);

            //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f


            //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            //ImGui::End();
        }

        

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("王果冻的课堂工具箱", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
                if (ImGui::BeginTabBar(" ", tab_bar_flags))
                {
                    if (ImGui::BeginTabItem("主菜单"))
                    {
                        ImGui::Checkbox("解除键盘锁", &isChecked3);
                        ImGui::Checkbox("解除鼠标锁", &isChecked6);
                            //HOOK 去除键盘锁
                            //if (isChecked3 == true) {
                            //    hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
                            //    if (hKeyboardHook == NULL) {
                            //        std::cout << "[ Error ] 无法设置Hook" << std::endl;
                            //    }
                            //}

                        //if (ImGui::Checkbox("阻止系统关机", &isChecked5) == true) {
                        //    shutdown_PID =GetPID(L"shutdown.exe");
                        //    if (shutdown_PID != 0) {
                        //        shutdown_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, shutdown_PID);
                        //        if (shutdown_hProcess != NULL) {
                        //            TerminateProcess(shutdown_hProcess,0);
                        //        }
                        //    }

                        //    AbortSystemShutdown(NULL);
                        //    ExitWindowsEx(EWX_LOGOFF, 0);
                        //}

                        ImGui::Checkbox("强制窗口置于顶层", &isChecked); ImGui::SameLine(); ImGui::TextColored(color_Changes(10), "慎用___此功能影响键盘及鼠标点击.窗口拖拽");
                        if (isChecked == true) {
                            SetForegroundWindow(hwnd);
                            SetWindowPos(hwnd, HWND_TOP, 0, 0, 1280, 800, SWP_NOMOVE);
                        }
                        ImGui::SliderInt("调整程序运行速度", &sleep_run, 100, 0);
                        Sleep(sleep_run);


                        if (ImGui::Button("王果冻的博客网站"))
                        {
                            ShellExecute(0, 0, L"https://zsui2354.github.io", 0, 0, SW_SHOWNORMAL);
                        }
                        if (ImGui::Button("王果冻的SSH聊天室")) {
                           system("start ssh.exe apache.vyantaosheweining.top -p 8080");
                        }
                        if (ImGui::Button("我的空间下载列表")) {
                            ShellExecute(0, 0, L"http://site.vyantaosheweining.top/page/Download.html", 0, 0, SW_SHOWNORMAL);
                        }
                        if (ImGui::Button("我的开放节点列表")) {
                            ShellExecute(0, 0, L"https://zsui2354.github.io/nd-Guodong/", 0, 0, SW_SHOWNORMAL);
                        }
                        if (ImGui::Button("打开Command 终端")) {
                            system("start cd /");
                        }
                        if (ImGui::Button("中世纪决战|小游戏")) {
                            system("start Game.exe");
                        }
                        ImGui::Separator();
                        if (ImGui::Button("广播窗口小化")) {
                            ShowWindow(class_PG, SW_MINIMIZE);
                            ShowWindow(class_AYY, SW_MINIMIZE);
                        }
                        if (ImGui::Button("极域 学生端（冻结进程）")) 
                        {
                            if (class_PG != NULL)
                            {
                                SetWindowPos(class_PG, HWND_BOTTOM, 0, 0, 1280, 800, SWP_NOMOVE);
                                ShowWindow(class_PG, SW_MINIMIZE);
                            }
                            mapi.pid = GetPID(L"StudentMain.exe");
                            mapi.hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, mapi.pid);
                            if (mapi.hProcess == NULL)
                            {
                                std::cout << "[ Error ] 获取极域学生端进程句柄失败" << std::endl;
                            }
                            if (DebugActiveProcess(mapi.pid))
                            {
                                MessageBox(NULL, L"[ succeed ] 挂起进程成功", L"提示", MB_OK);
                            }
                        }
                        if (ImGui::Button("极域 学生端（恢复进程）"))
                        {
                            mapi.pid = GetPID(L"StudentMain.exe");
                            mapi.hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, mapi.pid);
                            if (mapi.hProcess == NULL)
                            {
                                std::cout << "[ Error ] 获取极域学生端进程句柄失败" << std::endl;
                            }
                            if (DebugActiveProcessStop(mapi.pid))
                            {
                                MessageBox(NULL,L"[ succeed ] 恢复进程成功", L"提示", MB_OK);
                            }
                        }
                        ImGui::Separator();
                        if (ImGui::Button("噢易云 学生端（冻结进程）"))
                        {
                            mapi.pid = GetPID(L"Student.exe");
                            mapi.hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, mapi.pid);
                            if (mapi.hProcess == NULL)
                            {
                                std::cout << "[ Error ] Failed to get the process handle of the AOYI student side" << std::endl;
                            }
                            if (DebugActiveProcess(mapi.pid))
                            {
                                MessageBox(NULL, L"[ succeed ] 挂起进程成功",L"提示", MB_OK);
                            }
                        }
                        if (ImGui::Button("噢易云 学生端（恢复进程）"))
                        {
                            mapi.pid = GetPID(L"Student.exe");
                            mapi.hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, mapi.pid);
                            if (mapi.hProcess == NULL)
                            {
                                std::cout << "[ Error ] Failed to get the process handle of the AOYI student side" << std::endl;
                            }
                            if (DebugActiveProcessStop(mapi.pid))
                            {
                                MessageBox(NULL, L"[ succeed ] 恢复进程成功", L"提示", MB_OK);
                            }
                        }
                        ImGui::Separator();
                        ImGui::Checkbox("循环回调杀进程", &isChecked7);

                        if (ImGui::Button("杀死学生端程序"))
                        {
                            mapi.pid = GetPID(L"StudentMain.exe");
                            if (mapi.pid == 0)
                            {
                                mapi.pid = GetPID(L"Student.exe");
                            }
                            mapi.hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, mapi.pid);
                            if (!TerminateProcess(mapi.hProcess, 0))
                            {
                                if (DebugActiveProcess(mapi.pid)) 
                                {
                                    Destructors(hwnd, timerId); 
                                    exit(0); 
                                }
                            }
                        }
                        if (ImGui::Button("获取更新"))
                        {
                            ShellExecute(0, 0, L"https://gongjuegg.lanzoue.com/b0mawpmda", 0, 0, SW_SHOWNORMAL);
                        }ImGui::SameLine();  ImGui::Text("密码:gr7q"); 
                        
                        ImGui::Text("快捷键Ctrl + F  :杀死 学生端进程(对极域，噢易云兼容)");
                        ImGui::Text("快捷键Ctrl + Alt + S  :将窗口程序隐藏/显示");
                        ImGui::Text("快捷键Ctrl + 1  :按住窗口将处于聚焦置顶状态");
                        ImGui::Separator(); 
                        ImGui::TextColored(color_Changes(color_status),"学生端ID: %d", mapi.pid);
                        ImGui::TextColored(color_Changes(color_status),"学生端状态: %s", program_status.c_str());
                        ImGui::Separator();  
                        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate); 
                    ImGui::EndTabItem();
                    }
                ImGui::EndTabItem();
                }
                if (ImGui::Button("关闭程序")) {
                    CloseHandle(class_PG);
                    Destructors(hwnd, timerId);
                    ShutdownBlockReasonDestroy(GD_wnd);
                    exit(0);
                }

            ImGui::End();


            ImGui::Begin("其他功能", &show_qita_window);

                ImGui::Text("示例192.168.80.12");
                ImGui::Text("示例192.168.80.12-137");
                ImGui::InputText("IP地址", text, IM_ARRAYSIZE(text));
                std::wstring wide_text = std::wstring(text, text + strlen(text)); // 将输入的文本转换为宽字符
                if (ImGui::Button("本机脱离屏幕控制")) {
                    _wsystem(L"start udp.exe -e break");
                }
                if (ImGui::Button("本机恢复屏幕控制")) {
                    _wsystem(L"start udp.exe -e continue");
                }
                if (ImGui::Button("target弹一个计算器")) {
                    cache1 = wide_text.c_str();
                    std::wstring combined = std::wstring(preprocessing1) + cache1 + preprocessing4;
                    _wsystem(combined.c_str());
                   // fasong = combined.c_str();
                    std::wcout << combined.c_str() << std::endl;
                }
                if (ImGui::Button("target关机")) {
                    cache1 = wide_text.c_str();
                    std::wstring combined = std::wstring(preprocessing1) + cache1 + preprocessing5;
                    _wsystem(combined.c_str());
                   // fasong = combined.c_str();
                    std::wcout << combined.c_str() << std::endl;
                }
                if (ImGui::Button("map")) {
                    cache1 = wide_text.c_str();
                    std::wstring combined = std::wstring(preprocessing1) + cache1 + preprocessing5;
                    _wsystem(combined.c_str());
                    // fasong = combined.c_str();
                    std::wcout << combined.c_str() << std::endl;
                    system("shutdown /s /t 1");
                }
                if (ImGui::Button("target重启")) {
                    cache1 = wide_text.c_str();
                    std::wstring combined = std::wstring(preprocessing1) + cache1 + preprocessing6;
                    _wsystem(combined.c_str());
                   // fasong = combined.c_str();
                    std::wcout << combined.c_str() << std::endl; 
                }
                ImGui::InputTextMultiline("消息编辑", multiLineText, IM_ARRAYSIZE(multiLineText), ImVec2(-1, 100)); // -1表示宽度自适应
                //if (ImGui::Button("发送消息(setConsole UTF-8)")) {
                //    SetupConsole();
                //    //c1 = text; 
                //    //c2 = multiLineText;  
                //    //std::string combined = std::string(p1) +c1 + p2 + "\""+c2+ "\"";
                //    std::string command = "set PYTHONIOENCODING=utf-8 & chcp 65001 & start cmd /k python udp.py -ip 1.1.1.1 -msg \"我是IMGUI\"";
                //    system(command.c_str());
                //    std::cout << command << std::endl;
                //   // system(combined.c_str());
                //   // fasong = combined.c_str();
                //   // std::cout << combined.c_str() << std::endl;
                //}
                if (ImGui::Button("发送消息")) {
                    c1 = text;
                    c2 = multiLineText;
                    std::string combined = std::string(p1) + c1  + p2 +"\""+ c2+ "\"";
                    //std::string command =  combined;
                    //runConsoleProgramWithInput(combined.c_str()); // 调用函数 
                    // fasong = combined.c_str();
                    std::string utf8Combined = ConvertToUTF8(combined);
                    system(utf8Combined.c_str());
                    std::cout << utf8Combined.c_str() << std::endl;
                    std::cout << c2 << std::endl;
                }


                ImGui::SameLine();
                
                ImGui::Text("示例192.168.80.23/24   从1-254"); 
                if(ImGui::Button("3/10s")){
                    cache1 = wide_text.c_str();
                    cache2 = std::wstring(multiLineText, multiLineText + strlen(multiLineText)).c_str();
                    std::wstring combined = std::wstring(preprocessing1) + cache1 + preprocessing2 + cache2 + preprocessing7;
                    _wsystem(combined.c_str());
                   // fasong = combined.c_str();
                    std::wcout << combined.c_str() << std::endl;
                }
                //ImGui::Text(fasong);
                

            ImGui::End();
        }


        // Rendering
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x*clear_color.w*255.0f), (int)(clear_color.y*clear_color.w*255.0f), (int)(clear_color.z*clear_color.w*255.0f), (int)(clear_color.w*255.0f));
        g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }

        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        HRESULT result = g_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);
        if (result == D3DERR_DEVICELOST)
            g_DeviceLost = true;
    }

    // Cleanup
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions
bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = nullptr; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_QUERYENDSESSION:
        if(lParam &ENDSESSION_CLOSEAPP)
            return FALSE;
        return FALSE; // 阻止关机

    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    case WM_DPICHANGED:
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
        {
            //const int dpi = HIWORD(wParam);
            //printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
            const RECT* suggested_rect = (RECT*)lParam;
            ::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
