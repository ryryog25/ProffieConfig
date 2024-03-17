#pragma once
/*
 * ProffieConfig, All-In-One Proffieboard Management Utility
 * Copyright (C) 2024 Ryan Ogurek
 *
 * ui/combobox.h
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/combobox.h>

namespace PCUI {

class ComboBox : wxPanel {
public:
    ComboBox(
        wxWindow* parent,
        int32_t id = wxID_ANY,
        const wxString& label = wxEmptyString,
        const wxSize& size = wxDefaultSize,
        const wxArrayString& choices = {},
        int32_t style = 0,
        const wxOrientation& orient = wxVERTICAL
        );

    void setToolTip(wxToolTip* tip);

    const wxComboBox* entry() const;
    const wxStaticText* text() const;

private:
    wxComboBox* mEntry{nullptr};
    wxStaticText* mText{nullptr};
};

}
