#include "autoClickerFrame.h"

#include <wx/valnum.h>
#include <wx/config.h>

enum {
	ID_SELECT_POINT = 1, ID_TRIGGER_SET_POINT, ID_GO, ID_DOCLICK
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
	wxIntegerValidator<int> delayValidator(&delay);
	wxIntegerValidator<int> durationValidator(&duration);
	wxIntegerValidator<int> countValidator(&targetClicks);

    newSizer->Add(new wxStaticText(this, -1, "Click Delay:", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT), 1,wxALIGN_CENTER_VERTICAL,1);
    delayCtrl = new wxTextCtrl(this, -1, "", wxDefaultPosition, wxSize(60,20),0, delayValidator);
    newSizer->Add(delayCtrl, wxLeft, 1);
    newSizer->Add(new wxStaticText(this, -1, "  Duration:", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT), 1,wxALIGN_CENTER_VERTICAL,1);
    durationCtrl = new wxTextCtrl(this, -1, "", wxDefaultPosition, wxSize(60,20), 0, durationValidator);
    newSizer->Add(durationCtrl, wxLeft, 1);
    newSizer->Add(new wxStaticText(this, -1, "  Total Clicks:", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT), 1,wxALIGN_CENTER_VERTICAL,1);
    clickCountCtrl = new wxTextCtrl(this, -1, "", wxDefaultPosition, wxSize(60,20), 0, countValidator);
    newSizer->Add(clickCountCtrl, wxLeft, 1);
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
		wxFrame(NULL, wxID_ANY, "Clickerator"), initialX(0), initialY(0) {
	wxConfig *config = new wxConfig("Clickerator");
	duration = config->ReadLong("duration", 20);
	delay = config->ReadLong("delay", 40);
	targetClicks = duration * 1000/delay;
	delete config;

	wxMenu *menuFile = new wxMenu;
	menuFile->Append(wxID_EXIT);

	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(menuFile, "&File");

	SetMenuBar(menuBar);

	Bind(wxEVT_MENU, &AutoClickerFrame::OnExit, this, wxID_EXIT);
	Bind(wxEVT_BUTTON, &AutoClickerFrame::OnSelectPoint, this, ID_SELECT_POINT);
	Bind(wxEVT_BUTTON, &AutoClickerFrame::OnGoClicker, this, ID_GO);
	Bind(wxEVT_TIMER, &AutoClickerFrame::OnTriggerSetPoint, this,
			ID_TRIGGER_SET_POINT);
	Bind(wxEVT_TIMER, &AutoClickerFrame::ExecuteClick, this, ID_DOCLICK);
	Bind(wxEVT_CLOSE_WINDOW, &AutoClickerFrame::OnClose, this);

	wxBoxSizer *vSizer = new wxBoxSizer(wxVERTICAL);
	vSizer->Add(getSelectPointRow(), 1, wxEXPAND | wxALL, 10);
	vSizer->Add(getClickParamsRow(), 1, wxEXPAND | wxALL, 10);

	vSizer->Add(getGoBtnRow(), 1, wxALIGN_CENTER_HORIZONTAL, 5);
	vSizer->SetMinSize(wxSize(600, 100));
	SetSizerAndFit(vSizer);
	timer = new wxTimer(this, ID_TRIGGER_SET_POINT);
	clickStartTimer = new wxTimer(this, ID_DOCLICK);

	delayCtrl->Bind(wxEVT_KILL_FOCUS, &AutoClickerFrame::UpdateCountFromInputs, this);
	durationCtrl->Bind(wxEVT_KILL_FOCUS, &AutoClickerFrame::UpdateCountFromInputs, this);
	clickCountCtrl->Bind(wxEVT_KILL_FOCUS, &AutoClickerFrame::UpdateDurationFromInputs, this);
	TransferDataToWindow();
}

void AutoClickerFrame::OnExit(wxCommandEvent &event) {
	Close(true);
}

void AutoClickerFrame::OnClose(wxCloseEvent& event)
{
	wxConfig *config = new wxConfig("Clickerator");
	config->Write("duration", duration);
	config->Write("delay", delay);
	delete config;
	Destroy();
}

class TriggerEvtHandler: public wxEvtHandler {

};

void AutoClickerFrame::OnSelectPoint(wxCommandEvent &event) {
	selectPointTxt->SetLabelText("Move mouse to desired point.");
	if (timer == NULL) {
		timer = new wxTimer(this, ID_TRIGGER_SET_POINT);
	}
	timer->StartOnce(5 * 1000);
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

void AutoClickerFrame::OnGoClicker(wxCommandEvent &event)
{
	goBtn->Disable();
	wxPoint clientPos = ScreenToClient(wxPoint(initialX, initialY));
	WarpPointer(clientPos.x, clientPos.y);
	if (clickStartTimer == NULL) {
		clickStartTimer = new wxTimer(this, ID_DOCLICK);
	}
	clickStartTimer->StartOnce(100);
}

void AutoClickerFrame::ExecuteClick(wxTimerEvent &event)
{
	string cmd = string("xdotool click --delay ") + to_string(delay) + " --repeat " + to_string(targetClicks) + " 1";
	system(cmd.c_str());
	goBtn->Enable();
}

void AutoClickerFrame::UpdateCountFromInputs(wxFocusEvent &event)
{
	TransferDataFromWindow();
	targetClicks = duration * 1000/delay;
	TransferDataToWindow();
}

void AutoClickerFrame::UpdateDurationFromInputs(wxFocusEvent &event)
{
	TransferDataFromWindow();
	int estimateClicks = duration * 1000/delay;
	if (estimateClicks != targetClicks)
	{
	  duration = targetClicks * delay / 1000;
	}
	TransferDataToWindow();
}
