#include "prec.hpp"

#include "SpriteCtrl.hpp"

const wxColour BACKGROUND_COLOR1{ 200, 200, 200 };
const wxColour BACKGROUND_COLOR2{ 190, 190, 190 };
const wxColour OUTLINE_COLOR{ 0, 0, 255 };

SpriteCtrl::SpriteCtrl(wxWindow* parent)
	: wxControl(parent, wxID_ANY)
{
	Bind(wxEVT_PAINT, &SpriteCtrl::onPaint, this);
	Bind(wxEVT_SIZE, &SpriteCtrl::onResize, this);
}

SpriteCtrl::~SpriteCtrl()
{

}

void SpriteCtrl::onPaint(wxPaintEvent& evt)
{
	auto brush1 = wxTheBrushList->FindOrCreateBrush(BACKGROUND_COLOR1);
	auto brush2 = wxTheBrushList->FindOrCreateBrush(BACKGROUND_COLOR2, wxBRUSHSTYLE_CROSS_HATCH);
	auto outlinePen = wxThePenList->FindOrCreatePen(OUTLINE_COLOR, 1, wxPENSTYLE_DOT_DASH);

	auto r = GetClientRect();

	wxPaintDC dc(this);

	dc.SetPen(*wxTRANSPARENT_PEN);
	dc.SetBrush(*brush1);
	dc.DrawRectangle(r);
	dc.SetBrush(*brush2);
	dc.DrawRectangle(r);

	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	dc.SetPen(*outlinePen);

	for (const auto& frame : m_frames)
	{
		dc.DrawBitmap(frame.second, frame.first);

		
		dc.DrawRectangle(frame.first, frame.second.GetSize());
	}
}

void SpriteCtrl::onResize(wxSizeEvent& evt)
{
	Refresh();
}
