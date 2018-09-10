#include "MemoryChecker.h"
#include <windows.h>
#include <psapi.h>

MemoryChecker::MemoryChecker()
{

}

double MemoryChecker::memoryUsed()
{
    const int toMbBytes = 1048576;

    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    SIZE_T virtualMemUsedByMe = pmc.WorkingSetSize;
    return virtualMemUsedByMe / toMbBytes;
}
