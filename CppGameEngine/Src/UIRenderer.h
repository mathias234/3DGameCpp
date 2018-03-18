//
// Created by matha on 16/03/2018.
//

#pragma once

class Rect {
public:
    float x, y, width, height;
    Rect(float x, float y, float width, float height) {
        x = x;
        y = y;
        width = width;
        height = height;
    }

};

class UIRenderer {
private:
    void Setup2D();
public:
    void DrawRect(Rect rect);
};


