#pragma once

#include "rtpch.h"

#include "Raptor/Core/Base.h"
#include "Raptor/Event/Event.h"

namespace Raptor {
	struct WindowProps
	{
		std::string Title;
		uint32_t width;
		uint32_t height;

		WindowProps(const std::string& title = "Raptor Engine",
			uint32_t width = 1280, uint32_t height = 720)
			:Title(title), width(width), height(height)
		{

		}

	};

	class RAPTOR_API Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window(){}

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVsync(bool enabled) = 0;
		virtual bool IsVsync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};
}
