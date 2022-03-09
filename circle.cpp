#include "circle.h"
#include "graphics.h"

Circle::Circle() : Shape(), radius(0) {
}

Circle::Circle(double radius) : Shape() {
    setRadius(radius);
}

Circle::Circle(color fill) : Shape(fill), radius(0) {
}

Circle::Circle(point2D center) : Shape(center), radius(0) {
}

Circle::Circle(color fill, point2D center) : Shape(fill, center), radius(0) {
}

Circle::Circle(double red, double green, double blue, double alpha) : Shape(red, green, blue, alpha), radius(0) {
}

Circle::Circle(double x, double y) : Shape(x, y), radius(0) {
}

Circle::Circle(double red, double green, double blue, double alpha, double x, double y) : Shape(red, green, blue, alpha, x, y), radius(0) {
}

Circle::Circle(color fill, double x, double y) : Shape(fill, x, y), radius(0) {
}

Circle::Circle(double red, double green, double blue, double alpha, point2D center) : Shape(red, green, blue, alpha, center), radius(0) {
}

Circle::Circle(color fill, double radius) : Shape(fill) {
    setRadius(radius);
}

Circle::Circle(point2D center, double radius) : Shape(center) {
    setRadius(radius);
}

Circle::Circle(color fill, point2D center, double radius) : Shape(fill, center) {
    setRadius(radius);
}

Circle::Circle(double red, double green, double blue, double alpha, double radius) : Shape(red, green, blue, alpha) {
    setRadius(radius);
}

Circle::Circle(double x, double y, double radius) : Shape(x, y) {
    setRadius(radius);
}

Circle::Circle(double red, double green, double blue, double alpha, double x, double y, double radius) : Shape(red, green, blue, alpha, x, y) {
    setRadius(radius);
}

Circle::Circle(color fill, double x, double y, double radius) : Shape(fill, x, y) {
    setRadius(radius);
}

Circle::Circle(double red, double green, double blue, double alpha, point2D center, double radius) : Shape(red, green, blue, alpha, center) {
    setRadius(radius);
}

Circle::Circle(double redFill, double greenFill, double blueFill, double alphaFill, double redBorder, double greenBorder, double blueBorder, double alphaBorder, double x, double y, double radius) : Shape(redFill, greenFill, blueFill, alphaFill, redBorder, greenBorder, blueBorder, alphaBorder, x, y) {
    setRadius(radius);
}

double Circle::getRadius() const {
    return radius;
}

double Circle::getLeftX() const {
    return center.x - radius;
}

double Circle::getRightX() const  {
    return center.x + radius;
}

double Circle::getTopY() const {
    return center.y - radius;
}

double Circle::getBottomY() const {
    return center.y + radius;
}

void Circle::setRadius(double r) {
    if (r < 0) {
        r = 0;
    }
    radius = r;
}

void Circle::changeRadius(double delta) {
    setRadius(radius + delta);
}

bool Circle::isOverlapping(const Circle &c) const {
    // return true if the distance between the center points is <= the sum of the radii
    return (sqrt((c.center.x - center.x)*(c.center.x - center.x) + (c.center.y - center.y)*(c.center.y - center.y)) <= c.radius + radius);
}

void Circle::collide(Circle &c) {
    // 2-dimensional elastic collision
    double radiusSum = radius + c.radius;
    double xVel = xVelocity, yVel = yVelocity, xVel2 = c.xVelocity, yVel2 = c.yVelocity;
    xVelocity = (xVel - ((2.0*c.radius)/radiusSum) * ((xVel - xVel2)*(c.center.x - center.x) + (yVel - yVel2)*(c.center.y - center.y)) / double((c.center.x - center.x)*(c.center.x - center.x) + (c.center.y - center.y)*(c.center.y - center.y)) * (c.center.x - center.x));
    yVelocity = (yVel - ((2.0*c.radius)/radiusSum) * ((xVel - xVel2)*(c.center.x - center.x) + (yVel - yVel2)*(c.center.y - center.y)) / double((c.center.x - center.x)*(c.center.x - center.x) + (c.center.y - center.y)*(c.center.y - center.y)) * (c.center.y - center.y));
    c.xVelocity = (xVel2 - ((2.0*radius)/radiusSum) * ((xVel2 - xVel)*(center.x - c.center.x) + (yVel2 - yVel)*(center.y - c.center.y)) / double((c.center.x - center.x)*(c.center.x - center.x) + (c.center.y - center.y)*(c.center.y - center.y)) * (center.x - c.center.x));
    c.yVelocity = (yVel2 - ((2.0*radius)/radiusSum) * ((xVel2 - xVel)*(center.x - c.center.x) + (yVel2 - yVel)*(center.y - c.center.y)) / double((c.center.x - center.x)*(c.center.x - center.x) + (c.center.y - center.y)*(c.center.y - center.y)) * (center.y - c.center.y));

    while (isOverlapping(c)) {
        move(xVelocity/100.0, yVelocity/100.0);
        c.move(c.xVelocity/100.0, c.yVelocity/100.0);
    }
}

void Circle::draw() const {
    // Set drawing color to fill color
    glColor3f(fill.red, fill.green, fill.blue);
    // Draw circle as Triangle Fan
    glBegin(GL_TRIANGLE_FAN);
    // Draw center point
    glVertex2i(center.x, center.y);
    // Set drawing color to border color
    glColor3f(border.red, border.green, border.blue);
    // Draw points on edge of circle
    for (double i = 0; i < 2.0*PI+0.05; i += (2.0*PI)/360.0) {
        glVertex2i(center.x + (radius * cos(i)),
                   center.y + (radius * sin(i)));
    }
    // End Triangle Fan
    glEnd();
}
