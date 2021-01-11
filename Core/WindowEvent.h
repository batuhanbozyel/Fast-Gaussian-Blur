#pragma once

#define BIND_EVENT_FN(fn) [](auto&& ... args) -> decltype(auto) { return fn(std::forward<decltype(args)>(args)...); }

namespace Core
{
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
							   virtual EventType GetEventType() const override { return GetStaticType(); }\
							   virtual const char* GetName() const override { return #type; }
	
	struct Event
	{
		virtual ~Event() = default;

		bool Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual std::string ToString() const { return GetName(); }
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& e)
			: m_Event(e) {}
		
		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	struct WindowCloseEvent : public Event
	{
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
	};

	struct WindowResizeEvent : public Event
	{
		uint32_t Width, Height;

		WindowResizeEvent(uint32_t width, uint32_t height)
			: Width(width), Height(height) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << Width << ", " << Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
	};

	struct WindowFocusEvent : public Event
	{
		WindowFocusEvent() = default;

		EVENT_CLASS_TYPE(WindowFocus)
	};

	struct WindowLostFocusEvent : public Event
	{
		WindowLostFocusEvent() = default;

		EVENT_CLASS_TYPE(WindowLostFocus)
	};

	struct WindowMovedEvent : public Event
	{
		int xPos, yPos;

		WindowMovedEvent(int _xPos, int _yPos)
			: xPos(_xPos), yPos(_yPos) {}

		EVENT_CLASS_TYPE(WindowMoved)
		
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}