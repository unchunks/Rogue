#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>

// Mock necessary SDL types before including headers that use them
#include "SDL2/SDL.h"
#include "Utils/RectUtils.h"

struct TestCase {
    std::string name;
    SDL_Rect a;
    SDL_Rect b;
    bool expected;
};

void run_tests() {
    std::vector<TestCase> test_cases = {
        {"Full Overlap", {10, 10, 50, 50}, {10, 10, 50, 50}, true},
        {"A inside B", {20, 20, 10, 10}, {10, 10, 50, 50}, true},
        {"B inside A", {10, 10, 50, 50}, {20, 20, 10, 10}, true},
        {"Partial Overlap", {0, 0, 20, 20}, {10, 10, 20, 20}, true},
        {"Separate - Far", {0, 0, 10, 10}, {100, 100, 10, 10}, false},
        {"Separate - Horizontal", {0, 0, 10, 10}, {20, 0, 10, 10}, false},
        {"Separate - Vertical", {0, 0, 10, 10}, {0, 20, 10, 10}, false},
        {"Touch - Right", {0, 0, 10, 10}, {10, 0, 10, 10}, false},
        {"Touch - Left", {10, 0, 10, 10}, {0, 0, 10, 10}, false},
        {"Touch - Bottom", {0, 0, 10, 10}, {0, 10, 10, 10}, false},
        {"Touch - Top", {0, 10, 10, 10}, {0, 0, 10, 10}, false},
        {"Touch - Corner", {0, 0, 10, 10}, {10, 10, 10, 10}, false},
        {"Zero Width A", {0, 0, 0, 10}, {0, 0, 10, 10}, false},
        {"Zero Height B", {0, 0, 10, 10}, {0, 0, 10, 0}, false},
    };

    size_t passed = 0;
    for (const auto& tc : test_cases) {
        bool result = AinB(tc.a, tc.b);
        if (result == tc.expected) {
            std::cout << "[PASS] " << tc.name << std::endl;
            passed++;
        } else {
            std::cerr << "[FAIL] " << tc.name << " (Expected " << (tc.expected ? "true" : "false")
                      << ", got " << (result ? "true" : "false") << ")" << std::endl;
        }
    }

    std::cout << "Tests passed: " << passed << "/" << test_cases.size() << std::endl;
    if (passed != test_cases.size()) {
        exit(1);
    }
}

int main() {
    run_tests();
    return 0;
}
