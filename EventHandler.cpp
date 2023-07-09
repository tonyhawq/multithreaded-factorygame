#include "EventHandler.h"

handlers::MouseEvent::MouseEvent() {
	this->type = Type::INVALID;
	this->LMB = false;
	this->RMB = false;
	this->x = 0;
	this->y = 0;
	this->delta = 0;
}

handlers::MouseEvent::MouseEvent(Type type, MouseHandlerData parent, int x, int y, int delta) {
	this->type = type;
	this->LMB = parent.LMB;
	this->RMB = parent.RMB;
	this->x = x;
	this->y = y;
	this->delta = delta;
}

handlers::MouseEvent::Type handlers::MouseEvent::getType() const {
	return this->type;
}

bool handlers::MouseEvent::isPressDown() const {
	return (this->type == Type::LMB_DOWN || this->type == Type::RMB_DOWN);
}

bool handlers::MouseEvent::isPressUp() const {
	return (this->type == Type::LMB_UP || this->type == Type::RMB_UP);
}

bool handlers::MouseEvent::isMouseWheel() const {
	return (this->type == Type::WHEEL_UP || this->type == Type::WHEEL_DOWN);
}

bool handlers::MouseEvent::isLMBevent() const {
	return (this->type == Type::LMB_UP || this->type == Type::LMB_DOWN);
}

bool handlers::MouseEvent::isRMBevent() const {
	return (this->type == Type::RMB_UP || this->type == Type::RMB_DOWN);
}

bool handlers::MouseEvent::isLMB() const {
	return this->LMB;
}

bool handlers::MouseEvent::isRMB() const {
	return this->RMB;
}

int handlers::MouseEvent::getX() const {
	return this->x;
}

int handlers::MouseEvent::getY() const {
	return this->y;
}

bool handlers::MouseEvent::isMove() const {
	return this->type == Type::MOVE;
}

void handlers::MouseEvent::getPos(int* x, int* y) const {
	if (x) { (*x) = this->x; }
	if (y) { (*y) = this->y; }
}

bool handlers::MouseEvent::isValid() const {
	return this->type != Type::INVALID;
}

void handlers::MouseHandler::trimBuffer(unsigned char size) {
	while (this->mouseBuff.size() > size)
	{
		this->mouseBuff.pop();
	}
}

void handlers::MouseHandler::clearState() {
	this->RMB = false;
	this->LMB = false;
}

void handlers::MouseHandler::clearBuff() {
	while (!this->mouseBuff.empty())
	{
		this->mouseBuff.pop();
	}
}

void handlers::MouseHandler::onMousePress(bool LMB, bool RMB, int x, int y) {
	this->x = x;
	this->y = y;
	if (LMB)
	{
		this->LMB = true;
	}
	if (RMB)
	{
		this->RMB = true;
	}
	MouseEvent e;
	if (LMB)
	{
		e = MouseEvent(handlers::MouseEvent::Type::LMB_DOWN, { this->LMB, this->RMB }, x, y, 0);
	}
	else if (RMB)
	{
		e = MouseEvent(handlers::MouseEvent::Type::RMB_DOWN, { this->LMB, this->RMB }, x, y, 0);
	}
	this->mouseBuff.push(e);
	this->trimBuffer(32);
}

void handlers::MouseHandler::onMouseRelease(bool LMB, bool RMB, int x, int y) {
	this->x = x;
	this->y = y;
	if (LMB)
	{
		this->LMB = false;
	}
	if (RMB)
	{
		this->RMB = false;
	}
	MouseEvent e;
	if (LMB)
	{
		e = MouseEvent(handlers::MouseEvent::Type::LMB_UP, { this->LMB, this->RMB }, x, y, 0);
	}
	else if (RMB)
	{
		e = MouseEvent(handlers::MouseEvent::Type::RMB_UP, { this->LMB, this->RMB }, x, y, 0);
	}
	this->mouseBuff.push(e);
	this->trimBuffer(32);
}

void handlers::MouseHandler::onMouseMove(int x, int y) {
	this->x = x;
	this->y = y;
	MouseEvent e = MouseEvent(handlers::MouseEvent::Type::MOVE, { this->LMB, this->RMB }, x, y, 0);
	this->mouseBuff.push(e);
	this->trimBuffer(32);
}

