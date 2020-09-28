// Sarvottamananda (shreesh)
// 2020-09-28
// debug.cpp v0.0 (OpenGL Code Snippets) 
//
// Debug by printing with tests

#undef DEBUG
#define DEBUG

#include "debug.h"

#include <cassert>  // for assert macros
#include <cstdarg>  // for varargs, main debug print functions
#include <string>   // for suffix

namespace {

int dbg_level = 0;
FILE* dbg_filep = stderr;
std::string dbg_filename = "/dev/stderr";
int dbg_indent = 0;

const int tabwidth = 2;
std::string dbg_suffix = "D";

}  // namespace

int debug::getdebug(void) { return dbg_level; }

int debug::setdebug(int level)
{
    int olevel = dbg_level;

    assert(level >= 0);
    dbg_level = level;
    return olevel;
}

int debug::getindent(void) { return dbg_indent; }

int debug::setindent(int val)
{
    int oindent = dbg_indent;

    assert(val >= 0);
    dbg_indent = val * tabwidth;
    return oindent;
}

FILE* debug::setfilep(FILE* fp)
{
    FILE* ofilep = dbg_filep;

    assert(fp != nullptr);
    dbg_filep = fp;
    if (dbg_filep != stdin && dbg_filep != stdout && dbg_filep != stderr) {
	fclose(dbg_filep);
    }
    dbg_filep = fp;
    dbg_filename = "<unknown>";
    return ofilep;
}

void debug::setfilename(const std::string fn)
{
    FILE* fp;

    assert(fn.size() > 0);
    if (fn == dbg_filename)  // same as last time?
	return;
    if ((fp = fopen(fn.c_str(), "a+")) != nullptr) {
	setfilep(fp);
	std::string ofilename = dbg_filename;
	dbg_filename = fn;
    }
}

const std::string debug::setsuffix(const std::string s)
{
    std::string osuffix = dbg_suffix;
    dbg_suffix = s;
    return osuffix;
}

void debug::print(int level, const char* fmt, ...)
{
    DTRACK();
    if (dbg_level >= level) {
	va_list ap;

	fflush(stdout);
	flockfile(dbg_filep);

	va_start(ap, fmt);
	fprintf(dbg_filep, "%s:%*s", dbg_suffix.c_str(), dbg_indent, "");
	vfprintf(dbg_filep, fmt, ap);
	fputc('\n', dbg_filep);
	va_end(ap);

	fflush(dbg_filep);
	funlockfile(dbg_filep);
    }
}

void debug::printloc(int level, const char* fn, int lno, const char* fmt, ...)
{
    if (dbg_level >= level) {
	va_list ap;

	fflush(stdout);
	flockfile(dbg_filep);

	va_start(ap, fmt);
	fprintf(dbg_filep, "%s:%*s%s %d", dbg_suffix.c_str(), dbg_indent, "", fn, lno);
	vfprintf(dbg_filep, fmt, ap);
	fputc('\n', dbg_filep);
	va_end(ap);

	fflush(dbg_filep);
	funlockfile(dbg_filep);
    }
}

#if defined(TEST)

static void test(void)
{
    D_TRACE(1, "This should have appeared at debug level %d; %d %f\n", 1, 3, 3.141593);
    D_TRACE(2, "This should have appeared at debug level %d; %d %f\n", 2, 3, 3.141593);
    D_TRACELOC(3, "This should have appeared at debug level %d; %d %f\n", 3, 3, 3.141593);
    D_TRACE(4, "This should have appeared at debug level %d; %d %f\n", 4, 3, 3.141593);
    D_CALL(5, printf("Hello, world!\n"));
}

static void test1(int i)
{
    debug::setdebug(i);
    fprintf(stderr, "Debug level is: %d\n", debug::getdebug());
    test();
    debug::setindent(i);
    test();
    debug::setindent(0);
    test();
}

static void test_debug(int argc, char** argv)
{
    int i;

    for (i = 0; i <= argc; i++) test1(i);

    if (argc > 0) {
	fprintf(stderr, "Recurse with argc = %d\n", argc - 1);
	test_debug(argc - 1, argv);
    }
}

int main(int argc, char** argv)
{
    D_TRACKING();
    if (argc <= 1) printf("It is more fun if you add arguments to the command line!\n");
    test_debug(argc, argv);
    return (0);
}

#endif	// TEST
