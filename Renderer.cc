
#include "Renderer.h"
#include <d3dx9.h>

std::unique_ptr<Renderer> g_pRender = std::make_unique<Renderer>();

Renderer::Renderer()
{
	m_currentColour = D3DCOLOR_ARGB(255, 0, 0, 0);
	m_iCurrentFont = 0;
	m_iFontCount = 0;
}

void Renderer::SetDevice(IDirect3DDevice9* pDevice)
{
	m_pDevice = pDevice;
}

void Renderer::SetDrawColor(D3DCOLOR colour)
{
	m_currentColour = colour;
}

void Renderer::SetDrawColor(Color colour)
{
	m_currentColour = D3DCOLOR_ARGB(colour.a, colour.r, colour.g, colour.b);
}

void Renderer::SetDrawColor(int r, int g, int b, int a)
{
	m_currentColour = D3DCOLOR_ARGB(a, r, g, b);
}

void Renderer::SetCurrentFontID(int fontID)
{
	m_iCurrentFont = fontID;
}

static HANDLE handles[25];
static int iNextHandle = 0;
static auto initFont(HMODULE hModule, int data) -> void
{
	HRSRC pResource = FindResource(hModule, MAKEINTRESOURCE(data), RT_FONT);
	HANDLE pFontHandle;

	if (pResource != NULL)
	{
		HGLOBAL pMem = LoadResource(hModule, pResource);
		void* pData = LockResource(pMem);
		size_t iLen = SizeofResource(hModule, pResource);
		DWORD nFonts;

		pFontHandle = AddFontMemResourceEx(pData, iLen, NULL, &nFonts);

		if (pFontHandle != NULL)
		{
			handles[iNextHandle] = pFontHandle;
			++iNextHandle;
		}
	}
}

void Renderer::InitFontResources(HMODULE hModule)
{
	// Usage: initFont(hModule, IDR_FONTxx)
	// Example: initFont(hModule, IDR_FONT1)
}

void Renderer::FreeFontResources()
{
	for (int i = 0; i < iNextHandle; ++i)
	{
		if (handles[i] != NULL and handles[i] != nullptr)
			RemoveFontMemResourceEx(handles[i]);
	}
}

void Renderer::DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color, IDirect3DDevice9* dev)
{
	D3DRECT BarRect = { x, y, x + w, y + h };
	dev->Clear(1, &BarRect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, color, 0, 0);
}

void Renderer::DrawFilledRect(int x, int y, int w, int h, Color color, IDirect3DDevice9* dev)
{
	DrawFilledRect(x, y, w, h, D3DCOLOR_RGBA(color.r, color.g, color.b, color.a), dev);
}

void Renderer::DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color)
{
	D3DRECT BarRect = { x, y, x + w, y + h };
	m_pDevice->Clear(1, &BarRect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, color, 0, 0);
}

void Renderer::DrawFilledRect(int x, int y, int w, int h, Color color)
{
	DrawFilledRect(x, y, w, h, D3DCOLOR_RGBA(color.r, color.g, color.b, color.a));
}

void Renderer::DrawFilledRect(int x, int y, int w, int h)
{
	D3DRECT BarRect = { x, y, x + w, y + h };
	m_pDevice->Clear(1, &BarRect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, m_currentColour, 0, 0);
}

void Renderer::DrawRect(int x, int y, int w, int h, D3DCOLOR color, IDirect3DDevice9* dev)
{
	DrawLine(x, y, x + w, y, color, dev);			// Horizontal top
	DrawLine(x, y + h, x + w, y + h, color, dev);	// Horizontal bottom
	DrawLine(x, y, x, y + h, color, dev);			// Vertical right
	DrawLine(x + w, y, x + w, y + h, color, dev);	// Vertical right
}

void Renderer::DrawRect(int x, int y, int w, int h, Color color, IDirect3DDevice9* dev)
{
	DrawRect(x, y, w, h, D3DCOLOR_RGBA(color.r, color.g, color.b, color.a), dev);
}

void Renderer::DrawRect(int x, int y, int w, int h, D3DCOLOR color)
{
	DrawLine(x, y, x + w, y, color);			// Horizontal top
	DrawLine(x, y + h, x + w, y + h, color);	// Horizontal bottom
	DrawLine(x, y, x, y + h, color);			// Vertical right
	DrawLine(x + w, y, x + w, y + h, color);	// Vertical right
}

