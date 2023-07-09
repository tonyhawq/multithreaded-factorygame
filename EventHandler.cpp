#include "EventHandler.h"

handlers::KeyEvent::KeyEvent()
{
	this->type = Type::KEY_INVALID;
	this->keyCode = 0;
}

handlers::KeyEvent::KeyEvent(Type type, unsigned char keyCode)
{
	this->type = type;
	this->keyCode = keyCode;
}

bool handlers::KeyEvent::isPress() const { return type == Type::KEY_PRESS; }
bool handlers::KeyEvent::isRelease() const { return type == Type::KEY_RELEASE; }
bool handlers::KeyEvent::isValid() const { return type != Type::KEY_INVALID; }
unsigned char handlers::KeyEvent::getCode() { return keyCode; }

void handlers::KeyHandler::onKeyPressed(unsigned char keyCode) {
	keystates[keyCode] = true;
}

void handlers::KeyHandler::onKeyReleased(unsigned char keyCode) {
	keystates[keyCode] = false;
}

bool handlers::KeyHandler::keyPressed(unsigned char keyCode) const {
	return keystates[keyCode];
}

void handlers::KeyHandler::clearState() {
	keystates.reset();
}

bool handlers::EventHandler::keyPressed(unsigned char keyCode) const {
	return this->keyHandler.keyPressed(keyCode);
}

void handlers::KeyHandler::onChar(unsigned char keyCode) {
	KeyEvent e = KeyEvent(handlers::KeyEvent::Type::KEY_PRESS, keyCode);
	this->keyBuff.push(e);
	this->trimBuffer(32);
}

handlers::KeyEvent handlers::KeyHandler::getKeyInBuffer() {
	KeyEvent e;
	if (this->keyBuff.empty())
	{
		return e;
	}
	e = this->keyBuff.front();
	this->keyBuff.pop();
	return e;
}

void handlers::KeyHandler::trimBuffer(unsigned char size) {
	while (this->keyBuff.size() > size)
	{
		this->keyBuff.pop();
	}
}

LRESULT handlers::EventHandler::HandleMSG(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if ((lParam & 0x40000000) && (!this->keyHandler.doAutorepeat))
		{
			break;
		}
		this->keyHandler.onKeyPressed((unsigned char)wParam);
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		this->keyHandler.onKeyReleased((unsigned char)wParam);
		break;
	case WM_CHAR:
		this->keyHandler.onChar((unsigned char)wParam);
		break;
	case WM_KILLFOCUS:
		this->keyHandler.clearState();
		break;
	}
	return DefWindowProc(windowHandle, msg, wParam, lParam);
}