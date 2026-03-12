//#pragma once
//
//#include "SDLCppWrapper.hpp"
//#include <iostream>
//
//using namespace aZero;
//using namespace Window;
//using namespace Input;
//#undef max
//
//DeviceManager globDM;
//
//// Example of a window with custom event handling
//class RenderWindow : public aZero::Window::SDLWindow_Win32 {
//public:
//	RenderWindow() = default;
//	explicit RenderWindow(const aZero::Window::SDLWindowDesc& desc)
//		:SDLWindow_Win32(desc) {
//	}
//
//private:
//	KeyboardListener keyboardListener;
//
//	virtual void PollEventImpl(const SDL_Event& event) override {
//
//		// Window tests
//		if (event.key.type == SDL_EVENT_KEY_DOWN) {
//			if (event.key.key == SDLK_ESCAPE) {
//				this->Close(); // Close window
//			}
//		}
//
//		// Input tests
//		if (!globDM.m_Keyboard.get() || globDM.m_Gamepads.size() == 0)
//			return;
//
//		if (event.key.type == SDL_EVENT_KEY_DOWN) {
//			if (event.key.key == SDLK_ESCAPE) {
//				this->Close(); // Close window
//			}
//
//			// Overwrites the keyboardListener with new callbacks. The move-operator will remove the existing listener.
//			if (event.key.key == SDLK_BACKSPACE)
//			{
//				keyboardListener = globDM.ListenKeyboard({
//				[](const SDL_Event& event, Keyboard& keyboard) {
//						if (event.key.type == SDL_EVENT_KEY_UP)
//						{
//							std::cout << "UP!" << "\n";
//						}
//					},
//					[](const SDL_Event& event, Keyboard& keyboard) {std::cout << "OnDisconnect Keyboard\n"; }
//					});
//			}
//
//			// Overwrites the keyboardListener with an empty/invalid listener. The move-operator will remove the existing listener.
//			// This is the intended way to remove a listener from listening.
//			if (event.key.key == SDLK_RETURN)
//			{
//				keyboardListener = KeyboardListener();
//			}
//		}
//
//		if (event.key.type == SDL_EVENT_KEY_DOWN || event.key.type == SDL_EVENT_KEY_UP) {
//			for (auto& [id, callbacks] : globDM.m_Keyboard->m_Listeners) // Play keyboard listening events if key down or up
//			{
//				callbacks.m_OnEvent(event, globDM.m_Keyboard->m_Device);
//			}
//		}
//	}
//};
//
///*
//	OK:
//		Prints the callbacks for "validTest" gamepadlistener
//		Prints the callbacks for the keyboardlistener
//		Prints "Valid: 1" and "Invalid: 0"
//*/
//void Input_TestCallbacksAndLifespan()
//{
//	DeviceManager dm;
//	dm.m_Gamepads[""] = std::make_shared<DeviceController<Gamepad>>(DeviceController<Gamepad>());
//	dm.m_Gamepads["1"] = std::make_shared<DeviceController<Gamepad>>(DeviceController<Gamepad>());
//	dm.m_Keyboard = std::make_shared<DeviceController<Keyboard>>(DeviceController<Keyboard>());
//
//	KeyboardListener key1 = dm.ListenKeyboard({
//		[](const SDL_Event& event, Keyboard& keyboard) { std::cout << "OnEvent_Keyboard\n"; },
//		[](const SDL_Event& event, Keyboard& keyboard) {std::cout << "OnDisconnect_Keyboard1\n"; }
//		});
//
//	GamepadListener validTest = dm.ListenGamepad("",
//		{
//		[](const SDL_Event& event, Gamepad& gamepad) { std::cout << "OnEvent1\n"; },
//		[](const SDL_Event& event, Gamepad& gamepad) {std::cout << "OnDisconnect1\n"; }
//		}
//	);
//
//	GamepadListener invalidTest = dm.ListenGamepad("1",
//		{
//		[](const SDL_Event& event, Gamepad& gamepad) { std::cout << "OnEvent1\n"; },
//		[](const SDL_Event& event, Gamepad& gamepad) { std::cout << "OnDisconnect1\n"; }
//		}
//	);
//
//	{
//		auto gamepadController = dm.m_Gamepads[""]; // Fetching a controller and playing its listeners
//		for (auto& [id, callbacks] : gamepadController->m_Listeners)
//		{
//			callbacks.m_OnEvent(SDL_Event(), gamepadController->m_Device);
//			callbacks.m_OnDisconnect(SDL_Event(), gamepadController->m_Device);
//		}
//
//		for (auto& [id, callbacks] : dm.m_Keyboard->m_Listeners) // Playing the keyboards listeners
//		{
//			callbacks.m_OnEvent(SDL_Event(), dm.m_Keyboard->m_Device);
//			callbacks.m_OnDisconnect(SDL_Event(), dm.m_Keyboard->m_Device);
//		}
//	}
//
//	dm.m_Gamepads.erase("1"); // Remove gamepadcontroller from manager which invalides the invalidTest handle
//
//	std::cout << "Valid: " << validTest.IsValid() << "\n"; // Should be valid since the gamepadcontroller is still in the devicemanager
//	std::cout << "Invalid: " << invalidTest.IsValid() << "\n"; // Not valid because it was removed from the devicemanager
//}
//
///*
//	OK:
//		Should print the key that was pressed down
//		If backspace and return toggle if "UP!" should be printed when a key is let up
//*/
//void Input_TestAdvancedCallbacks()
//{
//	globDM.m_Gamepads[""] = std::make_shared<DeviceController<Gamepad>>(DeviceController<Gamepad>());
//	globDM.m_Gamepads["1"] = std::make_shared<DeviceController<Gamepad>>(DeviceController<Gamepad>());
//	globDM.m_Keyboard = std::make_shared<DeviceController<Keyboard>>(DeviceController<Keyboard>());
//
//	std::string referencedVariable;
//
//	KeyboardListener keyboardListener;
//	keyboardListener = globDM.ListenKeyboard({
//		[&referencedVariable](const SDL_Event& event, Keyboard& keyboard) {
//
//			// If event was key down, we store the key name in the referenced variable and print it
//			if (event.key.type == SDL_EVENT_KEY_DOWN)
//			{
//				referencedVariable = Keyboard::GetKeyName(event.key.key);
//				std::cout << referencedVariable << "\n";
//			}
//		},
//		[](const SDL_Event& event, Keyboard& keyboard) {std::cout << "OnDisconnect Keyboard\n"; }
//		});
//
//
//	RenderWindow window(SDLWindowDesc("MyWindow", { 0,0,800,600 }, { 255,255,0,0 }, SDL_WINDOW_RESIZABLE));
//
//	const HWND handle = window.GetNativeHandle(); // Use for swapchain creation
//
//	while (window.IsOpen()) {
//		window.PollEvents();
//		const std::string useReferencedVariable = referencedVariable;
//		//...
//	}
//}
//
///*
//	OK:
//		Window launches and can be exited with the exit button or ESC key
//*/
//void Window_TestWindowAndEvents()
//{
//	RenderWindow window(SDLWindowDesc("MyWindow", { 0,0,800,600 }, { 255,255,0,0 }, SDL_WINDOW_RESIZABLE));
//
//	const HWND handle = window.GetNativeHandle(); // Use for swapchain creation
//
//	while (window.IsOpen()) {
//		window.PollEvents();
//		//...
//	}
//}
//
//int main(int argc, char* argv[]) {
//	aZero::SDLCppWrapper::Init();
//
//	//Input_TestCallbacksAndLifespan();
//	//Input_TestAdvancedCallbacks();
//	//Window_TestWindowAndEvents();
//
//	aZero::SDLCppWrapper::Shutdown();
//
//	return 0;
//}