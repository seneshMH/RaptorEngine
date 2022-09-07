#pragma once

#include "rtpch.h"

#include "Raptor/Core/core.h"
#include "Raptor/Event/Event.h"

namespace Raptor {
	struct WindowProps
	{
		std::string Title;
		unsigned int width;
		unsigned int height;

		WindowProps(const std::string& title = "Raptor Engine",
			unsigned int width = 1280, unsigned int height = 720)
			:Title(title), width(width), height(height)
		{

		}

	};

	class RAPTOR_API Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window(){}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVsync(bool enabled) = 0;
		virtual bool IsVsync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}
