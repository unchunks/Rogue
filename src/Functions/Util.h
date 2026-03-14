#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string>

inline std::string resource_path(const char* relative_path) {
    const char* appdir = getenv("APPDIR");
    
    if (appdir) {
        // AppImage内で実行されている場合
        return std::string(appdir) + "/usr/share/rogue/assets/" + relative_path;
    } else {
        // 通常の実行の場合
        return std::string("assets/") + relative_path;
    }
}
