// ProffieConfig, All-In-One GUI Proffieboard Configuration Utility
// Copyright (C) 2023 Ryan Ogurek

#include "core/appstate.h"

#include <wx/splash.h>
#include <wx/app.h>

#ifdef __WXOSX__
#include <mach-o/dyld.h>
#include <libgen.h>
#endif

class ProffieConfig : public wxApp {
public:
  virtual bool OnInit() {

#   ifdef __WXOSX__
    uint32_t pathLen = sizeof(Misc::path);
    _NSGetExecutablePath(Misc::path, &pathLen);
    strncpy(Misc::path, dirname(Misc::path), PATH_MAX);
    chdir(Misc::path);
#   endif

    AppState::init();

    return true;
  }
};

wxIMPLEMENT_APP(ProffieConfig);
