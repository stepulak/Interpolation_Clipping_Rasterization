#pragma once

#include "Runnable.h"

class AppSelector final : public Runnable {
public:
    AppSelector(const BitmapFont& font, SDL_Window* w, SDL_Renderer* r);

    void DrawContent() const override;

private:
    template <typename T>
    void RunApp() const
    {
        auto app = std::make_unique<T>(GetFont(), GetWindow(), GetRenderer());
        app->Run();
    }

    bool HandleKeyPress(const SDL_Keycode& kc) override;
    void DrawAppInfo() const;
};