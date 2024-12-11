#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <SDL3/SDL.h>

bool is_key_down(SDL_Keycode key);
bool is_key_up(SDL_Keycode key);

#endif // KEYBOARD_H