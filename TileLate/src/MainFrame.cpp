#include "prec.hpp"

#include "BinPack.hpp"

#include "MainFrame.hpp"

MainFrame::MainFrame()
{
	auto res = wxXmlResource::Get();

	res->LoadFrame(this, nullptr, "MAIN_FRAME");

	m_dirsCtrl = XRCCTRL(*this, "DIRS", wxGenericDirCtrl);

	// Custom controls
	m_spriteCtrl = new SpriteCtrl(this);
	res->AttachUnknownControl("SPRITE", m_spriteCtrl, this);

	// Events
	Bind(wxEVT_DIRCTRL_FILEACTIVATED, &MainFrame::onAddFile, this, XRCID("DIRS"));
	Bind(wxEVT_MENU, &MainFrame ::onClickPack, this, XRCID("PACK"));
	Bind(wxEVT_MENU, &MainFrame::onClickExport, this, XRCID("EXPORT"));

	auto picturesDir = wxStandardPaths::Get().GetUserDir(wxStandardPathsBase::Dir_Pictures);

	m_dirsCtrl->SetPath(picturesDir);

	for (auto& sprite : m_spriteSheet)
	{
		cut(sprite.second);
	}

	pack();

	m_spriteCtrl->setSpriteSheet(m_spriteSheet);

	SetSize(wxSize(800, 600));
	Centre();
}

void MainFrame::cut(wxImage& img)
{
	// Inefficient
	int top = std::numeric_limits<int>::max();
	int left = top;

	int bottom = std::numeric_limits<int>::min();
	int right = bottom;

	for (int y(0); y < img.GetHeight(); ++y)
	{
		for (int x(0); x < img.GetWidth(); ++x)
		{
			if (img.GetAlpha(x, y) != wxIMAGE_ALPHA_TRANSPARENT)
			{
				top = std::min(top, y);
				left = std::min(left, x);

				bottom = std::max(bottom, y);
				right = std::max(right, x);
			}
		}
	}

	wxRect r;
	r.x = left;
	r.y = top;
	r.width = right - left;
	r.height = bottom - top;

	img = img.GetSubImage(r);
}

void MainFrame::onAddFile(wxTreeEvent& evt)
{
	auto path = m_dirsCtrl->GetPath();

	wxImage img;

	if (img.LoadFile(path))
	{
		m_spriteSheet.push_back(std::make_pair(wxPoint{}, img));

		pack();

		m_spriteCtrl->setSpriteSheet(m_spriteSheet);
	}
	else
	{
		wxMessageBox("Could not load file: " + path);
	}
}

void MainFrame::pack()
{
	BinPack bp;

	for (std::size_t i(0); i < m_spriteSheet.size(); ++i)
	{
		const auto& img = m_spriteSheet[i].second;

		Rect r;
		r.index = i;
		r.w = img.GetWidth();
		r.h = img.GetHeight();

		bp.m_rects.push_back(r);
	}

	bp.pack();

	for (const auto& r : bp.m_rects)
	{
		m_spriteSheet[r.index].first.x = r.x;
		m_spriteSheet[r.index].first.y = r.y;
	}

	m_spriteCtrl->setSpriteSheet(m_spriteSheet);
}

void MainFrame::onClickPack(wxCommandEvent& evt)
{
	pack();
}

void MainFrame::onClickExport(wxCommandEvent& evt)
{
	wxFileDialog dlg(this);
	dlg.SetWildcard("PNG files (*.png)|*.png");
	dlg.SetWindowStyle(wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_PREVIEW);

	if (dlg.ShowModal() == wxID_OK)
	{
		exportSheet(dlg.GetPath());
	}
}

void MainFrame::exportSheet(const wxString& path)
{
	if (!m_spriteSheet.empty())
	{
		wxSize textureSize;

		for (const auto& frame : m_spriteSheet)
		{
			auto bottomRight = frame.first + frame.second.GetSize();

			textureSize.x = std::max(textureSize.x, bottomRight.x);
			textureSize.y = std::max(textureSize.y, bottomRight.y);
		}

		wxImage outImg(textureSize);
		outImg.SetAlpha();

		for (const auto& frame : m_spriteSheet)
		{
			outImg.Paste(frame.second, frame.first.x, frame.first.y);
		}

		outImg.SaveFile(path, wxBITMAP_TYPE_PNG);
	}
	else
	{
		wxMessageBox("The current sheet is empty");
	}
}