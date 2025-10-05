#pragma once

#include "../../src/engine.hpp"
#include "./esp32_input_provider.hpp"

#define PANEL_RES_X 64
#define PANEL_RES_Y 32
#define PANEL_CHAIN 2

MatrixPanel_I2S_DMA *dma_display = nullptr;

std::shared_ptr<esp32_input_provider> input_provider = std::make_shared<esp32_input_provider>();
engine engineObj(input_provider);

void print_to_console(std::vector<std::vector<color> > frame) {
    std::string result = "\n";
    for (size_t y = 0; y < frame[0].size(); y++) {
        result += "|";
        for (size_t x = 0; x < frame.size(); x++) {
            result += frame[x][y].is_none() ? "  " : "O ";
        }
        result += "|\n";
    }
    std::cout << result;
}

void draw(int x, int y, color color_) {
    uint16_t c = dma_display->color565(color_.r * 255, color_.g * 255, color_.b * 255);

    if (y < 32) {
        dma_display->drawPixel(x, y, c);
    } else {
        dma_display->drawPixel(x + 64, y - 32, c);
    }
}

extern "C" void app_main(void) {
    HUB75_I2S_CFG mxconfig(
        PANEL_RES_X,
        PANEL_RES_Y,
        PANEL_CHAIN
    );

    mxconfig.gpio.r1 = 4;
    mxconfig.gpio.g1 = 5;
    mxconfig.gpio.b1 = 6;
    mxconfig.gpio.r2 = 7;
    mxconfig.gpio.g2 = 8;
    mxconfig.gpio.b2 = 9;
    mxconfig.gpio.a = 10;
    mxconfig.gpio.b = 11;
    mxconfig.gpio.c = 12;
    mxconfig.gpio.d = 13;
    mxconfig.gpio.clk = 14;
    mxconfig.gpio.lat = 15;
    mxconfig.gpio.oe = 16;

    dma_display = new MatrixPanel_I2S_DMA(mxconfig);
    dma_display->begin();
    dma_display->setBrightness8(80);

    engineObj.run();

    engine::set_on_frame_finished([](std::vector<std::vector<color> > frame) {
        for (int y = 0; y < engine::screen_size.y; y++) {
            for (int x = 0; x < engine::screen_size.x; x++) {
                draw(x, y, frame[x][y]);
            }
        }
    });

    while (true) {
        if (input::is_any_key_down()) {
            ESP_LOGI("APP", "any key DOWN!");
        }
        if (input::is_key_down(key::START))
            ESP_LOGI("APP", "START DOWN!");

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}
