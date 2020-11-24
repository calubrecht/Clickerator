#include "autoClickerGUI.h"
#include "autoClickerFrame.h"

wxIMPLEMENT_APP(AutoClickerApp);

bool AutoClickerApp::OnInit() {
	std::string configName = "";
	if (argc > 1)
	{
		configName = argv[1];
	}
	AutoClickerFrame *frame = new AutoClickerFrame(configName);
	frame->Show(true);
	return true;
}
