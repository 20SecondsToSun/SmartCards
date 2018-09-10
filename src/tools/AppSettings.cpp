#include "AppSettings.h"
#include <windows.h>

AppSettings::AppSettings()
{

}

void AppSettings::init()
{
    SetErrorMode(SEM_NOGPFAULTERRORBOX);
    _set_error_mode (_OUT_TO_STDERR);
}
