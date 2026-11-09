#include <SDL/SDL.h>

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Surface* screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
    SDL_WM_SetCaption("Title", NULL);

    int run = 1;
    int frame = 0;
    SDL_Rect rect = {200, 200, 20, 20};
    while (run) {
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 255 - frame%256, frame%256, frame%256));
        SDL_Flip(screen);
        SDL_Delay(10);

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    run=0;
                    break;
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            run=0;
                            break;
                        case SDLK_UP:
                            rect.y -= 10;
                            break;
                        case SDLK_DOWN:
                            rect.y += 10;
                            break;
                        case SDLK_LEFT:
                            rect.x -= 10;
                            break;
                        case SDLK_RIGHT:
                            rect.x += 10;
                            break;
                        default:
                            break;
                    }
                    break;
            }
        }

        frame++;
    }

    SDL_Quit();

}
