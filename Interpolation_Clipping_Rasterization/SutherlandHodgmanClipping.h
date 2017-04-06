#ifndef SUTHERLAND_HODGMAN_CLIPPING_H
#define SUTHERLAND_HODGMAN_CLIPPING_H

#include "RasterGridRunnable.h"

class SutherlandHodgmanClipping : public RasterGridRunnable {
private:

	static const int CLIPPING_LINE_THICKNESS = 2;

	bool m_polygonInserted;
	bool m_clippingLineInserted;
	unsigned int m_clippingLineX;
	bool m_nextStep;
	unsigned int m_lastPoint;
	bool m_intersectionFound; // true if intersection was found
	bool m_pointClipped; // true if point was clipped
	bool m_drawClippingLine;

	bool HandleMouseClick(Uint8 button, Sint32 x, Sint32 y) override;
	bool HandleMouseMotion(Sint32 x, Sint32 y) override;
	bool HandleKeyPress(const SDL_Keycode& kc) override;

	void FindNextIntersection();
	void ClipNextPoint();

	void DrawAppInfo() const;
	void Clear();

public:

	SutherlandHodgmanClipping(SDL_Window* w, SDL_Renderer* r);
	virtual ~SutherlandHodgmanClipping() {}

	virtual void UpdateContent() override;
	virtual void DrawContent() const override;
};

#endif