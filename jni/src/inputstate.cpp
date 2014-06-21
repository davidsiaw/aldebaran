#include "inputstate.hpp"
#include <cstring>

InputState::InputState()
:	
	fingered(false),
	fingerx(0),
	fingery(0),
    accelx(0),
    accely(0),
    accelz(0)
{
	memset(down, 0, 32);
	memset(pressed, 0, 32);
	memset(released, 0, 32);
}

static bool GetBit(const char bitIndex, const char* array)
{
    return (array[bitIndex >> 3] & ( 1 << (bitIndex & 0x7) )) != 0;
}

static void SetBit(char bitIndex, char* array, bool state)
{
    char* block = &array[bitIndex >> 3];
    char clear = ~( 1 << (bitIndex & 0x7) );
    char set = ( state << (bitIndex & 0x7) );
    *block &= clear;
    *block |= set;
}

bool InputState::GetButtonState(Key key) const
{
    return GetBit(key, down);
}

bool InputState::GetButtonPressed(Key key) const
{
    return GetBit(key, pressed);
}

bool InputState::GetButtonReleased(Key key) const
{
    return GetBit(key, released);
}

void InputState::SetButtonState(Key key, bool state)
{
    SetBit(key, down, state);
}

void InputState::SetButtonPressed(Key key, bool state)
{
    SetBit(key, pressed, state);
}

void InputState::SetButtonReleased(Key key, bool state)
{
    SetBit(key, released, state);
}

bool InputState::GetFingered() const
{
	return fingered;
}

float InputState::GetFingerX() const
{
	return fingerx;
}

float InputState::GetFingerY() const
{
	return fingery;
}

void InputState::SetFingered(bool state)
{
	fingered = state;
}

void InputState::SetFingerX(float fx)
{
	fingerx = fx;
}

void InputState::SetFingerY(float fy)
{
	fingery = fy;
}

float InputState::GetAccelX() const
{
    return accelx;
}

float InputState::GetAccelY() const
{
    return accely;
}

float InputState::GetAccelZ() const
{
    return accelz;
}

void InputState::SetAccelX(float amount)
{
    accelx = amount;
}

void InputState::SetAccelY(float amount)
{
    accely = amount;
}

void InputState::SetAccelZ(float amount)
{
    accelz = amount;
}
