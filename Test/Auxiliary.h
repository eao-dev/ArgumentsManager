#pragma once
#include <string>

static std::wstring toWstring(const std::string& str) {
	return std::wstring(str.begin(), str.end());
}