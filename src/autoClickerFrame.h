#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class AutoClickerFrame: public wxFrame {
public:

	AutoClickerFrame();

private:
	void OnExit(wxCommandEvent &event);
	void OnSelectPoint(wxCommandEvent &event);
	void OnGoClicker(wxCommandEvent &event);
	void OnTriggerSetPoint(wxTimerEvent &event);
	void ExecuteClick(wxTimerEvent &event);
	void OnClose(wxCloseEvent& event);

	void UpdateCountFromInputs(wxFocusEvent &event);
	void UpdateDurationFromInputs(wxFocusEvent &event);

	wxBoxSizer* getSelectPointRow();
	wxBoxSizer* getClickParamsRow();
	wxBoxSizer* getGoBtnRow();

	int initialX;
	int initialY;
	int delay;
	int duration;
	int targetClicks;

	wxStaticText *selectPointTxt;
	wxButton *selectPointBtn;
	wxButton *goBtn;
	wxTimer *timer;
	wxTimer *clickStartTimer;
	wxTextCtrl* delayCtrl;
	wxTextCtrl* durationCtrl;
	wxTextCtrl* clickCountCtrl;
};

