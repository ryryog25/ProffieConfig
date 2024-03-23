#include "styleedit.h"
#include "wx/log.h"
/*
 * ProffieConfig, All-In-One Proffieboard Management Utility
 * Copyright (C) 2024 Ryan Ogurek
 *
 * test/styleedit.cpp
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

#include <iostream>

#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/webview.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/tglbtn.h>
#include <wx/button.h>

static wxPanel* panel{nullptr};
static wxWebView* webview{nullptr};

enum {
    POWER,
    CLASH,
    STAB,
    BLAST
};

void loadPage() {
    webview->Reload();
    static_cast<wxToggleButton*>(panel->FindWindowById(POWER))->SetValue(false);

    auto size = webview->GetSize();
    webview->EnableContextMenu(false);
    webview->EnableHistory(false);
    webview->AddUserScript(R"(document.querySelectorAll('*').forEach(function(element){ element.style.margin = "0"; if (element !== document.getElementById("canvas_id") && !element.contains(document.getElementById("canvas_id"))) element.style.display = "none"; }))", wxWEBVIEW_INJECT_AT_DOCUMENT_END);
    webview->AddUserScript(R"(let canvas = document.getElementById("canvas_id");)", wxWEBVIEW_INJECT_AT_DOCUMENT_END);
    webview->AddUserScript("canvas.style.width = (canvas.width = " + std::to_string(size.GetWidth()) + ") + 'px';", wxWEBVIEW_INJECT_AT_DOCUMENT_END);
    webview->AddUserScript("canvas.style.height = (canvas.height = " + std::to_string(size.GetHeight()) + ") + 'px';", wxWEBVIEW_INJECT_AT_DOCUMENT_END);
    webview->AddUserScript(R"(width = canvas.width; height = canvas.height;)", wxWEBVIEW_INJECT_AT_DOCUMENT_END);
    webview->AddUserScript(R"(document.body.style.overflow = "hidden";)", wxWEBVIEW_INJECT_AT_DOCUMENT_END);
}

class MoveButton : public wxButton {
public:
    MoveButton(wxFrame* parent) : wxButton(parent, wxID_ANY, "Draggable"){
        this->parent = parent;
        Bind(wxEVT_LEFT_DOWN, &MoveButton::onMouseDown, this);
        Bind(wxEVT_MOTION, &MoveButton::onMove, this);
        Bind(wxEVT_MOUSE_CAPTURE_LOST, &MoveButton::onCaptureLost, this);
    }

    void onMouseDown(wxMouseEvent& event) {
        std::cout << "Capturing mouse" << std::endl;
        if (GetCapture() == this) return;
        CaptureMouse();
        isDragging = true;
        xPos = event.GetX();
        yPos = event.GetY();
    }
    void onMouseUp() {
        isDragging = false;
    }
    void onCaptureLost(wxMouseCaptureLostEvent&) {
        std::cout << "Lost Mouse" << std::endl;
    }
    void onMove(wxMouseEvent&) {
        if (isDragging) {
            auto mousePos = wxGetMousePosition();
            this->Move(GetParent()->ScreenToClient(wxPoint(mousePos.x - xPos, mousePos.y - yPos)));
        }
    }
private:
    wxFrame* parent{nullptr};
    volatile bool isDragging{false};
    int xPos;
    int yPos;
};

class TestFrame : public wxFrame {
public:
    TestFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
        Bind(wxEVT_LEFT_UP, &TestFrame::onMouseUp, this);
    }

    void onMouseUp(wxMouseEvent& event) {
        auto capture = GetCapture();
        if (capture) {
            std::cout << "Releasing" << std::endl;
            static_cast<MoveButton*>(capture)->onMouseUp();
            capture->ReleaseMouse();
        }
        else std::cout << "No capture" << std::endl;
    }
};


void StyleEdit::generatePage() {
    auto pageFrame = new TestFrame("Style Editor");
    // auto panel = new wxPanel(pageFrame, wxID_ANY);
    // auto sizer = new wxBoxSizer(wxVERTICAL);

    // webview = wxWebView::New(panel, wxID_ANY,"file:///home/ryanogurek/Documents/StyleEditor/style_editor.html?S=Layers<%0A++Layers<%0A++++Blue%2C%0A++++AudioFlickerL<DodgerBlue>>%2C%0A++BlastL<White>%2C%0A++LockupL<AudioFlicker<AudioFlicker<Blue%2CDodgerBlue>%2CWhite>>%2C%0A++SimpleClashL<White>%2C%0A++InOutHelperL<InOutFuncX<Int<300>%2CInt<800>>>>");
    // sizer->Add(webview, wxSizerFlags(1).Expand());

    // auto powButton = new wxToggleButton(panel, POWER, "Power");
    // sizer->Add(powButton);

    // auto clashButton = new wxButton(panel, CLASH, "Clash");
    // sizer->Add(clashButton);

    auto movableButton = new MoveButton(pageFrame);

    // panel->SetSizerAndFit(sizer);
    //panel->Bind(wxEVT_SIZE, [](wxSizeEvent& event){ event.Skip(); loadPage(); });
    // panel->Bind(wxEVT_TOGGLEBUTTON, [](wxCommandEvent&) { webview->RunScriptAsync("ClickPower()"); }, POWER);
    // panel->Bind(wxEVT_BUTTON, [](wxCommandEvent&) { webview->RunScriptAsync("AddClash()"); }, CLASH);

    pageFrame->Show();

}
