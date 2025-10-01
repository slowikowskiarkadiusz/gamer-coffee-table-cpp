#pragma once

#include <Arduino.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

#include "src/engine.hpp"
#include "src/input/table_input_provider.hpp"

#define PANEL_RES_X 64   // rozdzielczość jednego panelu
#define PANEL_RES_Y 64
#define PANEL_CHAIN 2    // liczba paneli w łańcuchu

MatrixPanel_I2S_DMA *dma_display = nullptr;

void setup() {
    HUB75_I2S_CFG mxconfig(
        PANEL_RES_X,
        PANEL_RES_Y,
        PANEL_CHAIN
    );

    // twoje mapowanie pinów
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
    dma_display->setBrightness8(50);

    std::shared_ptr<table_input_provider> input_provider = std::make_shared<table_input_provider>();

    engine engineObj(input_provider);
    engineObj.run();

    engineObj.set_on_frame_finished([&circles, &window](std::vector<std::vector<color> > frame) {
        dma_display->fillScreen(dma_display->color565(0, 0, 0));

        for (int y = 0; y < engine::screen_size.y; y++) {
            for (int x = 0; x < engine::screen_size.x; x++) {
                auto matrixPixel = frame[x][y];
                // circles[x][y].setFillColor(sf::Color(matrixPixel.r * 255, matrixPixel.g * 255, matrixPixel.b * 255, matrixPixel.a * 255));
                // window.draw(circles[x][y]);

                dma_display->drawPixel(x, y, dma_display->color565(matrixPixel.r, matrixPixel.g, matrixPixel.b));
            }
        }
    });
}

void loop() {
}


int main() {
    std::shared_ptr<table_input_provider> input_provider = std::make_shared<table_input_provider>();

    engine engineObj(input_provider);
    engineObj.run();

    engineObj.set_on_frame_finished([&circles, &window](std::vector<std::vector<color> > frame) {
        dma_display->fillScreen(dma_display->color565(0, 0, 0));

        for (int y = 0; y < engine::screen_size.y; y++) {
            for (int x = 0; x < engine::screen_size.x; x++) {
                auto matrixPixel = frame[x][y];
                // circles[x][y].setFillColor(sf::Color(matrixPixel.r * 255, matrixPixel.g * 255, matrixPixel.b * 255, matrixPixel.a * 255));
                // window.draw(circles[x][y]);

                dma_display->drawPixel(x, y, dma_display->color565(matrixPixel.r, matrixPixel.g, matrixPixel.b));
            }
        }
    });

    // while (window.isOpen()) {
    // sf::Event event;
    // while (window.pollEvent(event)) {
    // input_provider->sfEventQueue.push(event);
    // if (event.type == sf::Event::Closed)
    // window.close();
    // }
    // }

    // engineObj.stop();
    // return 0;
}
