// ProffieConfig, All-In-One GUI Proffieboard Configuration Utility
// Copyright (C) 2024 Ryan Ogurek

#include "editor/pages/propspage.h"

#include "core/appstate.h"
#include "core/defines.h"
#include "core/utilities/misc.h"
#include "core/config/propfile.h"
#include "editor/editorwindow.h"
#include "editor/pages/generalpage.h"
#include "ui/pccombobox.h"

#include <wx/scrolwin.h>
#include <wx/sizer.h>
#include <wx/tooltip.h>

PropsPage::PropsPage(wxWindow* window) : wxScrolledWindow(window), parent{static_cast<EditorWindow*>(window)} {
  sizer = new wxStaticBoxSizer(wxVERTICAL, this, "");
  auto top = new wxBoxSizer(wxHORIZONTAL);
  propSelection = new pcComboBox(sizer->GetStaticBox(), ID_PropSelect, "Prop File", wxDefaultPosition, wxDefaultSize, Misc::createEntries({"Default"}), wxCB_READONLY);
  buttonInfo = new wxButton(sizer->GetStaticBox(), ID_Buttons, "Buttons...");
  top->Add(propSelection, wxSizerFlags(0).Border(wxALL, 10));
  top->Add(buttonInfo, wxSizerFlags(0).Border(wxALL, 10).Bottom());

  sizer->Add(top);

  loadProps();

  bindEvents();

  SetSizerAndFit(sizer);
  SetScrollbars(-1, 10, -1, 1);
}

void PropsPage::bindEvents() {
  auto propSelectUpdate = [&](wxCommandEvent&) {
    updateSelectedProp();
    update();
    SetMinClientSize(wxSize(sizer->GetMinSize().GetWidth(), 0));
    FULLUPDATEWINDOW(parent);
    parent->SetSize(wxSize(parent->GetSize().GetWidth(), parent->GetMinHeight() + GetBestVirtualSize().GetHeight()));
    parent->SetMinSize(wxSize(parent->GetSize().GetWidth(), 350));
  };
  auto optionSelectUpdate = [&](wxCommandEvent&) {
    int32_t x, y;
    GetViewStart(&x, &y);
    update();
    Scroll(0, y);
  };

  Bind(wxEVT_COMBOBOX, propSelectUpdate, ID_PropSelect);
  Bind(wxEVT_CHECKBOX, optionSelectUpdate, wxID_ANY);
  Bind(wxEVT_RADIOBUTTON, optionSelectUpdate, wxID_ANY);
  Bind(wxEVT_SPINCTRL, optionSelectUpdate, wxID_ANY);
  Bind(wxEVT_SPINCTRLDOUBLE, optionSelectUpdate, wxID_ANY);

  Bind(wxEVT_BUTTON, [&](wxCommandEvent&) {
        std::string buttons;

        PropFile* activeProp{nullptr};
        for (auto& prop : props) {
          if (propSelection->entry()->GetStringSelection() == prop->getName()) activeProp = prop;
        }

        if (activeProp == nullptr) {
          buttons =
              (parent->generalPage->buttons.num->GetValue() == 0 ? wxString (
                   "On/Off - Twist\n"
                   "Next preset - Point up and shake\n"
                   "Clash - Hit the blade while saber is on."
                   ) : parent->generalPage->buttons.num->GetValue() == 1 ? wxString(
                     "On/Off - Click to turn the saber on or off.\n"
                     "Turn On muted - Double-click\n"
                     "Next preset - Hold button and hit the blade while saber is off.\n"
                     "Clash - Hit the blade while saber is on.\n"
                     "Lockup - Hold button, then trigger a clash. Release button to end.\n"
                     "Drag - Hold button, then trigger a clash while pointing down. Release button to end.\n"
                     "Melt - Hold button and stab something.\n"
                     "Force - Long-click button.\n"
                     "Start Soundtrack - Long-click the button while blade is off.\n"
                     "Enter/Exit Color Change - Hold button and Twist."
                     ) : parent->generalPage->buttons.num->GetValue() == 2 || parent->generalPage->buttons.num->GetValue() == 3 ? wxString (
                     "On/Off - Click POW\n"
                     "Turn On muted - Double-click POW button\n"
                     "Next preset - Hold POW button and hit the blade while saber is off.\n"
                     "Previous Preset - Hold AUX button and click the POW button while saber is off.\n"
                     "Clash - Hit the blade while saber is on.\n"
                     "Lockup -  Hold either POW or AUX, then trigger a clash. Release button to end.\n"
                     "Drag - Hold either POW or AUX, then trigger a clash while pointing down. Release button to end.\n"
                     "Melt - Hold either POW or AUX and stab something.\n"
                     "Force Lightning Block - Click AUX while holding POW.\n"
                     "Force - Long-click POW button.\n"
                     "Start Soundtrack - Long-click the POW button while blade is off.\n"
                     "Blaster block - Short-click AUX button.\n"
                     "Enter/Exit Color Change - Hold Aux and click POW while on."
                     ) : wxString("Button Configuration Not Supported"));
        } else {
          auto propButtons = activeProp->getButtons().at(parent->generalPage->buttons.num->GetValue());
          if (propButtons.empty()) buttons += "Selected number of buttons not supported by prop file.";
          else for (auto& state : propButtons) {
              buttons += "Button controls while saber is " + state.first + ":\n";
              for (auto& button : state.second) {
                buttons += "\t" + button.name + " - ";
                std::vector<std::string> activePredicates{};
                for (const auto& predicate : button.relevantSettings) {
                  auto key = activeProp->getSettings().find(predicate);
                  if (key == activeProp->getSettings().end()) continue;

                  if (!key->second.getOutput().empty()) activePredicates.push_back(key->first);
                }

                auto key = button.descriptions.find(activePredicates);
                if (key == button.descriptions.end()) buttons += "UNDEFINED";
                else buttons += key->second;

                buttons += '\n';
              }
              buttons += '\n';
            }
          if (buttons.at(buttons.size() - 1) == '\n') {
            buttons.pop_back();
            buttons.pop_back();
          }
        }

        auto buttonDialog = wxDialog(
            parent,
            wxID_ANY,
            "Prop File Buttons",
            wxDefaultPosition,
            wxDefaultSize,
            wxDEFAULT_DIALOG_STYLE | wxSTAY_ON_TOP
            );
        auto textSizer = new wxBoxSizer(wxVERTICAL);
        textSizer->Add(buttonDialog.CreateTextSizer(buttons), wxSizerFlags(0).Border(wxALL, 10));
        buttonDialog.SetSizer(textSizer);
        buttonDialog.DoLayoutAdaptation();
        buttonDialog.ShowModal();
      }, ID_Buttons);
}

