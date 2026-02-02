#include "StringUtils.h"
#include <algorithm>
#include <ranges>
#include <string>
#include <string_view>

using std::string;

namespace StringUtils {

std::string Slice(const std::string &str, ssize_t start, ssize_t end) noexcept {
    auto iter_begin = start < 0 ? str.cend() + start : str.cbegin() + start;
    auto iter_end = end == 0 ? str.cend() : str.cbegin() + end;
    return string(iter_begin, iter_end);
}

std::string Capitalize(const std::string &str) noexcept {
    auto newstr = string(str);
    newstr[0] = std::toupper(newstr[0]);
    return newstr;
}

std::string Upper(const std::string &str) noexcept {
    string newstr(str);
    auto iter_begin = newstr.begin();
    auto iter_end = newstr.end();
    while (iter_begin != iter_end) {
        *iter_begin = std::toupper(*iter_begin);
        iter_begin++;
    }
    return newstr;
}

std::string Lower(const std::string &str) noexcept {
    string newstr(str);
    auto iter_begin = newstr.begin();
    auto iter_end = newstr.end();
    while (iter_begin != iter_end) {
        *iter_begin = std::tolower(*iter_begin);
        iter_begin++;
    }
    return newstr;
}

std::string LStrip(const std::string &str) noexcept {
    auto idx = str.find_first_not_of(' ');
    return string(str.cbegin() + idx, str.cend());
}

std::string RStrip(const std::string &str) noexcept {
    auto idx = str.find_last_not_of(' ');
    return string(str.cbegin(), str.cbegin() + idx + 1);
}

std::string Strip(const std::string &str) noexcept {
    auto idx_l = str.find_first_not_of(' ');
    auto idx_r = str.find_last_not_of(' ');
    return string(str.cbegin() + idx_l, str.cbegin() + idx_r + 1);
}

std::string Center(const std::string &str, int width, char fill) noexcept {
    auto front = (width - str.length()) / 2;
    auto back = width - str.length() - front;
    return string(front, fill) + str + string(back, fill);
}

std::string LJust(const std::string &str, int width, char fill) noexcept {
    return str + string(width - str.length(), fill);
}

std::string RJust(const std::string &str, int width, char fill) noexcept {
    return string(width - str.length(), fill) + str;
}

std::string Replace(const std::string &str, const std::string &old,
                    const std::string &rep) noexcept {
    if (old.empty()) {
        return string(str);
    }
    string newstr(str);
    string::size_type pos = 0;
    while ((pos = newstr.find(old, pos)) != string::npos) {
        newstr.replace(pos, old.length(), rep);
        pos += rep.length();
    }
    return newstr;
}

std::vector<std::string> Split(const std::string &str,
                               const std::string &splt) noexcept {
    std::vector<string> result;

    auto delim = splt == "" ? string(" ") : splt;

    std::string_view str_v(str);
    std::string_view delim_v(delim);

    for (const auto it : std::views::split(str_v, delim_v)) {
        result.emplace_back(string(std::string_view(it)));
    }
    return result;
}

std::string Join(const std::string &str,
                 const std::vector<std::string> &vect) noexcept {
    auto joined_string =
        std::vector<std::string_view>(vect.cbegin(), vect.cend()) |
        std::views::join_with(str);
    return joined_string | std::ranges::to<string>();
}

std::string ExpandTabs(const std::string &str, int tabsize) noexcept {
    // Replace code here
    return Replace(str, "\t", string(tabsize, ' '));
}

int min3(int a, int b, int c) { return std::min(std::min(a, b), c); }

int EditDistanceRec(const std::string &left,
                    const std::string &right) noexcept {
    if (left.length() == 0)
        return right.length();
    if (right.length() == 0)
        return left.length();
    if (left.front() == right.front())
        return EditDistanceRec(string(left.cbegin() + 1, left.cend()),
                               string(right.cbegin() + 1, right.cend()));
    return 1 + min3(EditDistanceRec(string(left.cbegin(), left.cend()),
                                    string(right.cbegin() + 1, right.cend())),
                    EditDistanceRec(string(left.cbegin() + 1, left.cend()),
                                    string(right.cbegin(), right.cend())),
                    EditDistanceRec(string(left.cbegin() + 1, left.cend()),
                                    string(right.cbegin() + 1, right.cend())));
}

int EditDistance(const std::string &left, const std::string &right,
                 bool ignorecase) noexcept {
    return EditDistanceRec(ignorecase ? Upper(left) : left,
                           ignorecase ? Upper(right) : right);
}

bool IsAllWhiteSpace(const std::string &s) noexcept {
    return std::ranges::all_of(s, [](char c) { return std::isspace(c); });
}

}; // namespace StringUtils
