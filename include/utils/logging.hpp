#include <print>
#include <string.h>

// Thanks to https://stackoverflow.com/questions/8487986/file-macro-shows-full-path
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define LOG(format, ...) std::println("{}: " format, __FILENAME__, __VA_ARGS__)