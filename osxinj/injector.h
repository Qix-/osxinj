#ifndef _INJECTOR_H_
#define _INJECTOR_H_

#include <string>

#include "mach_inject.h"

class Injector
{
public:
    Injector(std::string = "bootstrap.dylib");
    ~Injector();

    std::string inject(pid_t pid, const char* lib);
    pid_t getProcessByName(const char *name);
private:
    void *module;
    void *bootstrapfn;
};

#endif
