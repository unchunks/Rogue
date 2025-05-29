#pragma once
#include <stdio.h>
#include <stdlib.h>

inline char* resource_path(const char* relative_path) {
    static char full_path[1024];
    const char* appdir = getenv("APPDIR");
    
    if (appdir) {
        // AppImage内で実行されている場合
        snprintf(full_path, sizeof(full_path), "%s/usr/share/rogue/assets/%s", appdir, relative_path);
    } else {
        // 通常の実行の場合
        snprintf(full_path, sizeof(full_path), "assets/%s", relative_path);
    }
    
    return full_path;
}