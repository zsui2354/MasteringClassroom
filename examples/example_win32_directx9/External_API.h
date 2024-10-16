//#pragma once
#ifndef EXTERNAL_API_H
#define EXTERNAL_API_H


#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <vector>



class External_API {
public:

    External_API() :hProcess(NULL), pid(0), moduleName(nullptr) {};

    External_API(DWORD pid, const wchar_t* moduleName) :pid(pid), hProcess(NULL) {};

    External_API(DWORD pid) :pid(pid), hProcess(NULL) {};

    ~External_API(){};

    HANDLE hProcess;                                    //进程句柄

    DWORD pid;                                          //进程ID

    const wchar_t* moduleName;                          //模块地址名称

    HANDLE Get_hProcess()const {
        return hProcess;
    };

};

#endif // EXTERNAL_API_H 
