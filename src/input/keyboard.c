#include "keyboard.h"

bool is_key_down(SDL_Keycode key) {
    SDL_Keymod mod = SDL_GetModState();
    SDL_Scancode scancode = SDL_GetScancodeFromKey(key, &mod);

    const bool *state = SDL_GetKeyboardState(NULL);
    return state[scancode];
}

bool is_key_up(SDL_Keycode key) {
    return !is_key_down(key);
}