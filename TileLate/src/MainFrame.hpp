#pragma once

#include "SpriteCtrl.hpp"
#include "SpriteSheet.hpp"

class MainFrame : public wxFrame
{
public:
	MainFrame();
	
	virtual ~MainFrame() {}

private:
	void onAddFile(wxTreeEvent& evt);
	void pack();
	void onClickPack(wxCommandEvent& evt);
	void onClickExport(wxCommandEvent& evt);
	void exportSheet(const wxString& path);
	void cut(wxImage& img);

private:
	SpriteCtrl* m_spriteCtrl;

	wxGenericDirCtrl* m_dirsCtrl;

	SpriteSheet m_spriteSheet;
};

