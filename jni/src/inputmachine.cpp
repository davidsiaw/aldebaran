#include "inputmachine.hpp"

void CaptureInputState(std::map<int, InputState::Key> keyMap, InputState* inputState, SDL_Event* e)
{
	if (e->type == SDL_JOYBUTTONDOWN)
	{

	}
	else if (e->type == SDL_JOYBUTTONUP)
	{

	}
	else if (e->type == SDL_FINGERUP)
	{	
		inputState->SetFingered(false);
	}
	else if (e->type == SDL_FINGERDOWN)
	{
		inputState->SetFingered(true);
		inputState->SetFingerX(e->tfinger.x);
		inputState->SetFingerY(e->tfinger.y);
	}
	else if (e->type == SDL_KEYDOWN)
	{
		auto keyIter = keyMap.find(e->key.keysym.sym);
		if ( keyIter != keyMap.end() )
		{
			inputState->SetButtonPressed(keyIter->second, true);
			inputState->SetButtonState(keyIter->second, true);
		}
	}
	else if (e->type == SDL_KEYUP)
	{
		auto keyIter = keyMap.find(e->key.keysym.sym);
		if ( keyIter != keyMap.end() )
		{
			inputState->SetButtonReleased(keyIter->second, true);
			inputState->SetButtonState(keyIter->second, false);
		}
	}
}