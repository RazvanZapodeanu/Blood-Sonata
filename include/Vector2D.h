#ifndef VECTOR2D_H
#define VECTOR2D_H
#include <iostream>
#include <cmath>

class Vector2D {
private:
    double x;
    double y;
public:
    explicit Vector2D(double xVal=0.0, double yVal=0.0) : x(xVal), y(yVal) {}

    Vector2D(const Vector2D& other) : x(other.x), y(other.y) {}

    Vector2D& operator=(const Vector2D& other) {
        if (this !=&other) {
            x=other.x;
            y=other.y;
        }
        return *this;
    }
    ~Vector2D() {}

    [[nodiscard]] double getX() const { return x; }
    [[nodiscard]] double getY() const { return y; }
    void setX(double newX) { this->x=newX; }
    void setY(double newY) { this->y=newY; }

    [[nodiscard]] double magnitude() const {
        return std::sqrt(x*x + y*y);
    }
    [[nodiscard]] double dot(const Vector2D& other) const {
        return x * other.x + y * other.y;
    }
    void normalize(){
        double mag = magnitude();
        if (mag>0) {
            x/=mag;
            y/=mag;
        }
    }

    [[nodiscard]] Vector2D normalized() const {
        Vector2D result(*this);
        result.normalize();
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
        os << "(" << v.getX() << ", " << v.getY() << ")";
        return os;
    }
};


#endif
