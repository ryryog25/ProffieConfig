#include "misc.h"

#include <wx/msgdlg.h>
#include <wx/arrstr.h>
#include <wx/filedlg.h>
#include <wx/wfstream.h>
#include <initializer_list>

#ifdef __WXOSX__
char Misc::path[];
#endif

wxEventTypeTag<wxCommandEvent> Misc::EVT_MSGBOX(wxNewEventType());

Misc::numEntry* Misc::createNumEntry(wxWindow* parent, wxString displayText, int32_t ID, int32_t minVal, int32_t maxVal, int32_t defaultVal) {
  Misc::numEntry* numEntry = new Misc::numEntry;

  numEntry->box = new wxBoxSizer(wxHORIZONTAL);
  numEntry->text = new wxStaticText(parent, wxID_ANY, displayText);
  numEntry->num = new wxSpinCtrl(parent, ID, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, minVal, maxVal, defaultVal);
  numEntry->box->Add(numEntry->text, wxSizerFlags(0).Border(wxRIGHT, 5).Center());
  numEntry->box->Add(numEntry->num, wxSizerFlags(0).Expand());

  return numEntry;
}
Misc::numEntryDouble* Misc::createNumEntryDouble(wxWindow* parent, wxString displayText, int32_t ID, double minVal, double maxVal, double defaultVal) {
  Misc::numEntryDouble* numEntry = new Misc::numEntryDouble;

  numEntry->box = new wxBoxSizer(wxHORIZONTAL);
  numEntry->text = new wxStaticText(parent, wxID_ANY, displayText);
  numEntry->num = new wxSpinCtrlDouble(parent, ID, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, minVal, maxVal, defaultVal, 0.1);
  numEntry->box->Add(numEntry->text, wxSizerFlags(0).Border(wxRIGHT, 5).Center());
  numEntry->box->Add(numEntry->num, wxSizerFlags(0).Expand());

  return numEntry;
}
Misc::comboBoxEntry* Misc::createComboBoxEntry(wxWindow* parent, wxString displayText, int32_t ID, wxString defaultOption, wxArrayString options, int32_t flags) {
  Misc::comboBoxEntry* comboBoxEntry = new Misc::comboBoxEntry;

  comboBoxEntry->box = new wxBoxSizer(wxVERTICAL);
  comboBoxEntry->text = new wxStaticText(parent, wxID_ANY, displayText);
  comboBoxEntry->entry = new wxComboBox(parent, ID, defaultOption, wxDefaultPosition, wxDefaultSize, options, flags);
  comboBoxEntry->box->Add(comboBoxEntry->text);
  comboBoxEntry->box->Add(comboBoxEntry->entry, wxSizerFlags(0).Expand());

  return comboBoxEntry;
}
Misc::textEntry* Misc::createTextEntry(wxWindow* parent, wxString displayText, int32_t ID, wxString defaultOption, int32_t flags) {
  Misc::textEntry* textEntry = new Misc::textEntry;

  textEntry->box = new wxBoxSizer(wxVERTICAL);
  textEntry->text = new wxStaticText(parent, wxID_ANY, displayText);
  textEntry->entry = new wxTextCtrl(parent, ID, defaultOption, wxDefaultPosition, wxDefaultSize, flags);
  textEntry->box->Add(textEntry->text);
  textEntry->box->Add(textEntry->entry, wxSizerFlags(0).Expand());

  return textEntry;
}


const wxArrayString Misc::createEntries(std::vector<wxString> list) {
  wxArrayString entries;
  for (const wxString& entry : list)
    entries.Add(entry);

  return entries;
}
const wxArrayString Misc::createEntries(std::initializer_list<wxString> list) {
  return Misc::createEntries(static_cast<std::vector<wxString>>(list));
}