#include <wx/wx.h>

#include "progress.h"
#include "threadrunner.h"

#pragma once

class MainWindow : public wxFrame {
public:
  MainWindow();

  static MainWindow* instance;
  ThreadRunner* thread{nullptr};
  Progress* progDialog{nullptr};

  wxBoxSizer* master{nullptr};

  wxButton* refreshButton{nullptr};
  wxButton* applyButton{nullptr};
  wxComboBox* windowSelect{nullptr};
  wxComboBox* devSelect{nullptr};

private:
  enum {
    ID_WindowSelect,
    ID_DUMMY, // on Win32, for some reason ID #1 is triggerred by hitting enter in wxTextCtrl? This is a workaround.
    ID_Initialize,
    ID_RefreshDev,
    ID_ApplyChanges,
    ID_DeviceSelect,
    ID_Docs,
    ID_Issue,

    ID_GenFile,
    ID_VerifyConfig,
    ID_ExportFile,
    ID_ImportFile,

    ID_OpenSerial,
    ID_StyleEditor
  };

  void bindEvents();
  void createToolTips();
  void createMenuBar();
  void createPages();
  void initialize();
};
