//
// Created by Arkadiusz on 27/07/2025.
//

#ifndef V2_H
#define V2_H


class v2 {
public:
    v2(float x, float y);

    float x;
    float y;

    static v2 copy(v2 from);
    static v2 getZero();
    static v2 getOne();
    static v2 getNOne();
    static v2 getUp();
    static v2 getDown();
    static v2 getLeft();
    static v2 getRight();
    static v2 getLeftDown();
    static v2 getLeftUp();
    static v2 getRightDown();
    static v2 getRightUp();
    static float dot(v2 v1, v2 v2);
    static float distance(v2 v1, v2 v2);

    v2 copy() const;
    v2 add(v2 v) const;
    v2 sub(v2 v) const;
    v2 mul(float scalar) const;
    v2 div(float scalar) const;
    float mag() const;
    v2 setX(float newX) const;
    v2 setY(float newY) const;
    v2 floor() const;
    v2 round() const;
    v2 ceil() const;
    v2 norm() const;
    bool eq(v2 other) const;
    v2 updt(v2 to);
    v2 rotateAround(v2 pivot, float degrees) const;
};


#endif //V2_H
