#include "inputmachine.hpp"

void CaptureInputState(InputState* inputState, SDL_Event* e)
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
		if (e->key.keysym.sym == SDLK_UP)
		{
			inputState->SetUpButtonState(true);
		}
		else if (e->key.keysym.sym == SDLK_DOWN)
		{
			inputState->SetDownButtonState(true);
		}
		else if (e->key.keysym.sym == SDLK_LEFT)
		{
			inputState->SetLeftButtonState(true);
		}
		else if (e->key.keysym.sym == SDLK_RIGHT)
		{
			inputState->SetRightButtonState(true);
		}
		else if (e->key.keysym.sym == SDLK_a)
		{
			inputState->SetAButtonState(true);
		}
		else if (e->key.keysym.sym == SDLK_s)
		{
			inputState->SetBButtonState(true);
		}
		else if (e->key.keysym.sym == SDLK_z)
		{
			inputState->SetXButtonState(true);
		}
		else if (e->key.keysym.sym == SDLK_x)
		{
			inputState->SetYButtonState(true);
		}
	}
	else if (e->type == SDL_KEYUP)
	{
		if (e->key.keysym.sym == SDLK_UP)
		{
			inputState->SetUpButtonState(false);
		}
		else if (e->key.keysym.sym == SDLK_DOWN)
		{
			inputState->SetDownButtonState(false);
		}
		else if (e->key.keysym.sym == SDLK_LEFT)
		{
			inputState->SetLeftButtonState(false);
		}
		else if (e->key.keysym.sym == SDLK_RIGHT)
		{
			inputState->SetRightButtonState(false);
		}
		else if (e->key.keysym.sym == SDLK_a)
		{
			inputState->SetAButtonState(false);
		}
		else if (e->key.keysym.sym == SDLK_s)
		{
			inputState->SetBButtonState(false);
		}
		else if (e->key.keysym.sym == SDLK_z)
		{
			inputState->SetXButtonState(false);
		}
		else if (e->key.keysym.sym == SDLK_x)
		{
			inputState->SetYButtonState(false);
		}
	}
}