void handlers::MouseHandler::onMouseEnter() {
	this->isInWindow = true;
	MouseEvent e = MouseEvent(handlers::MouseEvent::Type::ENTER, { this->LMB, this->RMB }, this->x, this->y, 0);
	this->mouseBuff.push(e);
	this->trimBuffer(32);
}

void handlers::MouseHandler::onMouseLeave() {
	this->isInWindow = false;
	MouseEvent e = MouseEvent(handlers::MouseEvent::Type::LEAVE, { this->LMB, this->RMB }, this->x, this->y, 0);
	this->mouseBuff.push(e);
	this->trimBuffer(32);
}

handlers::MouseEvent handlers::MouseHandler::read() {
	MouseEvent e = this->mouseBuff.front();
	this->mouseBuff.pop();
	return e;
}

bool handlers::MouseHandler::empty() {
	return this->mouseBuff.empty();
}

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
	return this->kbd.keyPressed(keyCode);
}

void handlers::KeyHandler::onChar(unsigned char keyCode) {
	KeyEvent e = KeyEvent(handlers::KeyEvent::Type::KEY_PRESS, keyCode);
	this->keyBuff.push(e);
	this->trimBuffer(32);
}

handlers::KeyEvent handlers::KeyHandler::read() {
	KeyEvent e;
	if (this->keyBuff.empty())
	{
		return e;
	}
	e = this->keyBuff.front();
	this->keyBuff.pop();
	return e;
}

bool handlers::KeyHandler::empty() {
	return this->keyBuff.empty();
}

void handlers::KeyHandler::trimBuffer(unsigned char size) {
	while (this->keyBuff.size() > size)
	{
		this->keyBuff.pop();
	}
}

void handlers::KeyHandler::clearBuff() {
	while (!this->keyBuff.empty())
	{
		this->keyBuff.pop();
	}
}

LRESULT handlers::EventHandler::HandleMSG(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam, int width, int height) {
	POINTS pt = { 0,0 };
	switch (msg)
	{
	/*  KEYBOARD EVENTS  */
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if ((lParam & 0x40000000) && (!this->kbd.doAutorepeat))
		{
			break;
		}
		this->kbd.onKeyPressed((unsigned char)wParam);
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		this->kbd.onKeyReleased((unsigned char)wParam);
		break;
	case WM_CHAR:
		this->kbd.onChar((unsigned char)wParam);
		break;
	case WM_KILLFOCUS:
		this->kbd.clearState();
		this->mouse.clearState();
		break;
	/*  MOUSE EVENTS  */
	case WM_MOUSEMOVE:
		pt = MAKEPOINTS(lParam);
		// if inside the window
		if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
		{
			// log movement
			this->mouse.onMouseMove(pt.x, pt.y);
			if (!this->mouse.isInWindow)
			{
				// and set it to be in window if not already
				SetCapture(windowHandle);
				this->mouse.onMouseEnter();
			}
		}
		else
		{
			// otherwise if a button is pressed
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				// log movement
				this->mouse.onMouseMove(pt.x, pt.y);
			}
			// but if not, log event for leaving
			else
			{
				ReleaseCapture();
				this->mouse.onMouseLeave();
			}
		}
		break;
	case WM_LBUTTONDOWN:
		pt = MAKEPOINTS(lParam);
		this->mouse.onMousePress(true, false, pt.x, pt.y);
		break;
	case WM_RBUTTONDOWN:
		pt = MAKEPOINTS(lParam);
		this->mouse.onMousePress(true, false, pt.x, pt.y);
		break;
	case WM_LBUTTONUP:
		pt = MAKEPOINTS(lParam);
		this->mouse.onMouseRelease(false, true, pt.x, pt.y);
		break;
	case WM_RBUTTONUP:
		pt = MAKEPOINTS(lParam);
		this->mouse.onMouseRelease(false, true, pt.x, pt.y);
		break;
	}
	return DefWindowProc(windowHandle, msg, wParam, lParam);
}