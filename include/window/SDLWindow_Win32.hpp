#pragma once
#include "SDLWindowBase.hpp"

#ifdef _WIN32
#include <Windows.h>

namespace aZero::Window {
    // Platform-specific definitions
    class SDLWindow_Win32 : public SDLWindowBase<HWND> {
    public:
        SDLWindow_Win32() = default;
        explicit SDLWindow_Win32(const Desc& desc)
            :SDLWindowBase<HWND>(desc) {}

        virtual ~SDLWindow_Win32() override = default;

        HWND GetNativeHandle() const final override {
            return (HWND)SDL_GetPointerProperty(
                SDL_GetWindowProperties(m_Window),
                SDL_PROP_WINDOW_WIN32_HWND_POINTER,
                NULL
            );
        }

        virtual void PollEventImpl(const SDL_Event& event) = 0;
    };
}
#endif