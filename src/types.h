#pragma once
#include <map>

static const std::nullptr_t null = nullptr;

template<typename T, typename R>
using Map = std::map<T, R>;
using uchar = unsigned char;
using ulong = unsigned long;
using ushort = unsigned short;
