#pragma once

#include <vector>
#include <queue>
#include <unordered_map>
#include <string>
#include <iostream>
#include "tui_tool_sets.hpp"


namespace utils
{
    using namespace std;
    using namespace ftxui;
    struct pair_hash
    {
        template <class T1, class T2>
        size_t operator()(const std::pair<T1, T2> &p) const
        {
            auto h1 = hash<T1>{}(p.first);
            auto h2 = hash<T2>{}(p.second);
            return h1 ^ h2;
        }
    };
    template <typename T>
    using pair_map = std::unordered_map<std::pair<int, int>, T, pair_hash>;
    template <typename T>
    using label_map = std::unordered_map<int, T>;

    template <typename T>
    T *row_major_vect_get(std::vector<vector<T>> &vec, int x, int y)
    {
        if (y < 0 || y >= vec.size() || x < 0 || x >= vec[y].size())
        {
            return nullptr;
        }
        return &vec[y][x];
    }

    template <typename... Args>
    static std::string str_format(const std::string &format, Args... args)
    {
        auto size_buf = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
        std::unique_ptr<char[]> buf(new (std::nothrow) char[size_buf]);

        if (!buf)
            return std::string("");

        std::snprintf(buf.get(), size_buf, format.c_str(), args...);
        return std::string(buf.get(), buf.get() + size_buf - 1);
    }

    // std::wstring
    template <typename... Args>
    static std::wstring wstr_format(const std::wstring &format, Args... args)
    {
        auto size_buf = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
        std::unique_ptr<char[]> buf(new (std::nothrow) char[size_buf]);

        if (!buf)
            return std::wstring("");

        std::snprintf(buf.get(), size_buf, format.c_str(), args...);
        return std::wstring(buf.get(), buf.get() + size_buf - 1);
    }

    template <typename K, typename V>
    std::string map_repr(std::unordered_map<K, V>* map) {
        std::ostringstream oss;
        oss << "{";
        bool first = true;
        for (const auto& pair : *map) {
            if (!first) {
                oss << ", ";
            }
            oss << "\"" << pair.first << "\": " << pair.second;
            first = false;
        }
        oss << "}";
        return oss.str();
    };

    template <typename T, typename K>
    std::function<K(K)> parse_element_style(T element, std::function<K(K)> default_style = nullptr) {
        std::function<K(K)> style = [](K ele) { return ele; };
        if (default_style != nullptr) { style = default_style; }
        std::function<K(K)> helper = style;
        if (element.contains("min_width")) {
            style = [=](K ele) { return helper(ele) | size(WIDTH, GREATER_THAN, element["min_width"]); };
        }
        helper = style;
        if (element.contains("max_width")) {
            style = [=](K ele) { return helper(ele) | size(WIDTH, LESS_THAN, element["max_width"]); };
        }
        helper = style;
        if (element.contains("min_height")) {
            style = [=](K ele) { return helper(ele) | size(HEIGHT, GREATER_THAN, element["min_height"]); };
        }
        helper = style;
        if (element.contains("max_height")) {
            style = [=](K ele) { return helper(ele) | size(HEIGHT, LESS_THAN, element["max_height"]); };
        }
        helper = style;
        if (element.contains("align_h") && element["align_h"] == "center") {
            style = [=](K ele) { return helper(ele) | hcenter; };
        } else if (element.contains("align_h") && element["align_h"] == "right") {
            style = [=](K ele) { return helper(ele) | align_right; };
        } else if (element.contains("align_h") && element["align_h"] == "left") {
            style = [=](K ele) { return helper(ele) | [](K child){ return hbox(std::move(child), filler()); }; };
        }
        helper = style;
        if (element.contains("align_v") && element["align_v"] == "center") {
            style = [=](K ele) { return helper(ele) | vcenter; };
        } else if (element.contains("align_v") && element["align_v"] == "up") {
            style = [=](K ele) { return helper(ele) | [](K child){ return vbox(std::move(child), filler()); }; };
        } else if (element.contains("align_v") && element["align_v"] == "bottom") {
            style = [=](K ele) { return helper(ele) | [](K child){ return vbox(filler(), std::move(child)); }; };
        }
        helper = style;
        if (element.contains("vscroll") && element["vscroll"] == true) {
            style = [=](K ele) { return helper(ele) | vscroll_indicator; };
        }
        helper = style;
        if (element.contains("hscroll") && element["hscroll"] == true) {
            style = [=](K ele) { return helper(ele) | hscroll_indicator; };
        }
        helper = style;
        if (element.contains("frame") && element["frame"] == true) {
            style = [=](K ele) { return helper(ele) | frame; };
        }
        helper = style;
        if (element.contains("vframe") && element["vframe"] == true) {
            style = [=](K ele) { return helper(ele) | vscroll_indicator | frame; };
        }
        helper = style;
        if (element.contains("hframe") && element["hframe"] == true) {
            style = [=](K ele) { return helper(ele) | hscroll_indicator | frame; };
        }
        return style;
    };

}
