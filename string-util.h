
#pragma once
#include <string>
#include <stdarg.h>
#include <algorithm>
#include <iterator>
#include <locale>
#include <functional>
#include <chrono>

#include <sstream>
#include <iostream>

#include <string>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <zlib.h>
std::string compressString(const std::string& str,
                            int compressionlevel = Z_BEST_COMPRESSION);

/** Decompress an STL string using zlib and return the original data. */
std::string decompressString(const std::string& str);

std::string insertSeparator(const std::string& s, char separator = ',', int width = 3);
std::string replaceAll(const std::string &str, const std::string &pattern, const std::string &replace);

