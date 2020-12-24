#include "drawing.h"

struct Vertex {
	float_t x, y, z, rhw;
	D3DCOLOR color;
};

LPDIRECT3DDEVICE9 directx::operator<<(LPDIRECT3DDEVICE9 device, const rectangle_t& rect)
{
	Vertex vertices[4] =
	{
		{rect.x,  rect.y + rect.h, 0.0f, 0.0f, rect.color },
		{rect.x, rect.y, 0.0f, 0.1f, rect.color},
		{rect.x + rect.w, rect.y + rect.h,	0.0f, 0.0f, rect.color},
		{rect.x + rect.w, rect.y,	0.0f, 0.0f, rect.color}
	};

	device->SetTexture(0, NULL);
	device->SetPixelShader(0);
	device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX2);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_ZENABLE, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(Vertex));

	if (rect.border_color != eColor::None)
		rect.border(device, rect.x - 1, rect.y - 1, rect.w + 1, rect.h + 1, rect.thickness, rect.border_color);

	return device;
}

LPDIRECT3DDEVICE9 directx::operator<<(LPDIRECT3DDEVICE9 device, const circle_t& circle)
{
	const DWORD D3D_FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
	const int NUMPOINTS = 64;

	Vertex vertices[NUMPOINTS + 1];
	float X, Y, Theta, WedgeAngle;
	WedgeAngle = (2.0 * D3DX_PI) / (float_t)NUMPOINTS;

	for (int i = 0; i <= NUMPOINTS; i++)
	{
		Theta = i * WedgeAngle;
		X = (float)(circle.x + circle.radius * cos(Theta));
		Y = (float)(circle.y - circle.radius * sin(Theta));
		vertices[i] = Vertex{ X, Y, 0.0f, 1.0f, circle.color };
	}

	device->SetRenderState(D3DRS_ZENABLE, 0);
	device->SetRenderState(D3DRS_FOGENABLE, false);
	device->SetRenderState(D3DRS_LIGHTING, false);
	device->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	device->SetFVF(D3D_FVF);
	device->SetTexture(0, NULL);
	device->DrawPrimitiveUP(D3DPT_LINESTRIP, NUMPOINTS, vertices, sizeof(Vertex));
	device->SetTexture(0, NULL);

	return device;
}

/*
* class rectangle_t:
*	7 attributes:
*		position: x, y -> uint32_t
*		size: w, h -> uint32_t
*		border-thickness: thickness -> uint32_t
*		color: color, border_color -> D3DCOLOR
*/

directx::rectangle_t::rectangle_t() : x(0), y(0), w(0), h(0), thickness(1), color(eColor::None), border_color(eColor::None)
{
}

directx::rectangle_t::rectangle_t(std::uint32_t x, std::uint32_t y, std::uint32_t w, std::uint32_t h, D3DCOLOR color, D3DCOLOR border_color, std::uint32_t thickness) :
	x(x), y(y), w(w), h(h), color(color), border_color(border_color), thickness(thickness)
{
}

directx::rectangle_t::rectangle_t(D3DRECT rect, D3DCOLOR color, D3DCOLOR border_color, std::uint32_t thickness) :
	color(color), border_color(border_color), thickness(thickness)
{
	x = rect.x1;
	y = rect.y1;
	w = rect.x2 - rect.x1;
	h = rect.y2 - rect.y1;
}

void directx::rectangle_t::set_position(std::uint32_t x, std::uint32_t y)
{
	if (x != -1) this->x = x;
	if (y != -1) this->y = y;
}

void directx::rectangle_t::set_size(std::uint32_t w, std::uint32_t h)
{
	if (w != -1) this->w = w;
	if (h != -1) this->h = h;
}

void directx::rectangle_t::set_color(D3DCOLOR color, D3DCOLOR border_color)
{
	if (color != eColor::None) this->color = color;
	if (border_color != eColor::None) this->border_color = border_color;
}

void directx::rectangle_t::set_thickness(std::uint32_t thickness)
{
	this->thickness = thickness;
}

void directx::rectangle_t::fill(LPDIRECT3DDEVICE9 device, std::uint32_t x, std::uint32_t y, std::uint32_t w, std::uint32_t h, D3DCOLOR color)
{
	if (w < 0)w = 1;
	if (h < 0)h = 1;
	if (x < 0)x = 1;
	if (y < 0)y = 1;

	D3DRECT rec = { x, y, x + w, y + h };
	device->Clear(1, &rec, D3DCLEAR_TARGET, color, 0, 0);
}

void directx::rectangle_t::border(LPDIRECT3DDEVICE9 device, std::uint32_t x, std::uint32_t y, std::uint32_t w, std::uint32_t h, std::uint32_t thickness, D3DCOLOR color)
{
	fill(device, x, (y + h - thickness), w, thickness, color);
	fill(device, x, y, thickness, h, color);
	fill(device, x, y, w, thickness, color);
	fill(device, (x + w - thickness), y, thickness, h, color);
}

/*
* class circle _t:
*	6 attributes:
*		position: x, y -> uint32_t
*		radius:	radius -> uint32_t
*		border-thickness: thickness -> uint32_t
*		color: color, border_color -> D3DCOLOR
*/

directx::circle_t::circle_t() : x(0), y(0), radius(0), thickness(1), color(eColor::None), border_color(eColor::None)
{
}

directx::circle_t::circle_t(std::uint32_t x, std::uint32_t y, std::uint32_t radius, D3DCOLOR color, D3DCOLOR border_color, std::uint32_t thickness) :
	x(x), y(y), radius(radius), color(color), border_color(border_color), thickness(thickness)
{
}

void directx::circle_t::set_position(std::uint32_t x, std::uint32_t y)
{
	this->x = x;
	this->y = y;
}

void directx::circle_t::set_radius(std::uint32_t radius)
{
	this->radius = radius;
}

/*
* class font_t:
*	5 attributes:
*		font_family
*		height, width, weight
*		italic
*/

directx::font_t::font_t()
{
}

directx::font_t::font_t(std::string font_family, std::uint32_t font_height, std::uint32_t font_width, std::uint32_t font_weight, std::uint32_t italic) :
	font_family(font_family), font_height(font_height), font_width(font_width), font_weight(font_weight), italic(italic)
{
}

directx::font_t::~font_t()
{
	this->release();
}

bool directx::font_t::init(LPDIRECT3DDEVICE9 pDevice)
{
	if (font) return false;
	return SUCCEEDED(D3DXCreateFontA(pDevice, font_height, font_width, font_weight, 1, italic, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, font_family.c_str(), &font));
}

bool directx::font_t::release()
{
	if (font)
	{
		font->Release();
		font = NULL;
		return true;
	}
	return false;
}

void directx::font_t::write(std::uint32_t x, std::uint32_t y, D3DCOLOR color, std::uint32_t flags, std::string text, ...)
{
	RECT rect = { x,y,x,y };
	char buffer[1024] = { 0 };

	const char* c_text = text.c_str();

	va_list ap;
	va_start(ap, c_text);
	vsprintf_s(buffer, sizeof(buffer), c_text, ap);
	va_end(ap);

	font->DrawTextA(NULL, buffer, -1, &rect, flags, color);
}
