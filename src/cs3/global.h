#ifndef SNIPPETS_GLOBAL_H
#define SNIPPETS_GLOBAL_H

//	Sarvottamananda (shreesh)
//	2020-09-28
//	global.h v0.0 (OpenGL Code Snippets)
//
//      Interface for global variables

class Global {
   public:  // All globals are public
    Global() = default;
    ~Global() = default;

    // Copy
    Global(const Global&) = delete;
    Global& operator=(const Global&) = delete;
    // Move
    Global(Global&&) = delete;
    Global& operator=(Global&&) = delete;

    int zero = 0;
    float zerof = 0.0f;
    // and other "global" variable

    static Global& Instance()
    {
	static Global global;
	return global;
    }

    void print();
};

// Eiher static or unnamed namespace

// static Global& global = Global::Instance();
namespace {
Global& global = Global::Instance();
}

#endif	// SNIPPETS_GLOBAL_H
