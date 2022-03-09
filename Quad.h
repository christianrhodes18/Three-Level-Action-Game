//
// Created by thech on 5/14/2021.
//

#ifndef FINAL_PROJECT_CRHODES2_KPOLITZA_QUAD_H
#define FINAL_PROJECT_CRHODES2_KPOLITZA_QUAD_H

struct color_quad {
    double red;
    double green;
    double blue;
};

struct point {
    int x;
    int y;
};

class Quad {
protected:
    color_quad fill;
    point center;
    unsigned int width;
    unsigned int height;

public:
    Quad();
    Quad(color_quad fill, point center, unsigned int width, unsigned int height);

    int getCenterX() const;
    int getLeftX() const;
    int getRightX() const;
    int getCenterY() const;
    int getTopY() const;
    int getBottomY() const;
    point getCenter() const;

    double getRed() const;
    double getGreen() const;
    double getBlue() const;
    color_quad getFill() const;

    unsigned int getWidth() const;
    unsigned int getHeight() const;

    void setColor(double red, double green, double blue);
    void setColor(color_quad fill);
    void move(int deltaX, int deltaY);
    void resize(unsigned int width, unsigned int height);

    virtual void draw() const;

    bool isOverlapping(int x, int y) const;
};

#endif //FINAL_PROJECT_CRHODES2_KPOLITZA_QUAD_H
