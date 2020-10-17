#pragma once

#include "BitmapFont.h"
#include "Utils.h"
#include <memory>
#include <sstream>
#include <utility>

// Runnable class
// Implements basic SDL graphics, window and input manipulation
class Runnable {
public:
    static constexpr auto FPS = 24u;

    Runnable(const BitmapFont& font, SDL_Window* w, SDL_Renderer* r);

    virtual ~Runnable() = default;

    SDL_Window* GetWindow() const
    {
        return m_window;
    }

    SDL_Renderer* GetRenderer() const
    {
        return m_renderer;
    }

    const BitmapFont& GetFont() const
    {
        return m_font;
    }

    float GetDeltaTime() const
    {
        return m_deltaTime;
    }

    int GetWindowWidth() const;
    int GetWindowHeight() const;
    SDL_Point GetMousePosition() const;

    void Run();
    void ClearRenderer();

    virtual void UpdateContent() {}
    virtual void DrawContent() const = 0;
    virtual bool HandleKeyPress(const SDL_Keycode& kc);

    virtual bool HandleMouseClick(uint8_t button, int x, int y)
    {
        return false;
    }

    virtual bool HandleMouseRelease(uint8_t button, int x, int y)
    {
        return false;
    }

    virtual bool HandleMouseMotion(int x, int y)
    {
        return false;
    }

    // Get application info about input handling, it's states etc...
    virtual std::string GetAppInfo() const;

private:
    const BitmapFont& m_font;
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    float m_deltaTime = 0.f;
    bool m_shouldQuit = false;

    void PollInputEvents();
    void DelayAndCountDeltaTime(uint ticks1, uint ticks2);
};
