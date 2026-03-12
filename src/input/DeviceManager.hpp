#pragma once
#include <unordered_map>
#include <string>
#include <functional>
#include <memory>
#include <limits>
#include "devices/gamepad.hpp"
#include "devices/Keyboard.hpp"

namespace aZero::Input {
	// Forward declarations
	template<typename DeviceType>
	struct DeviceController;

	class DeviceManager;

	// Aliases
	using ListenerID = uint32_t;

	template<typename DeviceType>
	using ListenerCallback = std::function<void(const SDL_Event&, DeviceType&)>;

	// Globals
	static constexpr ListenerID INVALID_LISTENING_ID = std::numeric_limits<ListenerID>::max();

	// Class definitions
	template<typename DeviceType>
	struct Listener
	{
		friend class DeviceManager;
		friend class DeviceController<DeviceType>;
	public:
		Listener(const Listener&) = delete;
		Listener& operator=(const Listener&) = delete;

		Listener() = default;
		Listener(std::weak_ptr<DeviceController<DeviceType>> manager, DeviceType& device, ListenerID id, const std::string& guid)
			:m_Controller(manager), m_Device(&device), m_Id(id), m_Guid(guid) {
		}

		Listener(Listener&& other) noexcept
		{ 
			*this = std::move(other);
		}

		Listener& operator=(Listener&& other) noexcept
		{
			std::swap(m_Controller, other.m_Controller);
			std::swap(m_Device, other.m_Device);
			std::swap(m_Guid, other.m_Guid);
			std::swap(m_Id, other.m_Id);
			return *this;
		}

		// Forward declare since we need to use DeviceController<DeviceType> in the destructor
		~Listener();

		bool IsValid() const
		{
			return !m_Controller.expired();
		}

		std::string GetDeviceGUID() const { return m_Guid; }
		DeviceType* GetDevice() const { return m_Device; }

	private:
		ListenerID m_Id = INVALID_LISTENING_ID;
		DeviceType* m_Device = nullptr;
		std::string m_Guid;
		std::weak_ptr<DeviceController<DeviceType>> m_Controller;

		void Invalidate()
		{
			m_Device = nullptr;
			m_Controller = std::weak_ptr<DeviceController<DeviceType>>();
			m_Id = INVALID_LISTENING_ID;
			m_Guid = "";
		}
	};

	template<typename DeviceType>
	struct DeviceController
	{
		friend class Listener<DeviceType>;
		friend class DeviceManager;
	public:
		struct Callbacks
		{
			ListenerCallback<DeviceType> m_OnEvent;
			ListenerCallback<DeviceType> m_OnDisconnect;
			Callbacks() = default;
			Callbacks(ListenerCallback<DeviceType>&& onEvent, ListenerCallback<DeviceType>&& onDisconnect)
				:m_OnEvent(std::move(onEvent)), m_OnDisconnect(std::move(onDisconnect))
			{

			}
		};
		DeviceController() = default;

		DeviceController(DeviceType&& device)
			:m_Device(std::move(device))
		{

		}

	private:
		DeviceType m_Device;
		std::unordered_map<ListenerID, Callbacks> m_Listeners;

		

		void StopListen(Listener<DeviceType>& listener)
		{
			if (listener.IsValid())
			{
				m_Listeners.erase(listener.m_Id);
			}
		}
	};

	using GamepadListener = Listener<Gamepad>;
	using KeyboardListener = Listener<Keyboard>;

	struct DeviceManager
	{
	public:
		GamepadListener ListenGamepad(const std::string& guid, DeviceController<Gamepad>::Callbacks&& callbacks)
		{
			auto& gamepad = m_Gamepads.at(guid);
			const ListenerID id = m_NextListenerId++;
			gamepad->m_Listeners[id] = DeviceController<Gamepad>::Callbacks(std::forward<DeviceController<Gamepad>::Callbacks>(callbacks));
			return GamepadListener(gamepad, gamepad->m_Device, id, guid);
		}

		KeyboardListener ListenKeyboard(DeviceController<Keyboard>::Callbacks&& callbacks)
		{
			const ListenerID id = m_NextListenerId++;
			m_Keyboard->m_Listeners[id] = DeviceController<Keyboard>::Callbacks(std::forward<DeviceController<Keyboard>::Callbacks>(callbacks));
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
		ListenerID m_NextListenerId = 0;
	};

	template<typename DeviceType>
	aZero::Input::Listener<DeviceType>::~Listener<DeviceType>()
	{
		if (!m_Controller.expired())
		{
			auto shared = m_Controller.lock();
			shared->StopListen(*this);
		}
	}
}