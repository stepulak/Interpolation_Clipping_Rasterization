#ifndef LINE_SEED_FILL_H
#define LINE_SEED_FILL_H

#include "FramebufferRunnable.h"
#include <queue>

class LineSeedFill : public FramebufferRunnable {
private:

	struct FillInfo {
		int x;
		int y;
		int xDirection;
		bool topExpanded;
		bool bottomExpanded;

		FillInfo(int x, int y, int xDirection, bool topExpanded, bool bottomExpanded)
		{
			this->x = x;
			this->y = y;
			this->xDirection = xDirection;
			this->topExpanded = topExpanded;
			this->bottomExpanded = bottomExpanded;
		}
	};

	static const float NEXT_STEP_TIME;

	std::queue<FillInfo> m_seedQueue;
	bool m_initSeedSet;
	float m_stepTimer;

	virtual bool HandleKeyPress(const SDL_Keycode& kc) override;
	virtual bool HandleMouseClick(Uint8 button, Sint32 x, Sint32 y) override;

	inline bool PointInGrid(int x, int y) const
	{ return x >= 0 && y >= 0 && x < static_cast<int>(FramebufferWidth()) && y < static_cast<int>(FramebufferHeight()); }

	inline bool CanContinueFill(int x, int y) const
	{ return PointInGrid(x, y) && GetColor(x, y) != FILL_COLOR; }

	// Try to expand filling in given position (if possible), or check, if another may happen soon
	void TryToExpandFilling(bool& expanded, int x, int y, bool parentTop);

	// return false if there is no more cell to fill, otherwise true
	bool PerformOneFill();

	void Clear();
	void DrawAppInfo() const;

public:

	LineSeedFill(SDL_Window* w, SDL_Renderer* r);
	virtual ~LineSeedFill() {}

	virtual void UpdateContent() override;
	virtual void DrawContent() const override;
};

#endif