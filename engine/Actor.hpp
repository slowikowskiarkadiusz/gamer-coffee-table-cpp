//
// Created by Arkadiusz on 27/07/2025.
//
#pragma once

#include <string>
#include "Matrix.hpp"
#include "v2.hpp"

class Actor {
public:
    v2 getCenter();

    v2 getSize();

    int getRotation();

    std::string getName();

    void rename(std::string newName);

    void resize(v2 newSize);

    void rotate(int degrees);

    void rotateAround(v2 around, int degrees);

    void setRotation(int degrees);

    void moveTo(v2 to);

    void moveBy(v2 by);

    void kill();

    virtual void update(int deltaTime);

    virtual Matrix render();

    virtual void fixedUpdate(int fixedDeltaTime);

protected:
    void setCenter(v2 newValue);

    virtual void redraw();

private:
    void setSize(v2 newSize);
};


#endif //ACTOR_H
