#ifndef RUNNABLE_H
#define RUNNABLE_H

#include "Utils.h"
#include "BitmapFont.h"
#include <memory>
#include <sstream>

// Base runnable class
class Runnable {
public:

	static const int FPS = 30;

private:

	// One common font for all Runnable classes
	static std::unique_ptr<BitmapFont> Font;

protected:

	template<typename ...Args>
	static inline void DrawText(Args&&... args) {
		if (Font.get() != nullptr) {
			Font->DrawLine(args...);
		}
		else {
			throw std::runtime_error("Font is not initialized");
		}
	}
	
public:

	static inline void SetupFont(std::unique_ptr<BitmapFont>& bf) { Font.swap(bf); }

private:

	// Handle SDL2 input
	void HandleInput();

	// Delay the app and count delta time
	void DelayAndCountDeltaTime(Uint32 ticks1, Uint32 ticks2);


	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	float m_deltaTime;
	bool m_shouldQuit;

protected:

	inline SDL_Window* GetWindow() const { return m_window; }
	inline SDL_Renderer* GetRenderer() const { return m_renderer; }
	inline float GetDeltaTime() const { return m_deltaTime; }
	int GetWindowWidth() const;
	int GetWindowHeight() const;

	void ClearRenderer();

	// implement ALL
	virtual bool HandleKeyPress(const SDL_Keycode& kc);

	inline virtual bool HandleMouseClick(Uint8 button, Sint32 x, Sint32 y) { return false; }
	inline virtual bool HandleMouseRelease(Uint8 button, Sint32 x, Sint32 y) { return false; }
	inline virtual bool HandleMouseMotion(Sint32 x, Sint32 y) { return false; }

	SDL_Point GetMousePosition() const;

	// Get application info about input handling, it's states etc...
	virtual std::stringstream GetAppInfo() const;

public:

	Runnable(SDL_Window* w, SDL_Renderer* r);
	virtual ~Runnable() {}

	void Start();

	inline virtual void UpdateContent() {}

	// Implement
	virtual void DrawContent() const = 0;
};

#endif