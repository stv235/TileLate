#include "prec.hpp"

#include "MainFrame.hpp"

#include "TileLate.hpp"

IMPLEMENT_APP(TileLateApp)

bool TileLateApp::OnInit()
{
	wxFileName xrcPath(wxStandardPaths::Get().GetDataDir(), "TileLate.xrc");

#ifdef _DEBUG
	xrcPath.AppendDir("TileLate");
#endif

	xrcPath.AppendDir("xrc");

	auto res = wxXmlResource::Get();
	res->InitAllHandlers();

	if (res->Load(xrcPath.GetFullPath()))
	{
		MainFrame* mainFrame = new MainFrame();
		mainFrame->Show();

		return true;
	}

	return false;
}
