//#pragma once
#ifndef ADDRESS_OFFSET_H
#define ADDRESS_OFFSET_H 

#include <Windows.h>
#include <vector>
#include <tlhelp32.h>



extern DWORD_PTR roomLimit_baseAddressOffset;				//房间人数
extern std::vector<DWORD> roomLimit_offsets;
extern DWORD_PTR roomLimit_finalAddress;
extern int roomLimit;

extern DWORD_PTR Flying_baseAddressOffset;                  //飞天状态 1正常 5飞天
extern std::vector<DWORD> Flying_offsets;
extern DWORD_PTR Flying_finalAddress;
extern int Flying;

extern DWORD_PTR Unlimitedpower_baseAddressOffset;          //自身电池电量
extern std::vector<DWORD> Unlimitedpower_offsets;
extern DWORD_PTR Unlimitedpower_finalAddress;
extern int Unlimitedpower;

extern DWORD_PTR Zpos_baseAddressOffset;					//Zpos 坐标
extern std::vector<DWORD> Zpos_offsets;
extern DWORD_PTR Zpos_finalAddress;
extern float Zpos;

extern DWORD_PTR Xpos_baseAddressOffset;					//Xpos 坐标
extern std::vector<DWORD> Xpos_offsets;
extern DWORD_PTR Xpos_finalAddress;
extern float Xpos;

extern DWORD_PTR Ypos_baseAddressOffset;					//Ypos 坐标
extern std::vector<DWORD> Ypos_offsets;
extern DWORD_PTR Ypos_finalAddress;
extern float Ypos;

extern DWORD_PTR witch_BaseAddress;							//右内 女巫艾格尼丝	房间电源 
extern std::vector<DWORD> witch_offsets;
extern DWORD_PTR witch_finalAddress;
extern int witch;

extern DWORD_PTR Joey_BaseAddress;							//右内 乔伊			房间电源
extern std::vector<DWORD> Joey_offsets;
extern DWORD_PTR Joey_finalAddress;
extern int Joey;

extern DWORD_PTR Karen_BaseAddress;							//右外 凯伦			房间电源
extern std::vector<DWORD> Karen_offsets;
extern DWORD_PTR Karen_finalAddress;
extern int Karen;

extern DWORD_PTR Quinn_BaseAddress;							//右外 奎恩			房间电源
extern std::vector<DWORD> Quinn_offsets;
extern DWORD_PTR Quinn_finalAddress;
extern int Quinn;

extern DWORD_PTR Kit_BaseAddress;							//左外 吉特			房间电源
extern std::vector<DWORD> Kit_offsets;
extern DWORD_PTR Kit_finalAddress;
extern int Kit;

extern DWORD_PTR Amelia_BaseAddress;						//左外 艾米利亚		房间电源
extern std::vector<DWORD> Amelia_offsets;
extern DWORD_PTR Amelia_finalAddress;
extern int Amelia;

extern DWORD_PTR Bouli_BaseAddress;							//左内 布利			房间电源
extern std::vector<DWORD> Bouli_offsets;
extern DWORD_PTR Bouli_finalAddress;
extern int Bouli;

extern DWORD_PTR Venusin_BaseAddress;						//左内 维妮辛		房间电源
extern std::vector<DWORD> Venusin_offsets;
extern DWORD_PTR Venusin_finalAddress;
extern int Venusin;

extern DWORD_PTR incinerator_BaseAddress;					//焚烧炉电源
extern std::vector<DWORD> incinerator_offsets;
extern DWORD_PTR incinerator_finalAddress;
extern int incinerator;

extern DWORD_PTR unit1_BaseAddress;							//小单元电源1
extern std::vector<DWORD> unit1_offsets;
extern DWORD_PTR unit1_finalAddress;
extern int unit1;

extern DWORD_PTR unit2_BaseAddress;							//小单元电源2
extern std::vector<DWORD> unit2_offsets;
extern DWORD_PTR unit2_finalAddress;
extern int unit2;


	/// <summary>
	/// 二楼电源
	/// </summary>
extern DWORD_PTR hall_BaseAddress;							//大厅电源
extern std::vector<DWORD> hall_offsets;
extern DWORD_PTR hall_finalAddress;
extern int hall;

extern DWORD_PTR toilet_BaseAddress;						//卫生间电源
extern std::vector<DWORD> toilet_offsets;
extern DWORD_PTR toilet_finalAddress;
extern int toilet;

extern DWORD_PTR diversion_BaseAddress;						//娱乐房电源
extern std::vector<DWORD> diversion_offsets;
extern DWORD_PTR diversion_finalAddress;
extern int diversion;

extern DWORD_PTR Officearea1_BaseAddress;					//办公区1电源
extern std::vector<DWORD> Officearea1_offsets;
extern DWORD_PTR Officearea1_finalAddress;
extern int Officearea1;

extern DWORD_PTR Officearea2_BaseAddress;					//办公区2电源
extern std::vector<DWORD> Officearea2_offsets;
extern DWORD_PTR Officearea2_finalAddress;
extern int Officearea2;

extern DWORD_PTR Officearea3_BaseAddress;					//办公区3电源
extern std::vector<DWORD> Officearea3_offsets;
extern DWORD_PTR Officearea3_finalAddress;
extern int Officearea3;

extern DWORD_PTR warehouse_BaseAddress;						//仓库电源
extern std::vector<DWORD> warehouse_offsets;
extern DWORD_PTR warehouse_finalAddress;
extern int warehouse;

extern DWORD_PTR Ghosthouse_BaseAddress;					//鬼房电源
extern std::vector<DWORD> Ghosthouse_offsets;
extern DWORD_PTR Ghosthouse_finalAddress;
extern int Ghosthouse;

extern DWORD_PTR Undergroundpassage_BaseAddress;			//地下通道电源
extern std::vector<DWORD> Undergroundpassage_offsets;
extern DWORD_PTR Undergroundpassage_finalAddress;
extern int Undergroundpassage;

extern DWORD_PTR cleanchannel_BaseAddress;					//净化通道电源
extern std::vector<DWORD> cleanchannel_offsets;
extern DWORD_PTR cleanchannel_finalAddress;
extern int cleanchannel;


extern DWORD_PTR GetModuleBaseAddress(DWORD pid, const wchar_t* moduleName);    // 获取模块基地址

extern DWORD_PTR ReadMultiLevelPointer(HANDLE hProcess, DWORD_PTR baseAddress, const std::vector<DWORD>& offsets);  // 读取多级指针的值

#endif // ADDRESS_OFFSET_H
