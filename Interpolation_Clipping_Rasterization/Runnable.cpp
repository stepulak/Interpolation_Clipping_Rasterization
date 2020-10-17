#include "Runnable.h"
#include <sstream>

Runnable::Runnable(const BitmapFont& font, SDL_Window* w, SDL_Renderer* r)
    : m_font(font)
    , m_window(w)
    , m_renderer(r)
{
    if (!w) {
        throw std::runtime_error("Window is null");
    }
    if (!r) {
        throw std::runtime_error("Renderer is null");
    }
}

int Runnable::GetWindowWidth() const
{
    int w;
    SDL_GetWindowSize(m_window, &w, nullptr);
    return w;
}

int Runnable::GetWindowHeight() const
{
    int h;
    SDL_GetWindowSize(m_window, nullptr, &h);
    return h;
}

void Runnable::ClearRenderer()
{
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
    SDL_RenderClear(m_renderer);
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
}

bool Runnable::HandleKeyPress(const SDL_Keycode& kc)
{
    if (kc == SDLK_ESCAPE) {
        m_shouldQuit = true;
        return true;
    }
    return false;
}

SDL_Point Runnable::GetMousePosition() const
{
    SDL_Point p;
    SDL_GetMouseState(&p.x, &p.y);
    return p;
}

void Runnable::Run()
{
    uint ticks;

    while (!m_shouldQuit) {
        ticks = SDL_GetTicks();
        PollInputEvents();
        UpdateContent();
        ClearRenderer();
        DrawContent();
        SDL_RenderPresent(m_renderer);
        DelayAndCountDeltaTime(ticks, SDL_GetTicks());
    }
}

std::string Runnable::GetAppInfo() const
{
    return "[ESC] EXIT\n";
}

void Runnable::PollInputEvents()
{
    for (SDL_Event e; SDL_PollEvent(&e);) {
        if (e.type == SDL_QUIT) {
            m_shouldQuit = true;
        } else if (e.type == SDL_KEYDOWN) {
            HandleKeyPress(e.key.keysym.sym);
        } else if (e.type == SDL_MOUSEBUTTONDOWN) {
            HandleMouseClick(e.button.button, e.button.x, e.button.y);
        } else if (e.type == SDL_MOUSEBUTTONUP) {
            HandleMouseRelease(e.button.button, e.button.x, e.button.y);
        } else if (e.type == SDL_MOUSEMOTION) {
            HandleMouseMotion(e.motion.x, e.motion.y);
        }
    }
}

void Runnable::DelayAndCountDeltaTime(uint t1, uint t2)
{
    const auto frametime = static_cast<uint>(1000.0f / FPS);

    if ((t2 - t1) < frametime) {
        SDL_Delay(frametime - (t2 - t1));
        m_deltaTime = 1.0f / FPS;
    } else {
        m_deltaTime = (t2 - t1) / 1000.0f;
    }
}