#pragma once

#include <bitset>
#include <queue>

#include "Winbrew.h"

namespace handlers {
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
		void onChar(unsigned char keyCode);
		void trimBuffer(unsigned char size);

		KeyEvent getKeyInBuffer();
	private:
		bool doAutorepeat = false;
		static constexpr unsigned int numberOfKeys = 256u;
		std::bitset<numberOfKeys> keystates;
		std::queue<KeyEvent> keyBuff;
	};

	class EventHandler {
	public:
		LRESULT HandleMSG(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam);
		bool keyPressed(unsigned char keyCode) const;
	private:
		KeyHandler keyHandler;
	};
}