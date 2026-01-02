

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <memory>
#include <game.h>
game* game_ = nullptr;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    SDL_Init(
        SDL_INIT_AUDIO |
        SDL_INIT_EVENTS |
        SDL_INIT_VIDEO
    );

    TTF_Init();

    //Init singletons 
    window::get_instance();
    camera::get_instance();
    fps::get_instance();
    level_manager::get_instance();

    game_ = new game();

    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{

    return game_->input(event);
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate)
{

    return game_->cycle();
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
    delete game_;

    level_manager::unload_all();

    TTF_Quit();
}
