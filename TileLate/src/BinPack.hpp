#pragma once

struct Anchor
{

};

struct Rect
{
	Rect() {}
	Rect(int px, int py, int pw, int ph) : x(px), y(py), w(pw), h(ph) {}

	std::size_t index = 0;

	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
};

class BinPack
{
public:
	std::vector<Rect> m_rects;

	std::pair<Rect, Rect> split(const Rect& r, const Rect& a)
	{
		if (a.h == std::numeric_limits<int>::max())
		{
			// Right
			Rect n1;
			n1.x = r.w;
			n1.y = a.y;
			n1.w = a.w - r.w;
			n1.h = r.h;

			// Bottom
			Rect n2;
			n2.x = a.x;
			n2.y = a.y + r.h;
			n2.w = a.w;
			n2.h = a.h;

			return std::make_pair(n1, n2);
		}
		else
		{
			// Right
			int a1 = (a.w - r.w) * a.h;
			// Bottom
			int a2 = (a.h - r.h) * a.w;

			// Right
			Rect n1;
			n1.x = a.x + r.w;
			n1.y = a.y;

			// Bottom
			Rect n2;
			n2.x = a.x;
			n2.y = a.y + r.h;

			// Right > Bottom
			if (a1 >= a2)
			{
				// Right
				n1.w = a.w - r.w;
				n1.h = a.h;

				// Bottom
				n2.w = r.w;
				n2.h = a.h - r.h;

				return std::make_pair(n2, n1);
			}
			else
			{
				// Right
				n1.w = a.w - r.w;
				n1.h = r.h;

				// Bottom
				n2.w = a.w;
				n2.h = a.h - r.h;

				return std::make_pair(n1, n2);
			}			
		}
	}

	int pack(int imp)
	{
		std::sort(m_rects.begin(), m_rects.end(), [](const Rect& r1, const Rect& r2) { return r1.h > r2.h; });

		int maxW = 0;
		int maxH = 0;

		std::vector<Rect> anchors;

		for (int i(0); i < imp; ++i)
		{
			maxW += m_rects[i].w;
		}

		anchors.push_back(Rect( 0, 0, maxW, std::numeric_limits<int>::max() ));

		for (auto& r : m_rects)
		{
			bool anchored = false;

			for (auto it = anchors.begin(); it != anchors.end(); ++it)
			{
				const auto& a = *it;

				if (r.w <= a.w && r.h <= a.h)
				{
					r.x = a.x;
					r.y = a.y;
					auto rest = split(r, a);
					*it = rest.first;
					anchors.insert(it + 1, rest.second);
					anchored = true;

					maxH = std::max(maxH, rest.first.y);
					maxH = std::max(maxH, rest.second.y);

					break;
				}
			}

			if (!anchored)
			{
				return -1;
			}
		}

		return maxW * maxH;
	}

	void pack()
	{
		std::sort(m_rects.begin(), m_rects.end(), [](const Rect& r1, const Rect& r2) { return r1.h > r2.h; });

		int bestA = std::numeric_limits<int>::max();
		int bestI = 0;

		for (int i(1); i < m_rects.size(); ++i)
		{
			int a = pack(i);

			if (a != -1 && a < bestA)
			{
				bestI = i;
				bestA = a;
			}
		}

		pack(bestI);

		/*
		int x = 0;

		for (auto& r : m_rects)
		{
			r.x = x;
			x += r.w;
		}
		*/
	}
};

