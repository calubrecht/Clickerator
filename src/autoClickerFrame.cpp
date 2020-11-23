#include "autoClickerFrame.h"

#include <vector>

enum {
	ID_SELECT_POINT = 1, ID_TRIGGER_SET_POINT, ID_GO
};

using namespace std;

wxBoxSizer* AutoClickerFrame::getSelectPointRow() {
	wxBoxSizer *newSizer = new wxBoxSizer(wxHORIZONTAL);
	selectPointTxt = new wxStaticText(this, -1, "No Point Selected");
	selectPointBtn = new wxButton(this, ID_SELECT_POINT,
			"Click to select Point");
	newSizer->Add(selectPointTxt, 1, wxEXPAND | wxALL, 1);
	newSizer->Add(selectPointBtn, 1, 0, 1);
	return newSizer;

}

wxBoxSizer* AutoClickerFrame::getClickParamsRow() {
	wxBoxSizer *newSizer = new wxBoxSizer(wxHORIZONTAL);

	return newSizer;

}

wxBoxSizer* AutoClickerFrame::getGoBtnRow() {
	wxBoxSizer *newSizer = new wxBoxSizer(wxHORIZONTAL);
	goBtn = new wxButton(this, ID_GO, "GO!");
	goBtn->Disable();
	newSizer->Add(goBtn, 1, wxALL, 1);
	return newSizer;

}

AutoClickerFrame::AutoClickerFrame() :
		wxFrame(NULL, wxID_ANY, "Clickerator"), initialX(0), initialY(0), locationSet(
				false), delay(40), targetClicks(1000), timer(NULL) {
	duration = targetClicks * delay / 1000;
	wxMenu *menuFile = new wxMenu;
	menuFile->Append(wxID_EXIT);

	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(menuFile, "&File");

	SetMenuBar(menuBar);

	Bind(wxEVT_MENU, &AutoClickerFrame::OnExit, this, wxID_EXIT);
	Bind(wxEVT_BUTTON, &AutoClickerFrame::OnSelectPoint, this, ID_SELECT_POINT);
	Bind(wxEVT_TIMER, &AutoClickerFrame::OnTriggerSetPoint, this,
			ID_TRIGGER_SET_POINT);

	wxBoxSizer *vSizer = new wxBoxSizer(wxVERTICAL);
	vSizer->Add(getSelectPointRow(), 1, wxEXPAND | wxALL, 10);
	vSizer->Add(getClickParamsRow(), 1, wxEXPAND | wxALL, 10);

	vSizer->Add(getGoBtnRow(), 1, wxALIGN_CENTER_HORIZONTAL, 5);
	vSizer->SetMinSize(wxSize(600, 100));
	SetSizerAndFit(vSizer);

}

void AutoClickerFrame::OnExit(wxCommandEvent &event) {
	Close(true);
}

class TriggerEvtHandler: public wxEvtHandler {

};

void AutoClickerFrame::OnSelectPoint(wxCommandEvent &event) {
	selectPointTxt->SetLabelText("Move mouse to desired point.");
	if (timer == NULL) {
		timer = new wxTimer(this, ID_TRIGGER_SET_POINT);
	}
	timer->StartOnce(10 * 1000);
}

void AutoClickerFrame::OnTriggerSetPoint(wxTimerEvent &event) {
	wxPoint betterPoint = wxGetMousePosition();
	initialX = betterPoint.x;
	initialY = betterPoint.y;
	selectPointTxt->SetLabelText(
			"Click at (" + to_string(initialX) + "," + to_string(initialY)
					+ ")");
	goBtn->Enable();
}