void Renderer::DrawRect(int x, int y, int w, int h, Color color)
{
	DrawRect(x, y, w, h, D3DCOLOR_RGBA(color.r, color.g, color.b, color.a));
}

void Renderer::DrawRect(int x, int y, int w, int h)
{
	DrawLine(x, y, x + w, y);			// Horizontal top
	DrawLine(x, y + h, x + w, y + h);	// Horizontal bottom
	DrawLine(x, y, x, y + h);			// Vertical right
	DrawLine(x + w, y, x + w, y + h);	// Vertical right
}

void Renderer::DrawLine(int x, int y, int x2, int y2, D3DCOLOR color, IDirect3DDevice9* dev)
{
	LPD3DXLINE pLine;
	D3DXCreateLine(dev, &pLine);

	D3DXVECTOR2 cLine[2];
	cLine[0].x = x;
	cLine[0].y = y;
	cLine[1].x = x2;
	cLine[1].y = y2;

	pLine->SetWidth(1);
	pLine->SetAntialias(FALSE);
	pLine->SetGLLines(FALSE);
	pLine->Begin();
	pLine->Draw(cLine, 2, color);
	pLine->End();
}

void Renderer::DrawLine(int x, int y, int x2, int y2, Color color, IDirect3DDevice9* dev)
{
	DrawLine(x, y, x2, y2, D3DCOLOR_RGBA(color.r, color.g, color.b, color.a), dev);
}

void Renderer::DrawLine(int x, int y, int x2, int y2, D3DCOLOR color)
{
	DrawLine(x, y, x2, y2, color, m_pDevice);
}

void Renderer::DrawLine(int x, int y, int x2, int y2, Color color)
{
	DrawLine(x, y, x2, y2, D3DCOLOR_RGBA(color.r, color.g, color.b, color.a));
}

void Renderer::DrawLine(int x, int y, int x2, int y2)
{
	DrawLine(x, y, x2, y2, m_currentColour, m_pDevice);
}

void Renderer::DrawSimpleText(int x, int y, const std::string& text, D3DCOLOR color, DWORD flags, int fontID)
{
	RECT BarRect = {};
	BarRect.left = x;
	BarRect.right = GetTextWidth(text, fontID); // TODO: fix wtf?
	BarRect.top = y;
	BarRect.bottom = BarRect.top + GetTextHeight(text, fontID); // TODO: ???

	DWORD flags_ = 0;
	if ((flags & TEXT_NO_CLIP) == TEXT_NO_CLIP)					flags_ |= DT_NOCLIP;
	if ((flags & TEXT_CENTER_X) == TEXT_CENTER_X)				flags_ |= DT_CENTER;
	if ((flags & TEXT_CENTER_Y) == TEXT_CENTER_Y)				flags_ |= DT_VCENTER;
	if ((flags & TEXT_JUSTIFY_TOP) == TEXT_JUSTIFY_TOP)			flags_ |= DT_TOP;
	if ((flags & TEXT_JUSTIFY_BOTTOM) == TEXT_JUSTIFY_BOTTOM)	flags_ |= DT_BOTTOM;
	if ((flags & TEXT_JUSTIFY_RIGHT) == TEXT_JUSTIFY_RIGHT)		flags_ |= DT_RIGHT;
	if ((flags & TEXT_JUSTIFY_LEFT) == TEXT_JUSTIFY_LEFT)		flags_ |= DT_LEFT;
	if ((flags & TEXT_RTL) == TEXT_RTL)							flags_ |= DT_RTLREADING;
	if ((flags & TEXT_TAB_EXPAND) == TEXT_TAB_EXPAND)			flags_ |= DT_EXPANDTABS;
	if ((flags & TEXT_SINGLE_LINE) == TEXT_SINGLE_LINE)			flags_ |= DT_SINGLELINE;

	LPD3DXFONT pFont = m_pFonts[fontID];
	pFont->DrawTextA(NULL, text.c_str(), -1, &BarRect, flags_, color);
}

void Renderer::DrawSimpleText(int x, int y, const std::string& text, Color color, DWORD flags, int fontID)
{
	DrawSimpleText(x, y, text, D3DCOLOR_RGBA(color.r, color.g, color.b, color.a), flags, fontID);
}

