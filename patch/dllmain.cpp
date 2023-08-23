#define _CRT_SECURE_NO_WARNINGS
// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include<Windows.h>
#include"winhook/hookcode.h"
#include"nlohmann/json.hpp"
#include<fstream>
#include<vector>
#include<map>
#include<string> 
#include<regex>
 
std::wstring StringToWideString(const std::string& text, UINT encoding=65001)
{
    std::vector<wchar_t> buffer(text.size() + 1);
    int length = MultiByteToWideChar(encoding, 0, text.c_str(), text.size() + 1, buffer.data(), buffer.size());
    return std::wstring(buffer.data(), length - 1);

}

std::string WideStringToString(const std::wstring& text, UINT encoding = CP_UTF8)
{
    std::vector<char> buffer((text.size() + 1) * 4);
    WideCharToMultiByte(encoding, 0, text.c_str(), -1, buffer.data(), buffer.size(), nullptr, nullptr);
    return buffer.data();
} 
bool all_ascii(const char* s, int maxsize)
{
    if (s)
        for (int i = 0; i < maxsize && *s; i++, s++)
            if ((BYTE)*s > 127) // unsigned char
                return false;
    return true;
}

namespace {
    nlohmann::json namemap = []() {
        std::ifstream f("namemap.json");
        if (f.fail())
            return nlohmann::json();
        return nlohmann::json::parse(f);
    }();
    nlohmann::json translate = []() {
         
        std::ifstream f("translate.json");
        if (f.fail())
            return nlohmann::json();
        return nlohmann::json::parse(f);
    }();
    
}
void strReplace(std::string& str, const std::string& oldStr, const std::string& newStr)
{
    size_t pos = 0;
    while ((pos = str.find(oldStr, pos)) != std::string::npos) {
        str.replace(pos, oldStr.length(), newStr);
        pos += newStr.length();
    }
}
bool hookfun(winhook::hook_stack* s)
{
    auto text = (char*)s->ecx;
    if (text == 0)return false;
    std::string utf8string = text;
    if (all_ascii(text,strlen(text)))return false;
    
    if (namemap.find(utf8string) != namemap.end()) {
        std::string res = namemap.at(utf8string);
        auto cs = new char[res.size() + 1];
        strcpy(cs, res.c_str());
        s->ecx = (DWORD)cs;
        //strcpy(text, res.c_str());
        return true;
    }
    auto utf8save = utf8string; 
    strReplace(utf8save, "%51;", "\\-");
    strReplace(utf8save, "%164;", "\\+\\+");
    strReplace(utf8save, "%123;", "\\+");
    strReplace(utf8save, "%205;", "\\+\\+\\+");
    strReplace(utf8save, "#000033ff;", "\\#0033FF");
    strReplace(utf8save, "#;", "\\#FFFFFF");
    strReplace(utf8save, "#00ff0000;", "\\#FF0000");
    strReplace(utf8save, "%p-1;%f\xef\xbc\xad\xef\xbc\xb3 \xe3\x82\xb4\xe3\x82\xb7\xe3\x83\x83\xe3\x82\xaf;", ""); //"%p-1;%fＭＳ ゴシック;"
    strReplace(utf8save, "%p;%fuser;", "");
    if (translate.find(utf8save) != translate.end()) {
        
        std::string res = translate.at(utf8save);
        strReplace(res, "\\-", "%51;");
        strReplace(res, "\\+\\+", "%164;");
        strReplace(res, "\\+", "%123;");
        strReplace(res, "\\+\\+\\+", "%205;");
        strReplace(res, "\\#0033FF", "#000033ff;");
        strReplace(res, "\\#FFFFFF", "#;");
        strReplace(res, "\\#FF0000", "#00ff0000;");
        
        //strcpy(text, res.c_str());
        auto cs = new char[res.size() + 1];
        strcpy(cs, res.c_str());
        s->ecx = (DWORD)cs;
        return true;
    }
    else {
        std::ofstream notrans;
        notrans.open("notrans.txt", std::ios::app);
        if (notrans.is_open()) {
            notrans << utf8string << "\n";
            notrans <<"+"<< utf8save << "\n";
            notrans.close();
        }
        return false;
    }
    
}
#include<thread>
uintptr_t forwardsearch(BYTE* b, int size, uintptr_t addr, int range) {
    for (int i = 0; i < range; i++) {
        bool ok = true;
        for (int j = 0; j < size; j++) { 
            if ((*(BYTE*)(b + j)) != ((*(BYTE*)(addr + i + j)))) {
                ok = false; break;
            }
        }
        if (ok) {
            return addr + i;
        }
    }
    return 0;
}
bool CreateFontIndirectWh(winhook::hook_stack* s) {
    auto lplf = (LOGFONTW*)s->stack[1];
    wcscpy(lplf->lfFaceName, L"微软雅黑");
    return true;
} 

void hooks() {
    std::thread([]() {

        winhook::hook_before((UINT)CreateFontIndirectW, CreateFontIndirectWh);
        Sleep(4000);
        BYTE sig[] = { 0x55,0x8B,0xEC,0x83,0xEC,0x08,0x53,0x56,0x8B,0xF1,0x33,0xDB,0x8A,0x06,0x57,0x8B };
        DWORD start =(DWORD) GetModuleHandleW(0);
        DWORD addr = forwardsearch(sig, sizeof(sig), start, 0x10000);
        if (addr - (DWORD)(GetModuleHandle(0)) != 0x2180) {
            MessageBox(0, L"版本错误", L"", 0);
            return;
        } 
        winhook::hook_before(addr, hookfun);
        }
    ).detach();
    
}
__declspec(dllexport) void _() {}
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        hooks();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

