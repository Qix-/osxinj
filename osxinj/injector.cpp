#include "injector.h"

#include <cstdio>
#include <cstring>
#include <sstream>
#include <stdexcept>

#include <mach/mach.h>
#include <mach/thread_act.h>
#include <mach/mach_init.h>
#include <mach/error.h>
#include <mach/mach_error.h>
#include <pthread.h>
#include <dlfcn.h>
#include <signal.h>
#include <sys/proc_info.h>
#include <libproc.h>

Injector::Injector(std::string bootstrapPath) : module(0), bootstrapfn(0)
{
    module = dlopen(bootstrapPath.c_str(),
        RTLD_NOW | RTLD_LOCAL);

    printf("module: 0x%X\n", module);
    if (!module)
    {
        fprintf(stderr, "dlopen error: %s\n", dlerror());
        return;
    }

    bootstrapfn = dlsym(module, "bootstrap");
    printf("bootstrapfn: 0x%X\n", bootstrapfn);

    if (!bootstrapfn)
    {
        fprintf(stderr, "could not locate bootstrap fn\n");
        return;
    }
}

Injector::~Injector()
{
    if (module)
    {
        dlclose(module);
        module = NULL;
    }
}

std::string Injector::inject(pid_t pid, const char* lib)
{
    if (!module || !bootstrapfn)
    {
	std::stringstream ss;
        ss << "failed to inject: module:0x" << std::hex << module << " bootstrapfn:0x" << bootstrapfn << std::endl;
	return ss.str();
    }
    mach_error_t err = mach_inject((mach_inject_entry)bootstrapfn, lib, strlen(lib) + 1, pid, 0);
    if (err) {
        return mach_error_string(err);
    }
    return std::string();
}

pid_t Injector::getProcessByName(const char *name)
{
    int procCnt = proc_listpids(PROC_ALL_PIDS, 0, NULL, 0);
    pid_t pids[1024];
    memset(pids, 0, sizeof pids);
    proc_listpids(PROC_ALL_PIDS, 0, pids, sizeof(pids));

    for (int i = 0; i < procCnt; i++)
    {
        if (!pids[i]) continue;
        char curPath[PROC_PIDPATHINFO_MAXSIZE];
        char curName[PROC_PIDPATHINFO_MAXSIZE];
        memset(curPath, 0, sizeof curPath);
        proc_pidpath(pids[i], curPath, sizeof curPath);
        int len = strlen(curPath);
        if (len)
        {
            int pos = len;
            while (pos && curPath[pos] != '/') --pos;
            strcpy(curName, curPath + pos + 1);
            if (!strcmp(curName, name))
            {
                return pids[i];
            }
        }
    }
    return 0;
}
