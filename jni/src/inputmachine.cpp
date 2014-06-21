#include "inputmachine.hpp"
#include "printlog.hpp"

void CaptureInputState(std::map<int, InputState::Key> keyMap, InputState* inputState, SDL_Event* e)
{
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