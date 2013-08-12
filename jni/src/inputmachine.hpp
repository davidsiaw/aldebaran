#ifndef INPUTMACHINE_HPP
#define INPUTMACHINE_HPP

#include <SDL.h>
#include "inputstate.hpp"

void CaptureInputState(InputState* state, SDL_Event* e);

#endif //INPUTMACHINE_HPP
