#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string>

inline std::string resource_path(const char* relative_path) {
    const char* appdir = getenv("APPDIR");
    std::string path;

    if (appdir) {
        // AppImage内で実行されている場合
        path = std::string(appdir) + "/usr/share/rogue/assets/";
    } else {
        // 通常の実行の場合
        path = "assets/";
    }

    return path + relative_path;
}

inline std::string output_path(const std::string& filename) {
    const char* tmpdir = getenv("TMPDIR");
    if (!tmpdir) {
        tmpdir = "/tmp";
    }
    std::string path = tmpdir;
    if (!path.empty() && path.back() != '/') {
        path += "/";
    }
    return path + filename;
}