void PropsPage::updateProps() {
  auto lastSelect = propSelection->entry()->GetStringSelection();
  propSelection->entry()->Clear();
  propSelection->entry()->Append("Default");
  for (const auto& prop : props) {
    propSelection->entry()->Append(prop->getName());
  }
  if ([=]() { for (const auto& prop : propSelection->entry()->GetStrings()) if (prop == lastSelect) return true; return false; }()) {
    propSelection->entry()->SetStringSelection(lastSelect);
  } else propSelection->entry()->SetStringSelection("Default");
}

void PropsPage::update() {
  for (auto& prop : props) {
    if (propSelection->entry()->GetStringSelection() != prop->getName()) continue;

    for (auto& [ name, setting ] : prop->getSettings()) {
      for (const auto& disable : setting.disables) {
        auto key = prop->getSettings().find(disable);
        if (key == prop->getSettings().end()) continue;

        key->second.disabled = !setting.getOutput().empty();
      }
    }

    for (auto& [ name, setting ] : prop->getSettings()) {
      setting.enable(!setting.disabled && setting.checkRequiredSatisfied(prop->getSettings()));
    }
  }
}

const std::vector<PropFile*>& PropsPage::getLoadedProps() { return props; }
PropFile* PropsPage::getSelectedProp() {
  for (const auto& prop : props) {
    if (prop->getName() == propSelection->entry()->GetStringSelection()) return prop;
  }
  return nullptr;
}

void PropsPage::updateSelectedProp(const wxString& newProp) {
  if (!newProp.empty()) propSelection->entry()->SetStringSelection(newProp);
  for (auto& prop : props) {
    prop->Show(propSelection->entry()->GetStringSelection() == prop->getName());
  }
}
void PropsPage::loadProps() {
  props.clear();
  for (const auto& prop : AppState::instance->getPropFileNames()) {
    auto propConfig = PropFile::createPropConfig(prop, sizer->GetStaticBox());
    if (propConfig != nullptr) {
      sizer->Add(propConfig);
      props.push_back(propConfig);
    }
  }
  updateProps();
}
