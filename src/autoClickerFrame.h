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
	void OnTriggerSetPoint(wxTimerEvent &event);

	wxBoxSizer* getSelectPointRow();
	wxBoxSizer* getClickParamsRow();
	wxBoxSizer* getGoBtnRow();

	int initialX;
	int initialY;
	bool locationSet;
	int delay;
	int duration;
	int targetClicks;

	wxStaticText *selectPointTxt;
	wxButton *selectPointBtn;
	wxButton *goBtn;
	wxTimer *timer;
};

