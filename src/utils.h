#include <iostream>
#include <string>
#include <cstring>
#include <cctype>
#include <algorithm>
#include <vector>

enum class DesktopType {
    X11,
    WAYLAND,
    UNKNOWN
};

enum class DesktopEnvironment {
    GNOME,
    KDE,
    UKUI,
    UNKNOWN
};

static std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                  [](unsigned char c) { return std::tolower(c); });
    return result;
}

static std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);
    
    while (end != std::string::npos) {
        if (end != start) {
            tokens.push_back(str.substr(start, end - start));
        }
        start = end + 1;
        end = str.find(delimiter, start);
    }
    
    if (start < str.length()) {
        tokens.push_back(str.substr(start));
    }
    
    return tokens;
}

static inline DesktopEnvironment getDesktopEnvironment() {
    //在ubuntu 上该环境变量会返回gnome:GNOME
    const char* xdgDesktop = std::getenv("XDG_CURRENT_DESKTOP");
    if (xdgDesktop == nullptr) {
        return DesktopEnvironment::UNKNOWN;
    }

    std::string lowerDesktop = toLower(xdgDesktop);
    auto desktops = splitString(lowerDesktop, ':');
    
    for (const auto& name : desktops) {
        if (name == "kde") {
            return DesktopEnvironment::KDE;
        } else if (name == "gnome") {
            return DesktopEnvironment::GNOME;
        } else if (name == "ukui") {
            return DesktopEnvironment::UKUI;
        }
    }
    
    return DesktopEnvironment::UNKNOWN;
}

static inline DesktopType getDesktopType() {
    const char* sessionType = std::getenv("XDG_SESSION_TYPE");
    if (sessionType == nullptr) {
        return DesktopType::UNKNOWN;
    }
    
    std::string type = toLower(sessionType);
    if (type == "x11") {
        return DesktopType::X11;
    } else if (type == "wayland") {
        return DesktopType::WAYLAND;
    }
    
    return DesktopType::UNKNOWN;
}