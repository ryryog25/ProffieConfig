// ProffieConfig, All-In-One GUI Proffieboard Configuration Utility
// Copyright (C) 2024 Ryan Ogurek

#include "core/config/configuration.h"

#include "core/defines.h"
#include "core/config/settings.h"
#include "core/config/propfile.h"
#include "editor/editorwindow.h"
#include "editor/pages/generalpage.h"
#include "editor/pages/presetspage.h"
#include "editor/pages/propspage.h"
#include "editor/pages/bladespage.h"
#include "editor/dialogs/bladearraydlg.h"
#include "wx/event.h"

#include <cstring>
#include <sstream>
#include <exception>
#include <wx/filedlg.h>

# define ERR(msg) \
  Misc::MessageBoxEvent* msgEvent = new Misc::MessageBoxEvent(wxID_ANY, std::string(msg) + "\n\nConfiguration not saved.", "Configuration Error", wxOK | wxCENTER | wxICON_ERROR); \
  wxQueueEvent(editor->GetEventHandler(), msgEvent); \
  return false;


bool Configuration::outputConfig(const std::string& filePath, EditorWindow* editor) {
  editor->presetsPage->update();
  editor->bladesPage->update();
  editor->bladesPage->bladeArrayDlg->update();

  if (!runPreChecks(editor)) return false;

  std::ofstream configOutput(filePath);
  if (!configOutput.is_open()) {
    ERR("Could not open config file for output.");
  }

  configOutput <<
      "/*" << std::endl <<
      "This configuration file was generated by ProffieConfig " VERSION ", created by Ryryog25." << std::endl <<
      "The tool can be found here: https://github.com/ryryog25/ProffieConfig" << std::endl <<
      "ProffieConfig is an All-In-One utility for managing your Proffieboard." << std::endl <<
      "*/" << std::endl << std::endl;

  outputConfigTop(configOutput, editor);
  outputConfigProp(configOutput, editor);
  outputConfigPresets(configOutput, editor);
  outputConfigButtons(configOutput, editor);

  configOutput.close();
  return true;
}
bool Configuration::outputConfig(EditorWindow* editor) { return Configuration::outputConfig(CONFIG_DIR + editor->getOpenConfig() + ".h", editor); }
bool Configuration::exportConfig(EditorWindow* editor) {
  wxFileDialog configLocation(editor, "Save ProffieOS Config File", "", editor->getOpenConfig(), "ProffieOS Configuration (*.h)|*.h", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

  if (configLocation.ShowModal() == wxID_CANCEL) return false; // User Closed

  return Configuration::outputConfig(configLocation.GetPath().ToStdString(), editor);
}

void Configuration::outputConfigTop(std::ofstream& configOutput, EditorWindow* editor) {
  configOutput << "#ifdef CONFIG_TOP" << std::endl;
  outputConfigTopGeneral(configOutput, editor);
  outputConfigTopPropSpecific(configOutput, editor);
  outputConfigTopCustom(configOutput, editor);
  configOutput << "#endif" << std::endl << std::endl;

}
void Configuration::outputConfigTopGeneral(std::ofstream& configOutput, EditorWindow* editor) {
  if (editor->generalPage->massStorage->GetValue()) configOutput << "//PROFFIECONFIG ENABLE_MASS_STORAGE" << std::endl;
  if (editor->generalPage->webUSB->GetValue()) configOutput << "//PROFFIECONFIG ENABLE_WEBUSB" << std::endl;

  configOutput << findInVMap(Proffieboard, editor->generalPage->board->entry()->GetValue().ToStdString()).second << std::endl;

  configOutput << "const unsigned int maxLedsPerStrip = " << editor->generalPage->maxLEDs->entry()->GetValue() << ";" << std::endl;
  configOutput << "#define ENABLE_AUDIO" << std::endl;
  configOutput << "#define ENABLE_WS2811" << std::endl;
  configOutput << "#define ENABLE_SD" << std::endl;
  configOutput << "#define ENABLE_MOTION" << std::endl;
  configOutput << "#define SHARED_POWER_PINS" << std::endl;

  for (const auto& [ name, define ] : editor->settings->generalDefines) {
    if (define->shouldOutput()) configOutput << "#define " << define->getOutput() << std::endl;
  }
}
void Configuration::outputConfigTopPropSpecific(std::ofstream& configOutput, EditorWindow* editor) {
  auto selectedProp = editor->propsPage->getSelectedProp();
  if (selectedProp == nullptr) return;

  for (const auto& [ name, setting ] : selectedProp->getSettings()) {
    if (!setting.checkRequiredSatisfied(selectedProp->getSettings()) || setting.disabled) continue;
    auto output = setting.getOutput();
    if (!output.empty()) configOutput << "#define " << output << std::endl;
  }
}
void Configuration::outputConfigTopCustom(std::ofstream& configOutput, EditorWindow* editor) {
  for (const auto& [ name, value ] : editor->generalPage->customOptDlg->getCustomDefines()) {
    configOutput << "#define " << name << " " << value << std::endl;
  }
}

void Configuration::outputConfigProp(std::ofstream& configOutput, EditorWindow* editor) {
  auto selectedProp = editor->propsPage->getSelectedProp();
  if (selectedProp == nullptr) return;

  configOutput << "#ifdef CONFIG_PROP" << std::endl;
  configOutput << "#include \"../props/" << selectedProp->getFileName() << "\"" << std::endl;
  configOutput << "#endif" << std:: endl << std::endl; // CONFIG_PROP
}
void Configuration::outputConfigPresets(std::ofstream& configOutput, EditorWindow* editor) {
  configOutput << "#ifdef CONFIG_PRESETS" << std::endl;
  outputConfigPresetsStyles(configOutput, editor);
  outputConfigPresetsBlades(configOutput, editor);
  configOutput << "#endif" << std::endl << std::endl;
}
void Configuration::outputConfigPresetsStyles(std::ofstream& configOutput, EditorWindow* editor) {
  for (const BladeArrayDlg::BladeArray& bladeArray : editor->bladesPage->bladeArrayDlg->bladeArrays) {
    configOutput << "Preset " << bladeArray.name << "[] = {" << std::endl;
    for (const PresetsPage::PresetConfig& preset : bladeArray.presets) {
      configOutput << "\t{ \"" << preset.dirs << "\", \"" << preset.track << "\"," << std::endl;
      if (preset.styles.size() > 0) {
        for (const wxString& style : preset.styles) {
          configOutput << "\t\t" << style << "," << std::endl;
        }
      } else configOutput << "\t\t," << std::endl;
      configOutput << "\t\t\"" << preset.name << "\"}";
      // If not the last one, add comma
      if (&bladeArray.presets[bladeArray.presets.size() - 1] != &preset) configOutput << ",";
      configOutput << std::endl;
    }
    configOutput << "};" << std::endl;
  }
}
void Configuration::outputConfigPresetsBlades(std::ofstream& configOutput, EditorWindow* editor) {
  configOutput << "BladeConfig blades[] = {" << std::endl;
  for (const BladeArrayDlg::BladeArray& bladeArray : editor->bladesPage->bladeArrayDlg->bladeArrays) {
    configOutput << "\t{ " << (bladeArray.name == "no_blade" ? "NO_BLADE" : std::to_string(bladeArray.value)) << "," << std::endl;
    for (const BladesPage::BladeConfig& blade : bladeArray.blades) {
      if (blade.type == BD_PIXELRGB || blade.type == BD_PIXELRGBW) {
        if (blade.isSubBlade) genSubBlades(configOutput, blade);
        else {
          configOutput << "\t\t";
          genWS281X(configOutput, blade);
          configOutput << "," << std::endl;
        }
      } else if (blade.type == BD_TRISTAR || blade.type == BD_QUADSTAR) {
        bool powerPins[4]{true, true, true, true};
        configOutput << "\t\tSimpleBladePtr<";
        if (blade.Star1 != BD_NORESISTANCE) configOutput << "CreeXPE2" << blade.Star1 << "Template<" << blade.Star1Resistance << ">, ";
        else {
          configOutput << "NoLED, ";
          powerPins[0] = false;
        }
        if (blade.Star2 != BD_NORESISTANCE) configOutput << "CreeXPE2" << blade.Star2 << "Template<" << blade.Star2Resistance << ">, ";
        else {
          configOutput << "NoLED, ";
          powerPins[1] = false;
        }
        if (blade.Star3 != BD_NORESISTANCE) configOutput << "CreeXPE2" << blade.Star3 << "Template<" << blade.Star3Resistance << ">, ";
        else {
          configOutput << "NoLED, ";
          powerPins[2] = false;
        }
        if (blade.Star4 != BD_NORESISTANCE && blade.type == BD_QUADSTAR) configOutput << "CreeXPE2" << blade.Star4 << "Template<" << blade.Star4Resistance << ">, ";
        else {
          configOutput << "NoLED, ";
          powerPins[3] = false;
        }

        int8_t usageIndex = 0;
        for (auto& usePowerPin : powerPins) {
          if (usePowerPin && usageIndex < static_cast<int8_t>(blade.powerPins.size())) {
            configOutput << blade.powerPins.at(usageIndex++);
          } else {
            configOutput << "-1";
          }

          if (&usePowerPin != &powerPins[3]) configOutput << ", ";
        }
        configOutput << ">()," << std::endl;
      } else if (blade.type == BD_SINGLELED) {
        configOutput << "\t\tSimpleBladePtr<CreeXPE2WhiteTemplate<550>, NoLED, NoLED, NoLED, ";
        configOutput << (blade.powerPins.size() > 0 ? blade.powerPins.at(0) : "-1");
        configOutput << ", -1, -1, -1>()," << std::endl;
      }
    }
    configOutput << "\t\tCONFIGARRAY(" << bladeArray.name << "), \"" << bladeArray.name << "\"" << std::endl << "\t}";
    if (&bladeArray != &editor->bladesPage->bladeArrayDlg->bladeArrays[editor->bladesPage->bladeArrayDlg->bladeArrays.size() - 1]) configOutput << ",";
    configOutput << std::endl;
  }
  configOutput << "};" << std::endl;
}
void Configuration::genWS281X(std::ofstream& configOutput, const BladesPage::BladeConfig& blade) {
  wxString bladePin = blade.dataPin;
  wxString bladeColor = blade.type == BD_PIXELRGB || blade.useRGBWithWhite ? blade.colorType : [=](wxString colorType) -> wxString { colorType.replace(colorType.find("W"), 1, "w"); return colorType; }(blade.colorType);

  configOutput << "WS281XBladePtr<" << blade.numPixels << ", " << bladePin << ", Color8::" << bladeColor << ", PowerPINS<";
  for (const auto& powerPin : blade.powerPins) {
    configOutput << powerPin << (&powerPin != &blade.powerPins.back() ? ", " : "");
  }
  configOutput << ">>()";
};
void Configuration::genSubBlades(std::ofstream& configOutput, const BladesPage::BladeConfig& blade) {
  int32_t subNum{0};
  for (const auto& subBlade : blade.subBlades) {
    if (blade.useStride) {
      configOutput << "\t\tSubBladeWithStride( ";
      configOutput << subNum << ", ";
      configOutput << blade.numPixels - blade.subBlades.size() + subNum << ", ";
      configOutput << blade.subBlades.size() << ", ";
    } else if (blade.useZigZag) {
      configOutput << "\t\tSubBladeZZ( ";
      configOutput << "0, ";
      configOutput << blade.numPixels - 1 << ", ";
      configOutput << blade.subBlades.size() << ", ";
      configOutput << subNum << ", ";
    } else {
      configOutput << "\t\tSubBlade( ";
      configOutput << subBlade.startPixel << ", " << subBlade.endPixel << ", ";
    }

    if (subNum == 0) {
      genWS281X(configOutput, blade);
      configOutput << ")," << std::endl;
    } else {
      configOutput << "NULL)," << std::endl;
    }

    subNum++;
  }
}
void Configuration::outputConfigButtons(std::ofstream& configOutput, EditorWindow* editor) {
  configOutput << "#ifdef CONFIG_BUTTONS" << std::endl;
  configOutput << "Button PowerButton(BUTTON_POWER, powerButtonPin, \"pow\");" << std::endl;
  if (editor->generalPage->buttons->entry()->GetValue() >= 2) configOutput << "Button AuxButton(BUTTON_AUX, auxPin, \"aux\");" << std::endl;
  if (editor->generalPage->buttons->entry()->GetValue() == 3) configOutput << "Button Aux2Button(BUTTON_AUX2, aux2Pin, \"aux\");" << std::endl; // figure out aux2 syntax
  configOutput << "#endif" << std::endl << std::endl; // CONFIG_BUTTONS
}

bool Configuration::readConfig(const std::string& filePath, EditorWindow* editor) {
  std::ifstream file(filePath);
  if (!file.is_open()) return false;

  try {
    std::string section;
    while (!file.eof()) {
      file >> section;
      if (section == "//") {
        getline(file, section);
        continue;
      }
      if (std::strstr(section.data(), "/*")) {
        while (!file.eof()) {
          if (std::strstr(section.data(), "*/")) break;
          file >> section;
        }
        continue;
      }
      if (section == "#ifdef") {
        file >> section;
        if (section == "CONFIG_TOP") Configuration::readConfigTop(file, editor);
        if (section == "CONFIG_PROP") Configuration::readConfigProp(file, editor);
        if (section == "CONFIG_PRESETS") Configuration::readConfigPresets(file, editor);
        if (section == "CONFIG_STYLES") Configuration::readConfigStyles(file, editor);
      }
    }
  } catch (std::exception& e) {
    std::string errorMessage = "There was an error parsing config, please ensure it is valid:\n\n";
    errorMessage += e.what();

    std::cerr << errorMessage << std::endl;
    return false;
  }

  //GeneralPage::update();
  editor->propsPage->update();
  editor->bladesPage->update();
  editor->presetsPage->update();

  return true;
}
bool Configuration::importConfig(EditorWindow* editor) {
  wxFileDialog configLocation(editor, "Choose ProffieOS Config File", "", "", "C Header Files (*.h)|*.h", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

  if (configLocation.ShowModal() == wxID_CANCEL) return false; // User Closed

  return Configuration::readConfig(configLocation.GetPath().ToStdString(), editor);
}

void Configuration::readConfigTop(std::ifstream& file, EditorWindow* editor) {
  std::string element;
  editor->settings->readDefines.clear();
  while (!file.eof() && element != "#endif") {
    file >> element;
    if (element == "//") {
      getline(file, element);
      continue;
    }
    if (std::strstr(element.data(), "/*")) {
      while (!file.eof()) {
        if (std::strstr(element.data(), "*/")) break;
        file >> element;
      }
      continue;
    }
    if (element == "#define" && !file.eof()) {
      getline(file, element);
      editor->settings->readDefines.push_back(element);
    } else if (element == "const" && !file.eof()) {
      getline(file, element);
      std::strtok(element.data(), "="); // unsigned int maxLedsPerStrip =
      element = std::strtok(nullptr, " ;");
      editor->generalPage->maxLEDs->entry()->SetValue(std::stoi(element));
    } else if (element == "#include" && !file.eof()) {
      file >> element;
      if (std::strstr(element.c_str(), "v1") != NULL) {
        editor->generalPage->board->entry()->SetSelection(0);
      } else if (std::strstr(element.c_str(), "v2") != NULL) {
        editor->generalPage->board->entry()->SetSelection(1);
      } else if (std::strstr(element.c_str(), "v3") != NULL) {
        editor->generalPage->board->entry()->SetSelection(2);
      }
    } else if (element == "//PROFFIECONFIG") {
      file >> element;
      if (element == "ENABLE_MASS_STORAGE") editor->generalPage->massStorage->SetValue(true);
      if (element == "ENABLE_WEBUSB") editor->generalPage->webUSB->SetValue(true);
    }
  }

  editor->settings->parseDefines(editor->settings->readDefines);
  for (const auto& define : editor->settings->readDefines) {
    auto key = Settings::ProffieDefine::parseKey(define);
    editor->generalPage->customOptDlg->addDefine(key.first, key.second);
  }
}
void Configuration::readConfigProp(std::ifstream& file, EditorWindow* editor) {
  std::string element;
  while (!file.eof() && element != "#endif") {
    file >> element;
    for (auto& prop : editor->propsPage->getLoadedProps()) {
      if (element.find(prop->getFileName()) != std::string::npos) {
        editor->propsPage->updateSelectedProp(prop->getName());
        for (auto define = editor->settings->readDefines.begin(); define < editor->settings->readDefines.end();) {
          std::istringstream defineStream(*define);
          std::string defineName{};
          double value{0};

          defineStream >> defineName;
          auto key = prop->getSettings().find(defineName);
          if (key == prop->getSettings().end()) {
            define++;
            continue;
          }

          defineStream >> value;
          key->second.setValue(value);
          define = editor->settings->readDefines.erase(define);
        }
        break;
      }
    }
  }
}
void Configuration::readConfigPresets(std::ifstream& file, EditorWindow* editor) {
  editor->bladesPage->bladeArrayDlg->bladeArrays.clear();
  std::string element;
  while (!file.eof() && element != "#endif") {
    file >> element;
    if (element == "//") {
      getline(file, element);
      continue;
    }
    if (std::strstr(element.data(), "/*")) {
      while (!file.eof()) {
        if (std::strstr(element.data(), "*/")) break;
        file >> element;
      }
      continue;
    }
    if (element == "Preset") readPresetArray(file, editor);
    if (element == "BladeConfig") readBladeArray(file, editor);
  }
}
void Configuration::readConfigStyles(std::ifstream& file, EditorWindow* editor) {
  std::string element;
  std::string styleName;
  std::string style;

  file >> element;
  while (!file.eof() && element != "#endif") {
    if (element == "using") {
      file >> element;
      styleName = element;

      file >> element; // Clear "="

      style = "";
      while (style.find(";") == std::string::npos) {
        file >> element;
        style += element;
      }
      style.erase(style.rfind(";")); // remove trailing ";"
      style.erase(std::remove(style.begin(), style.end(), '\n'), style.end()); // remove newlines

      // Remove potential StylePtr<> syntax
      if (style.find("StylePtr") != std::string::npos) {
        style.erase(style.rfind("StylePtr<"), 9);
        style.erase(style.find(">()"), 3);
      }

      Configuration::replaceStyles(styleName, style, editor);
    }
    file >> element;
  }
}
void Configuration::readPresetArray(std::ifstream& file, EditorWindow* editor) {
# define CHKSECT if (file.eof() || element == "#endif" || strstr(element.data(), "};") != NULL) return
# define RUNTOSECTION element.clear(); while (element != "{") { file >> element; CHKSECT; }

  editor->bladesPage->bladeArrayDlg->bladeArrays.push_back(BladeArrayDlg::BladeArray());
  BladeArrayDlg::BladeArray& bladeArray = editor->bladesPage->bladeArrayDlg->bladeArrays.at(editor->bladesPage->bladeArrayDlg->bladeArrays.size() - 1);

  char* tempData;
  std::string presetInfo;
  std::string element;
  std::string comment;
  element.clear();
  file >> element;
  bladeArray.name.assign(std::strtok(element.data(), "[]"));

  RUNTOSECTION;
  uint32_t preset = -1;
  bladeArray.presets.clear();
  while (!false) {
    presetInfo.clear();
    RUNTOSECTION;
    bladeArray.presets.push_back(PresetsPage::PresetConfig());
    preset++;

    while (std::strstr(presetInfo.data(), "}") == nullptr) {
      file >> element;
      CHKSECT;
      presetInfo.append(element);
    }

    // Directory
    element = presetInfo.substr(0, presetInfo.find(",")); // Get dir section
    presetInfo = presetInfo.substr(presetInfo.find(",") + 1); // increment presetInfo
    tempData = std::strtok(element.data(), ",\""); // Detokenize dir section
    bladeArray.presets[preset].dirs.assign(tempData == nullptr ? "" : tempData);

    // Same thing but for track
    element = presetInfo.substr(0, presetInfo.find(","));
    presetInfo = presetInfo.substr(presetInfo.find(",") + 1);
    tempData = std::strtok(element.data(), ",\"");
    bladeArray.presets[preset].track.assign(tempData == nullptr ? "" : tempData);

    // Deal with Fett's comments
    comment.clear();
    while (presetInfo.find("/*") != std::string::npos) {
      auto commentBegin = presetInfo.find("/*");
      auto commentLength = presetInfo.find("*/") - commentBegin + 2;
      comment += presetInfo.substr(commentBegin, commentLength);
      presetInfo.erase(commentBegin, commentLength);
    }

    // Read actual styles
    for (int32_t blade = 0; blade < editor->settings->numBlades; blade++) {
      if (presetInfo.find("&style_charging,") < presetInfo.find("Style")) {
        presetInfo = presetInfo.substr(presetInfo.find("&style_charging,") + 16 /* length of "&style_charging,"*/);
        bladeArray.presets[preset].styles.push_back("&style_charging");
      } else if (presetInfo.find("&style_pov") < presetInfo.find("Style")) {
        presetInfo = presetInfo.substr(presetInfo.find("&style_pov,") + 11 /* length of "&style_pov,"*/);
        bladeArray.presets[preset].styles.push_back("&style_pov");
      } else {
        element = presetInfo.substr(presetInfo.find("Style"), presetInfo.find("()") - presetInfo.find("Style") + 2); // Copy in next

        presetInfo = presetInfo.substr(presetInfo.find("()") + 2); // Increment
        bladeArray.presets[preset].styles.push_back(comment + element);
      }
    }

    // Name
    tempData = std::strtok(presetInfo.data(), ",\"}");
    bladeArray.presets[preset].name.assign(tempData == nullptr ? "" : tempData);
  }
# undef CHKSECT
# undef RUNTOSECTION
}
void Configuration::readBladeArray(std::ifstream& file, EditorWindow* editor) {
# define CHKSECT if (file.eof() || element == "#endif" || strstr(element.data(), "};") != NULL) return
# define RUNTOSECTION element.clear(); while (element != "{") { file >> element; CHKSECT; }
  // In future get detect val and presetarray association

  BladeArrayDlg::BladeArray bladeArray;
  std::string element;
  std::string data;
  int32_t tempNumBlades;
  RUNTOSECTION;

  while (true) {
    bladeArray = {};
    RUNTOSECTION;
    file >> element;
    element = std::strtok(element.data(), " ,");
    bladeArray.value = std::strstr(element.data(), "NO_BLADE") ? 0 : std::stoi(element);
    CHKSECT;
    bladeArray.blades.clear();
    tempNumBlades = editor->settings->numBlades;
    for (int32_t blade = 0; blade < tempNumBlades; blade++) {
      data.clear();

      do { // Gather entire blade data
        file >> element;
        CHKSECT;
        data.append(element);
      } while (std::strstr(data.data(), "),") == nullptr);

      if (std::strstr(data.data(), "SubBlade") != nullptr) {
        if (std::strstr(data.data(), "NULL") == nullptr) { // Top Level SubBlade
          bladeArray.blades.push_back(BladesPage::BladeConfig());
          if (std::strstr(data.data(), "WithStride")) bladeArray.blades.at(blade).useStride = true;
          if (std::strstr(data.data(), "ZZ")) bladeArray.blades.at(blade).useZigZag = true;
        } else { // Lesser SubBlade
          blade--;
          tempNumBlades--;
          // Switch to operating on previous blade
        }

        bladeArray.blades[blade].isSubBlade = true;
        std::strtok(data.data(), "("); // SubBlade(
        bladeArray.blades[blade].subBlades.push_back({ (uint32_t)std::stoi(std::strtok(nullptr, "(,")), (uint32_t)std::stoi(std::strtok(nullptr, " (,")) });
        data = std::strtok(nullptr, ""); // Clear out mangled data from strtok, replace with rest of data ("" runs until end of what's left)
        // Rest will be handled by WS281X "if"
      }
      if (std::strstr(data.data(), "WS281XBladePtr") != nullptr) {
        if (static_cast<int32_t>(bladeArray.blades.size()) - 1 != blade) bladeArray.blades.push_back(BladesPage::BladeConfig());
        data = std::strstr(data.data(), "WS281XBladePtr"); // Shift start to blade data, in case of SubBlade;

        std::strtok(data.data(), "<,"); // Clear WS281XBladePtr
        bladeArray.blades[blade].numPixels = std::stoi(std::strtok(nullptr, "<,"));
        bladeArray.blades[blade].dataPin = std::strtok(nullptr, ",");
        std::strtok(nullptr, ":"); // Clear Color8::
        element = std::strtok(nullptr, ":,"); // Set to color order;
        bladeArray.blades[blade].useRGBWithWhite = strstr(element.data(), "W") != nullptr;
        bladeArray.blades[blade].colorType.assign(element);

        std::strtok(nullptr, "<"); // Clear PowerPINS
        while (!false) {
          char* tempStore = std::strtok(nullptr, " ()<>,");
          if (tempStore == nullptr) break;
          bladeArray.blades[blade].powerPins.push_back(tempStore);
        }
        continue;
      }
      if (std::strstr(data.data(), "SimpleBladePtr") != nullptr) {
        bladeArray.blades.push_back(BladesPage::BladeConfig());
        uint32_t numLEDs = 0;
        auto getStarTemplate = [](const std::string& element) -> std::string {
          if (std::strstr(element.data(), "RedOrange") != nullptr) return "RedOrange";
          if (std::strstr(element.data(), "Amber") != nullptr) return "Amber";
          if (std::strstr(element.data(), "White") != nullptr) return "White";
          if (std::strstr(element.data(), "Red") != nullptr) return "Red";
          if (std::strstr(element.data(), "Green") != nullptr) return "Green";
          if (std::strstr(element.data(), "Blue") != nullptr) return "Blue";
          // With this implementation, RedOrange must be before Red
          return BD_NORESISTANCE;
        };

        std::strtok(data.data(), "<"); // Clear SimpleBladePtr and setup strtok

        element = std::strtok(nullptr, "<,");
        bladeArray.blades[blade].Star1.assign(getStarTemplate(element));
        if (bladeArray.blades[blade].Star1 != BD_NORESISTANCE) {
          numLEDs++;
          bladeArray.blades[blade].Star1Resistance = std::stoi(std::strtok(nullptr, "<>"));
        }
        element = std::strtok(nullptr, "<,");
        bladeArray.blades[blade].Star2.assign(getStarTemplate(element));
        if (bladeArray.blades[blade].Star2 != BD_NORESISTANCE) {
          numLEDs++;
          bladeArray.blades[blade].Star2Resistance = std::stoi(std::strtok(nullptr, "<>"));
        }
        element = std::strtok(nullptr, "<, ");
        bladeArray.blades[blade].Star3.assign(getStarTemplate(element));
        if (bladeArray.blades[blade].Star3 != BD_NORESISTANCE) {
          numLEDs++;
          bladeArray.blades[blade].Star3Resistance = std::stoi(std::strtok(nullptr, "<>"));
        }
        element = std::strtok(nullptr, "<, ");
        bladeArray.blades[blade].Star4.assign(getStarTemplate(element));
        if (bladeArray.blades[blade].Star4 != BD_NORESISTANCE) {
          numLEDs++;
          bladeArray.blades[blade].Star4Resistance = std::stoi(std::strtok(nullptr, "<>"));
        }

        if (numLEDs <= 2) bladeArray.blades[blade].type.assign(BD_SINGLELED);
        if (numLEDs == 3) bladeArray.blades[blade].type.assign(BD_TRISTAR);
        if (numLEDs >= 4) bladeArray.blades[blade].type.assign(BD_QUADSTAR);

        while (!false) {
          char* tempStore = std::strtok(nullptr, " ()<>,");
          if (tempStore == nullptr) break;
          if (strstr(tempStore, "-1")) break;
          bladeArray.blades[blade].powerPins.push_back(tempStore);
        }
      }
    }

    data.clear();
    do {
      file >> element;
      CHKSECT;
      data.append(element);
    } while (std::strstr(data.data(), ")") == nullptr);

    int32_t nameStart = data.find("CONFIGARRAY(") + 12;
    int32_t nameEnd = data.find(")");
    bladeArray.name.assign(data.substr(nameStart, nameEnd - nameStart));

    if (bladeArray.blades.empty()) bladeArray.blades.push_back(BladesPage::BladeConfig{});

    for (BladeArrayDlg::BladeArray& array : editor->bladesPage->bladeArrayDlg->bladeArrays) {
      if (array.name == bladeArray.name) {
        array.value = bladeArray.value;
        array.blades = bladeArray.blades;

        if (array.value == 0 && array.name != "no_blade") {
          array.name = "blade_in";
        }
      }
    }

    for (const auto& blade : bladeArray.blades) {
      for (const auto& powerPin : blade.powerPins) {
        if (editor->bladesPage->powerPins->FindString(powerPin) == wxNOT_FOUND) editor->bladesPage->powerPins->Append(powerPin);
      }
    }
  }


# undef CHKSECT
# undef RUNTOSECTION
}
void Configuration::replaceStyles(const std::string& styleName, const std::string& styleFill, EditorWindow* editor) {
  std::string styleCheck;
  for (PresetsPage::PresetConfig& preset : editor->bladesPage->bladeArrayDlg->bladeArrays[editor->bladesPage->bladeArray->entry()->GetSelection()].presets) {
    for (wxString& style : preset.styles) {
      styleCheck = (style.find(styleName) == std::string::npos) ? style : style.substr(style.find(styleName));
      while (styleCheck != style) {
        // If there are no comments in the style, we're fine.
        // if the start of the next comment comes before the end of a comment, we *should* be outside the comment, and we're good to go.
        // This potentially could be broken though...
        if (style.find("/*") == std::string::npos || styleCheck.find("/*") <= styleCheck.find("*/")) {
          style.replace(style.find(styleCheck), styleName.length(), styleFill);
        }
        styleCheck = styleCheck.find(styleName) == std::string::npos ? style : style.substr(styleCheck.find(styleName));
      }
    }
  }
}

bool Configuration::runPreChecks(EditorWindow* editor) {
  if (editor->bladesPage->bladeArrayDlg->enableDetect->GetValue() && editor->bladesPage->bladeArrayDlg->detectPin->entry()->GetValue() == "") {
    ERR("Blade Detect Pin cannot be empty.");
  }
  if (editor->bladesPage->bladeArrayDlg->enableID->GetValue() && editor->bladesPage->bladeArrayDlg->IDPin->entry()->GetValue() == "") {
    ERR("Blade ID Pin cannot be empty.");
  }
  if ([&]() { for (const BladeArrayDlg::BladeArray& array : editor->bladesPage->bladeArrayDlg->bladeArrays) if (array.name == "") return true; return false; }()) {
    ERR("Blade Array Name cannot be empty.");
  }
  if (editor->bladesPage->bladeArrayDlg->enableID->GetValue() && editor->bladesPage->bladeArrayDlg->mode->entry()->GetStringSelection() == BLADE_ID_MODE_BRIDGED && editor->bladesPage->bladeArrayDlg->pullupPin->entry()->GetValue() == "") {
    ERR("Pullup Pin cannot be empty.");
  }
  if (editor->bladesPage->bladeArrayDlg->enableDetect->GetValue() && editor->bladesPage->bladeArrayDlg->enableID->GetValue() && editor->bladesPage->bladeArrayDlg->IDPin->entry()->GetValue() == editor->bladesPage->bladeArrayDlg->detectPin->entry()->GetValue()) {
    ERR("Blade ID Pin and Blade Detect Pin cannot be the same.");
  }
  if ([&]() -> bool {
        auto getNumBlades = [](const BladeArrayDlg::BladeArray& array) {
          int32_t numBlades = 0;
          for (const BladesPage::BladeConfig& blade : array.blades) {
            blade.isSubBlade ? numBlades += blade.subBlades.size() : numBlades++;
          }
          return numBlades;
        };

        int32_t lastNumBlades = getNumBlades(editor->bladesPage->bladeArrayDlg->bladeArrays.at(0));
        for (const BladeArrayDlg::BladeArray& array : editor->bladesPage->bladeArrayDlg->bladeArrays) {
          if (getNumBlades(array) != lastNumBlades) return true;
          lastNumBlades = getNumBlades(array);
        }
        return false;
      }()) {
    ERR("All Blade Arrays must be the same length.\n\nPlease add/remove blades to make them equal");
  }

  for (auto& bladeArray : editor->bladesPage->bladeArrayDlg->bladeArrays) {
    for (uint32_t idx = 0; idx < bladeArray.blades.size(); idx++) {
      if (bladeArray.blades.at(idx).type == BD_QUADSTAR && bladeArray.blades.at(idx).powerPins.size() != 4) {
        ERR(BD_QUADSTAR " blade " + std::to_string(idx) + " in array \"" + bladeArray.name + "\" should have 4 power pins selected.");
      }
      if (bladeArray.blades.at(idx).type == BD_TRISTAR && bladeArray.blades.at(idx).powerPins.size() != 3) {
        ERR(BD_TRISTAR " blade " + std::to_string(idx) + " in array \"" + bladeArray.name + "\" should have 3 power pins selected.");
      }
      if (bladeArray.blades.at(idx).type == BD_SINGLELED && bladeArray.blades.at(idx).powerPins.size() != 1) {
        ERR(BD_SINGLELED " blade " + std::to_string(idx) + " in array \"" + bladeArray.name + "\" should have 1 power pin selected.");
      }
    }
    for (auto& preset : bladeArray.presets) {
      for (auto& style : preset.styles) {
        auto styleBegin = style.find("Style");
        auto styleEnd = style.find("()");
        if (styleBegin == std::string::npos || styleEnd == std::string::npos || styleBegin > styleEnd) {
          if (style == "&style_pov" || style == "&style_charging") continue;
          ERR("Malformed bladestyle in preset \"" + preset.name + "\" in blade array \"" + bladeArray.name + "\"");
        }
      }
    }
  }

  return true;
}

const Configuration::MapPair& Configuration::findInVMap(const Configuration::VMap& map, const std::string& search) {
  return *std::find_if(map.begin(), map.end(), [&](const MapPair& pair) { return (pair.second == search || pair.first == search); });
}
