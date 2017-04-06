#include "Utils.h"

#include <unordered_map>
#include <SDL2/SDL_image.h>
#include <ctime>
#include <cstdlib>
#include <memory>

namespace Utils {

	struct ColorPushBuffer {
		std::vector<SDL_Color> buffer;
		int index;

		ColorPushBuffer() : index(0) {}
	};

	// One single stack for each renderer
	static std::unordered_map<SDL_Renderer*, ColorPushBuffer> colorBuf;

	void PushColor(SDL_Renderer * r)
	{
		auto& buf = colorBuf[r];
		SDL_Color c;
		SDL_GetRenderDrawColor(r, &c.r, &c.g, &c.b, &c.a);
		buf.buffer.push_back(c);
		buf.index++;
	}

	void PopColor(SDL_Renderer * r)
	{
		auto& buf = colorBuf[r];
		if (buf.index == 0) {
			throw std::runtime_error("PopColor: no possible pop");
		}
		auto& c = buf.buffer[--buf.index];
		SDL_SetRenderDrawColor(r, c.r, c.g, c.b, c.a);
	}

	void InitWindowRenderer(const std::string& title, SDL_Window *& window, SDL_Renderer *& renderer, int winW, int winH)
	{
		window = SDL_CreateWindow(title.c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			winW, winH, SDL_WINDOW_SHOWN);

		if (window == nullptr) {
			throw std::runtime_error("InitWindowRenderer: window init failed");
		}

		renderer = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

		if (renderer == nullptr) {
			SDL_DestroyWindow(window);
			throw std::runtime_error("InitWindowRenderer: renderer init failed");
		}
	}

	void DestroyWindowRenderer(SDL_Window* window, SDL_Renderer* renderer)
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
	}

	SDL_Texture* LoadTexture(SDL_Renderer* rend, const std::string& file, int* texW, int* texH) {
		auto* surf = IMG_Load(file.c_str());
		if (surf == nullptr) {
			throw std::runtime_error("LoadTexture: Unable to load " + file);
		}

		if (texW) {
			*texW = surf->w;
		}
		if (texH) {
			*texH = surf->h;
		}

		auto* tex = SDL_CreateTextureFromSurface(rend, surf);
		if (tex == nullptr) {
			SDL_FreeSurface(surf);
			throw std::runtime_error("LoadTexture: Unable to create texture" + file);
		}

		SDL_FreeSurface(surf);
		
		// to make sure alpha channel is drawn properly
		SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
		return tex;
	}

	Vec2f FindLineIntersetion(Vec2f p1, Vec2f v1, Vec2f p2, Vec2f v2)
	{
		float nom = p1.y * v2.x - v2.y * (p1.x - p2.x) - p2.y * v2.x;
		float den = v1.x * v2.y - v1.y * v2.x;
		float t0 = nom / den;
		return Vec2f(p1.x + t0 * v1.x, p1.y + t0 * v1.y);
	}

	void InitRandom()
	{
		srand(static_cast<unsigned int>(time(nullptr)));
	}

	static float PointOrientation(const Vec2f& p, const Vec2f& t1, const Vec2f& t2)
	{
		return (p.x - t2.x) * (t1.y - t2.y) - (t1.x - t2.x) * (p.y - t2.y);
	}

	bool PointInTriangle(const Vec2f& p, const Vec2f& t1, const Vec2f& t2, const Vec2f& t3)
	{
		bool o1, o2, o3;
		o1 = PointOrientation(p, t1, t2) <= 0.f;
		o2 = PointOrientation(p, t2, t3) <= 0.f;
		o3 = PointOrientation(p, t3, t1) <= 0.f;
		return o1 == o2 && o2 == o3;
	}
	
	void DrawRectangle(SDL_Renderer* rend, const SDL_Rect& r, int borderThickness, bool borderInside)
	{
		int borderExpand = !borderInside ? borderThickness : 0;
		SDL_Rect borders[4];

		borders[0] = {
			r.x - borderExpand,
			r.y - borderExpand,
			r.w + borderExpand * 2,
			borderThickness
		};
		borders[1] = {
			r.x - borderExpand,
			r.y + r.h - borderThickness + borderExpand,
			r.w + borderExpand * 2,
			borderThickness
		};
		borders[2] = {
			r.x - borderExpand,
			r.y,
			borderThickness,
			r.h
		};
		borders[3] = {
			r.x + r.w - borderThickness + borderExpand,
			r.y,
			borderThickness,
			r.h
		};

		SDL_RenderFillRects(rend, borders, 4);
	}

	void DrawGrid(SDL_Renderer* rend, const SDL_Rect& gridArea, int cellSize)
	{
		// vertical lines
		for (int x = gridArea.x; x <= gridArea.x + gridArea.w; x += cellSize) {
			SDL_RenderDrawLine(rend, x, gridArea.y, x, gridArea.y + gridArea.h);
		}

		// horizontal lines
		for (int y = gridArea.y; y <= gridArea.y + gridArea.h; y += cellSize) {
			SDL_RenderDrawLine(rend, gridArea.x, y, gridArea.x + gridArea.w, y);
		}
	}
}
