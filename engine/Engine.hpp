//
// Created by Arkadiusz on 27/07/2025.
//

#ifndef ENGINE_H
#define ENGINE_H


class Engine {
public:
    void run();

    void registerActor();

    void unregisterActor();

    void openScene();

    void setTimeout();

    void clearTimeout();

    void setInterval();

    void clearInterval();
};


#endif //ENGINE_H
