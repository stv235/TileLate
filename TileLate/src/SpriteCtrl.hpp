#pragma once

#include "SpriteSheet.hpp"

class SpriteCtrl : public wxControl
{
public:
	SpriteCtrl(wxWindow* parent);
	virtual ~SpriteCtrl();

	void setSpriteSheet(const SpriteSheet& spriteSheet)
	{
		m_frames = spriteSheet;
		Refresh();
	}

private:
	void onPaint(wxPaintEvent& evt);
	void onResize(wxSizeEvent& evt);

private:
	std::vector<std::pair<wxPoint, wxImage>> m_frames;
};

