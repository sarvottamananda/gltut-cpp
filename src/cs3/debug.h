#ifndef SNIPPETS_DEBUG_H
#define SNIPPETS_DEBUG_H
//	Sarvottamananda (shreesh)
//	2020-09-28
//	debug.h v0.0 (OpenGL Code Snippets)
//
//      Debugging by printing idiom
//
//      The debugging by printing idiom mainly follows
//      Jonathan Leffler's ideas from stackoverflow, but it is simplified.

#include <cstdio>
#include <string>

namespace debug {

//  Control whether debugging macros are active at compile time.
#if defined(DEBUG)
const int active = 1;
#else
const int active = 0;
#endif  // DEBUG

extern int setdebug(int level);
extern int getdebug(void);
extern FILE* setfilep(FILE* fp);
extern void setfilename(const std::string fn);
extern const std::string setsuffix(const std::string s);
extern int getindent(void);
extern int setindent(int val);
extern void print(int level, const char* fmt, ...);
extern void printloc(int level, const char* fn, int lno, const char* fmt, ...);
}  // namespace debug

// Usage (DP):  D_PRINT(fmt, ...);
// Usage (DD):  D_DEBUG(level, fmt, ...);
// Usage (DT):  D_TRACEL(level, fmt, ...);
// Usage (DC):  D_CALL(level, ...);

// The structure of the macros means that the code is always validated
// but is not called when DEBUG is undefined.

// -- See chapter 8 of 'The Practice of Programming', by Kernighan and Pike.

// Debug print
#define DPRINT(...)                                        \
    do {                                                   \
        if (debug::active) debug::print(999, __VA_ARGS__); \
    } while (0)

// Debug print based on level
#define DDEBUG(level, ...)                                   \
    do {                                                     \
        if (debug::active) debug::print(level, __VA_ARGS__); \
    } while (0)

// Debug trace based on level
#define DTRACE(level, ...)                                                          \
    do {                                                                            \
        if (debug::active) debug::printloc(level, __FILE__, __LINE__, __VA_ARGS__); \
    } while (0)

// Debug call based on level
#define DCALL(level, ...)                                    \
    do {                                                     \
        if (debug::active && debug::getdebug() >= (level)) { \
            __VA_ARGS__;                                     \
        }                                                    \
    } while (0)

// DB_TRACKING(); uses the FEATURE_FILE macro from klduge.h to embed a
// string in a function identifying that the file is compiled with debug
// enabled.

#define DTRACK()                                                             \
    do {                                                                     \
        if (debug::active) {                                                 \
            static int once = 0;                                             \
            if (once++ == 0) {                                               \
                fprintf(stderr, "** DEBUGGING ENABLED (%s) **\n", __FILE__); \
            }                                                                \
        }                                                                    \
    } while (0)

#endif  // SNIPPETS_DEBUG_H
