#include "inputmachine.hpp"
#include "printlog.hpp"

void CaptureInputState(std::shared_ptr<GameContext> context, InputState* inputState, SDL_Event* e)
{
	std::map<int, InputState::Key> keyMap = context->GetKeyMap();
    if (e->type == SDL_FINGERUP)
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
	else if (e->type == SDL_JOYBUTTONDOWN)
	{
		if (e->jbutton.button == 0)
		{
			inputState->SetButtonPressed(InputState::A, true);
			inputState->SetButtonState(InputState::A, true);
		}
		//printlog("Joystick %d button %d pressed\n", e->jbutton.which, e->jbutton.button);
	}
	else if (e->type == SDL_JOYBUTTONUP)
	{
		if (e->jbutton.button == 0)
		{
			inputState->SetButtonReleased(InputState::A, true);
			inputState->SetButtonState(InputState::A, false);
		}
		//printlog("Joystick %d button %d released\n", e->jbutton.which, e->jbutton.button);
	}
	else if (e->type == SDL_JOYAXISMOTION)
	{
		if (e->jaxis.axis == 0)
		{
			inputState->SetJoyX(e->jaxis.value);
		}
		if (e->jaxis.axis == 1)
		{
			inputState->SetJoyY(e->jaxis.value);
		}
		//printlog("Joystick %d axis %d value = %d\n", e->jaxis.which, e->jaxis.axis, e->jaxis.value);
	}
    
#if __APPLE__
    
#if TARGET_IPHONE_SIMULATOR
    // iOS Simulator
    
    static int warnNoAccelerometer = 0;
    if (!warnNoAccelerometer)
    {
        printlog("Warning: iOS Simulator has no accelerometer\n");
        warnNoAccelerometer = 1;
    }
    
#elif TARGET_OS_IPHONE
    // iOS device
    
    if (e->type == SDL_JOYAXISMOTION)
    {
        switch(e->jaxis.axis)
        {
            case 0:
                inputState->SetAccelX(e->jaxis.value);
                break;
            case 1:
                inputState->SetAccelY(e->jaxis.value);
                break;
            case 2:
                inputState->SetAccelZ(e->jaxis.value);
                break;
        }
    }
    
#endif
    
#endif // __APPLE__
}