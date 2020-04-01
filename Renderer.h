#pragma once

#include <string>
#include <memory>
#include <d3d9.h>
#include <d3dx9.h>

struct Point
{
	int x;
	int y;
};

struct Color
{
	__forceinline Color(int r, int g, int b, int a = 255)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	int r;
	int g;
	int b;
	int a = 255;
};

struct FontData
{
	int				size = 13;
	bool			italic = false;
	bool			antialias = true;
	unsigned int	weight = FW_NORMAL; // Normal - 400, Bold - 700
	std::string		facename = "Arial";
};

enum TextFlags
{
	TEXT_NO_CLIP,
	TEXT_CENTER_X,
	TEXT_CENTER_Y,
	TEXT_JUSTIFY_TOP,
	TEXT_JUSTIFY_BOTTOM,
	TEXT_JUSTIFY_RIGHT,
	TEXT_JUSTIFY_LEFT,
	TEXT_RTL,
	TEXT_TAB_EXPAND,
	TEXT_SINGLE_LINE
};

class Renderer
{
private:
	IDirect3DDevice9* m_pDevice;
	D3DCOLOR m_currentColour;
	LPD3DXFONT m_pFonts[15];
	int m_iCurrentFont;
	int m_iFontCount;

public:
	Renderer();

public:
	void SetDevice(IDirect3DDevice9* pDevice);
	void SetDrawColor(D3DCOLOR colour);
	void SetDrawColor(Color colour);
	void SetDrawColor(int r, int g, int b, int a = 255);
	void SetCurrentFontID(int fontID);

	void InitFontResources(HMODULE hModule);
	void FreeFontResources();

	void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color, IDirect3DDevice9* dev);
	void DrawFilledRect(int x, int y, int w, int h, Color color, IDirect3DDevice9* dev);
	void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color);
	void DrawFilledRect(int x, int y, int w, int h, Color color);
	void DrawFilledRect(int x, int y, int w, int h);

	void DrawRect(int x, int y, int w, int h, D3DCOLOR color, IDirect3DDevice9* dev);
	void DrawRect(int x, int y, int w, int h, Color color, IDirect3DDevice9* dev);
	void DrawRect(int x, int y, int w, int h, D3DCOLOR color);
	void DrawRect(int x, int y, int w, int h, Color color);
	void DrawRect(int x, int y, int w, int h);

	void DrawLine(int x, int y, int x2, int y2, D3DCOLOR color, IDirect3DDevice9* dev);
	void DrawLine(int x, int y, int x2, int y2, Color color, IDirect3DDevice9* dev);
	void DrawLine(int x, int y, int x2, int y2, D3DCOLOR color);
	void DrawLine(int x, int y, int x2, int y2, Color color);
	void DrawLine(int x, int y, int x2, int y2);

	void DrawSimpleText(int x, int y, const std::string& text, D3DCOLOR color, DWORD flags, int fontID);
	void DrawSimpleText(int x, int y, const std::string& text, Color color, DWORD flags, int fontID);
	void DrawSimpleText(int x, int y, const std::string& text, D3DCOLOR color, DWORD flags);
	void DrawSimpleText(int x, int y, const std::string& text, Color color, DWORD flags);
	void DrawSimpleText(int x, int y, const std::string& text, D3DCOLOR color);
	void DrawSimpleText(int x, int y, const std::string& text, Color color);
	void DrawSimpleText(int x, int y, const std::string& text);

	void DrawPixel(int x, int y, D3DCOLOR color);
	void DrawPixel(int x, int y, Color color);
	void DrawPixel(int x, int y);

	void DrawPoint(Point point, D3DCOLOR color);
	void DrawPoint(Point point, Color color);
	void DrawPoint(Point point);

	template <int n>
	void DrawPoints(Point points[n], D3DCOLOR color);
	template <int n>
	void DrawPoints(Point points[n], Color color);
	template <int n>
	void DrawPoints(Point points[n]);

	int GetTextWidth(const std::string& text);
	int GetTextWidth(const std::string& text, int fontID);

	int GetTextHeight(const std::string& text);
	int GetTextHeight(const std::string& text, int fontID);

	int RegisterFont(FontData data, IDirect3DDevice9* pDevice);
};

extern std::unique_ptr<Renderer> g_pRender;
