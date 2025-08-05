#include <SDL2/SDL.h>

#include "./src/engine.hpp"
#include "src/input/keyboard_input_provider.hpp"

int main() {
    std::shared_ptr<keyboard_input_provider> input_provider = std::make_shared<keyboard_input_provider>();

    engine engineObj(input_provider);
    engineObj.run();

    constexpr int scale = 10;
    const int width = engine::screen_size.x;
    const int height = engine::screen_size.y;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) return -1;

    SDL_Window* window = SDL_CreateWindow(
        "gamer-coffee-table-cpp",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width * scale,
        height * scale,
        0
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        width,
        height
    );

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            input_provider->sdlEventQueue.push(event);
            if (event.type == SDL_QUIT)
                running = false;
        }

        const auto& pixels = engineObj.screen.pixels();

        void* pixelsPtr;
        int pitch;
        SDL_LockTexture(texture, nullptr, &pixelsPtr, &pitch);

        uint32_t* dst = static_cast<uint32_t*>(pixelsPtr);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                auto p = pixels[x][y];
                uint8_t r = static_cast<uint8_t>(p.r * 255);
                uint8_t g = static_cast<uint8_t>(p.g * 255);
                uint8_t b = static_cast<uint8_t>(p.b * 255);
                uint8_t a = static_cast<uint8_t>(p.a * 255);
                dst[y * width + x] = (a << 24) | (b << 16) | (g << 8) | r;
            }
        }

        SDL_UnlockTexture(texture);

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderSetScale(renderer, scale, scale);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    engineObj.stop();
    return 0;
}
