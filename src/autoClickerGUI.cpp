#include "autoClickerGUI.h"
#include "autoClickerFrame.h"

wxIMPLEMENT_APP(AutoClickerApp);

bool AutoClickerApp::OnInit() {
	AutoClickerFrame *frame = new AutoClickerFrame();
	frame->Show(true);
	return true;
}
