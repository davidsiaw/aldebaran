#ifndef INPUTMACHINE_HPP
#define INPUTMACHINE_HPP

#include <map>
#include <SDL.h>
#include "inputstate.hpp"
#include "gamecontext.hpp"

void CaptureInputState(std::shared_ptr<GameContext> context, InputState* state, SDL_Event* e);

#endif //INPUTMACHINE_HPP
