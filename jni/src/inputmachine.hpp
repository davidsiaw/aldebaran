#ifndef INPUTMACHINE_HPP
#define INPUTMACHINE_HPP

#include <map>
#include <SDL.h>
#include "inputstate.hpp"

void CaptureInputState(std::map<int, InputState::Key> keyMap, InputState* state, SDL_Event* e);

#endif //INPUTMACHINE_HPP
