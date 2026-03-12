#include <iostream>
#include "SDLCppWrapper.hpp"

using namespace aZero;
using namespace Window;
using namespace Input;

class RenderWindow : public aZero::Window::SDLWindow_Win32 {
public:
	RenderWindow() = default;
	explicit RenderWindow(const aZero::Window::SDLWindowDesc& desc, DeviceManager& deviceManager)
		:SDLWindow_Win32(desc), di_DeviceManager(&deviceManager)
	{

	}

private:
	DeviceManager* di_DeviceManager;
	KeyboardListener m_KeyboardListener;
	GamepadListener m_GamepadListener;

	virtual void PollEventImpl(const SDL_Event& event) override {
		if (event.key.type == SDL_EVENT_KEY_DOWN) {
			if (event.key.key == SDLK_ESCAPE) {
				this->Close(); // Close window
			}
		}
	}
};


// API EXAMPLE
int main(int argc, char* argv[]) {
	aZero::SDLCppWrapper::Init();

	DeviceManager deviceManager;
	deviceManager.ReloadDevices();

	RenderWindow window(SDLWindowDesc("MyWindow", { 0,0,800,600 }, { 255,255,0,0 }, SDL_WINDOW_RESIZABLE), deviceManager);

	const HWND handle = window.GetNativeHandle(); // Use for swapchain creation
	
	const std::vector<std::string> connectedDevices = deviceManager.GetGamepadGUIDS();
	for (const auto& guid : connectedDevices)
	{
		std::cout << "Device: " << guid << "\n";
	}

	while (window.IsOpen()) {
		window.PollEvents();
		
	}

	aZero::SDLCppWrapper::Shutdown();

	return 0;
}