void Renderer::DrawSimpleText(int x, int y, const std::string& text, D3DCOLOR color, DWORD flags)
{
	DrawSimpleText(x, y, text, color, flags, m_iCurrentFont);
}

void Renderer::DrawSimpleText(int x, int y, const std::string& text, Color color, DWORD flags)
{
	DrawSimpleText(x, y, text, D3DCOLOR_RGBA(color.r, color.g, color.b, color.a), flags);
}

void Renderer::DrawSimpleText(int x, int y, const std::string& text, D3DCOLOR color)
{
	DrawSimpleText(x, y, text, color, 0, m_iCurrentFont);
}

void Renderer::DrawSimpleText(int x, int y, const std::string& text, Color color)
{
	DrawSimpleText(x, y, text, D3DCOLOR_RGBA(color.r, color.g, color.b, color.a));
}

void Renderer::DrawSimpleText(int x, int y, const std::string& text)
{
	DrawSimpleText(x, y, text, m_currentColour, 0, m_iCurrentFont);
}

void Renderer::DrawPixel(int x, int y, D3DCOLOR color)
{
	DrawLine(x, y, x, y, color);
}

void Renderer::DrawPixel(int x, int y, Color color)
{
	DrawPixel(x, y, D3DCOLOR_RGBA(color.r, color.g, color.b, color.a));
}

void Renderer::DrawPixel(int x, int y)
{
	DrawLine(x, y, x, y, m_currentColour);
}

void Renderer::DrawPoint(Point point, D3DCOLOR color)
{
	DrawLine(point.x, point.y, point.x, point.y, color);
}

void Renderer::DrawPoint(Point point, Color color)
{
	DrawPoint(point, D3DCOLOR_RGBA(color.r, color.g, color.b, color.a));
}

void Renderer::DrawPoint(Point point)
{
	DrawLine(point.x, point.y, point.x, point.y, m_currentColour);
}

template<int n>
void Renderer::DrawPoints(Point points[n], D3DCOLOR color)
{
	for (unsigned int i = 0; i < n; ++i)
		DrawPoint(points[i], color);
}

template<int n>
void Renderer::DrawPoints(Point points[n], Color color)
{
	for (unsigned int i = 0; i < n; ++i)
		DrawPoint(points[i], color);
}

template<int n>
void Renderer::DrawPoints(Point points[n])
{
	for (unsigned int i = 0; i < n; ++i)
		DrawPoint(points[i]);
}

int Renderer::GetTextWidth(const std::string& text)
{
	LPD3DXFONT pFont = m_pFonts[m_iCurrentFont];

	RECT BarRect = {};
	pFont->DrawTextA(NULL, text.c_str(), strlen(text.c_str()), &BarRect, DT_CALCRECT, D3DCOLOR_XRGB(0, 0, 0));
	return BarRect.right - BarRect.left;
}

int Renderer::GetTextWidth(const std::string& text, int fontID)
{
	LPD3DXFONT pFont = m_pFonts[fontID];

	RECT BarRect = {};
	pFont->DrawTextA(NULL, text.c_str(), strlen(text.c_str()), &BarRect, DT_CALCRECT, D3DCOLOR_XRGB(0, 0, 0));
	return BarRect.right - BarRect.left;
}

int Renderer::GetTextHeight(const std::string& text)
{
	LPD3DXFONT pFont = m_pFonts[m_iCurrentFont];

	RECT BarRect = {};
	pFont->DrawTextA(NULL, text.c_str(), strlen(text.c_str()), &BarRect, DT_CALCRECT, D3DCOLOR_XRGB(0, 0, 0));
	return BarRect.bottom - BarRect.top;
}

int Renderer::GetTextHeight(const std::string& text, int fontID)
{
	LPD3DXFONT pFont = m_pFonts[fontID];

	RECT BarRect = {};
	pFont->DrawTextA(NULL, text.c_str(), strlen(text.c_str()), &BarRect, DT_CALCRECT, D3DCOLOR_XRGB(0, 0, 0));
	return BarRect.bottom - BarRect.top;
}

int Renderer::RegisterFont(FontData data, IDirect3DDevice9* pDevice)
{
	if (D3DXCreateFontA(pDevice, data.size, 0, data.weight, 0, data.italic, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, data.facename.c_str(), &m_pFonts[m_iFontCount]) == S_OK)
	{
		++m_iFontCount;

		return m_iFontCount - 1;
	}

	return -1;
}
