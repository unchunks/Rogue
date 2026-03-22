#include <iostream>
#include <string>
#include <cassert>
#include <stdlib.h>
#include "Functions/Util.h"

void test_output_path_vulnerability() {
    // Test default behavior
    unsetenv("TMPDIR");
    assert(output_path("test.txt") == "/tmp/test.txt");

    // Test path traversal vulnerability - should fallback to /tmp
    setenv("TMPDIR", "/tmp/../etc", 1);
    std::string path = output_path("passwd");
    std::cout << "Path with malicious TMPDIR (traversal): " << path << std::endl;
    assert(path == "/tmp/passwd");

    // Test relative path - should fallback to /tmp
    setenv("TMPDIR", "relative/path", 1);
    path = output_path("test.txt");
    std::cout << "Path with malicious TMPDIR (relative): " << path << std::endl;
    assert(path == "/tmp/test.txt");

    // Test valid absolute path
    setenv("TMPDIR", "/var/tmp", 1);
    path = output_path("test.txt");
    std::cout << "Path with valid TMPDIR: " << path << std::endl;
    assert(path == "/var/tmp/test.txt");

    // Test valid absolute path without trailing slash
    setenv("TMPDIR", "/var/tmp/", 1);
    path = output_path("test.txt");
    std::cout << "Path with valid TMPDIR (trailing slash): " << path << std::endl;
    assert(path == "/var/tmp/test.txt");
}

void test_resource_path() {
    // Test default behavior
    unsetenv("APPDIR");
    assert(resource_path("hero.png") == "assets/hero.png");

    // Test AppImage behavior
    setenv("APPDIR", "/tmp/appimage.12345", 1);
    assert(resource_path("hero.png") == "/tmp/appimage.12345/usr/share/rogue/assets/hero.png");
}

int main() {
    test_output_path_vulnerability();
    test_resource_path();
    std::cout << "Util tests completed!" << std::endl;
    return 0;
}
