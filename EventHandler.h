#pragma once

#include <bitset>
#include <queue>

#include "Winbrew.h"

namespace handlers {
	struct MouseHandlerData
	{
		bool LMB = false;
		bool RMB = false;
	};

	class MouseEvent
	{
	public:
		enum class Type
		{
			LMB_DOWN,
			LMB_UP,
			RMB_DOWN,
			RMB_UP,
			WHEEL_UP,
			WHEEL_DOWN,
			MOVE,
			ENTER,
			LEAVE,
			INVALID
		};

		MouseEvent();
		MouseEvent(Type type, MouseHandlerData parent, int x, int y, int delta);

		Type getType() const;
		bool isValid() const;
		bool isPressDown() const;
		bool isPressUp() const;
		bool isMouseWheel() const;
		bool isLMBevent() const;
		bool isRMBevent() const;
		bool isLMB() const;
		bool isRMB() const;
		bool isMove() const;
		int getX() const;
		int getY() const;
		void getPos(int* x, int* y) const;
	private:
		Type type;
		bool LMB;
		bool RMB;
		int x;
		int y;
		int delta;
	};

	class MouseHandler
	{
		friend class EventHandler;
	public:
		void onMousePress(bool LMB, bool RMB, int x, int y);
		void onMouseRelease(bool LMB, bool RMB, int x, int y);
		void onMouseMove(int x, int y);
		void clearState();
		void clearBuff();
		void trimBuffer(unsigned char size);

		void onMouseEnter();
		void onMouseLeave();

		bool LMB = false;
		bool RMB = false;
		int x = 0;
		int y = 0;

		MouseEvent read();
		bool empty();

		bool isInWindow = false;
	private:
		std::queue<MouseEvent> mouseBuff;
	};

	class KeyEvent
	{
	public:
		enum class Type
		{
			KEY_PRESS,
			KEY_RELEASE,
			KEY_INVALID
		};

		KeyEvent();
		KeyEvent(Type type, unsigned char keyCode);

		bool isPress() const;
		bool isRelease() const;
		bool isValid() const;
		unsigned char getCode();
	private:
		Type type;
		unsigned char keyCode;
	};

	class KeyHandler
	{
	friend class EventHandler;
	public:
		void onKeyPressed(unsigned char keyCode);
		void onKeyReleased(unsigned char keyCode);
		bool keyPressed(unsigned char keyCode) const;
		void clearState();
		void clearBuff();
		void onChar(unsigned char keyCode);
		void trimBuffer(unsigned char size);

		KeyEvent read();
		bool empty();
	private:
		bool doAutorepeat = false;
		static constexpr unsigned int numberOfKeys = 256u;
		std::bitset<numberOfKeys> keystates;
		std::queue<KeyEvent> keyBuff;
	};

	class EventHandler {
	public:
		LRESULT HandleMSG(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam, int width, int height);
		bool keyPressed(unsigned char keyCode) const;

		KeyHandler kbd;
		MouseHandler mouse;
	private:

	};
}