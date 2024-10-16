#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <vector>
#include "lib_function.h"
#include "addressOffset.h"



extern class External_API {
public:

    HANDLE hProcess;                                    //进程句柄

    DWORD pid =0;                                          //进程ID

    const wchar_t* moduleName;                          //模块地址名称

    HANDLE Get_hProcess() const {
        return hProcess;
    }

    //enum class PROCESS_ACCESS_STATUS : DWORD {
    //    ALL = PROCESS_ALL_ACCESS,                       //进程全部权限

    //    Create_Thread = PROCESS_CREATE_THREAD,          //在进程中创建线程时必需的。
    //};


    External_API():hProcess(NULL), pid(0), moduleName(nullptr) {

    }

    External_API(DWORD pid , const wchar_t* moduleName):pid(pid), hProcess(NULL) {
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
        this->moduleName = moduleName;
    }

    External_API(DWORD pid) :pid(pid), hProcess(NULL) {  
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    }

    ~External_API() {
        if (hProcess != NULL) {
            CloseHandle(hProcess);
        }
    }
};
