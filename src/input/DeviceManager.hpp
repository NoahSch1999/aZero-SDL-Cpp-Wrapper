#pragma once
#include "DeviceController.hpp"
#include "devices/gamepad.hpp"
#include "devices/Keyboard.hpp"

namespace aZero::Input {
	using GamepadListener = DeviceListener<Gamepad>;
	using KeyboardListener = DeviceListener<Keyboard>;

	struct DeviceManager
	{
	public:
		GamepadListener ListenGamepad(const std::string& guid, ListenerCallbacks<Gamepad>&& callbacks)
		{
			auto& gamepad = m_Gamepads.at(guid);
			const DeviceListenerID id = m_NextListenerId++;
			gamepad->m_Listeners[id] = ListenerCallbacks<Gamepad>(std::forward<ListenerCallbacks<Gamepad>>(callbacks));
			return GamepadListener(gamepad, gamepad->m_Device, id, guid);
		}

		KeyboardListener ListenKeyboard(ListenerCallbacks<Keyboard>&& callbacks)
		{
			const DeviceListenerID id = m_NextListenerId++;
			m_Keyboard->m_Listeners[id] = ListenerCallbacks<Keyboard>(std::forward<ListenerCallbacks<Keyboard>>(callbacks));
			return KeyboardListener(m_Keyboard, m_Keyboard->m_Device, id, "");
		}

		void ReloadDevices()
		{
			m_NextListenerId = 0;

			m_Gamepads.clear();
			m_Keyboard.reset();
			// TODO: Release mice

			auto gamepads = Input::Gamepad::GetGamepads();
			for (auto id : gamepads)
			{
				Gamepad gamepad(id);
				std::string guid = gamepad.GetSDLGUID();
				m_Gamepads.emplace(std::move(guid), std::make_shared<DeviceController<Gamepad>>(std::move(gamepad)));
			}

			if (Input::Keyboard::GetKeyboards().size() > 0)
			{
				m_Keyboard = std::make_shared<DeviceController<Keyboard>>(Keyboard());
			}

			// TODO: Load mice
		}

		std::vector<std::string> GetGamepadGUIDS() const 
		{
			std::vector<std::string> guids;
			guids.reserve(m_Gamepads.size());
			for (const auto& [guid, controller] : m_Gamepads)
			{
				guids.emplace_back(guid);
			}
			return guids;
		}

	private:
		std::unordered_map<std::string, std::shared_ptr<DeviceController<Gamepad>>> m_Gamepads;
		std::shared_ptr<DeviceController<Keyboard>> m_Keyboard;
		DeviceListenerID m_NextListenerId = 0;
	};

	
}