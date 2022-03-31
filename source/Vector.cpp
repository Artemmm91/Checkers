#include "Vector.hpp"


Vector2D::Vector2D(): x(0), y(0) {}
Vector2D::Vector2D(int x, int y): x(x), y(y) {}

Vector2D& Vector2D::operator+=(const Vector2D& v) {
    x += v.x;
    y += v.y;
    return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& v) {
    *this += (-v);
    return *this;
}

Vector2D& Vector2D::operator*=(int scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector2D Vector2D::operator-() const {
    return {-x, -y};
}

bool operator==(const Vector2D& v1, const Vector2D& v2) {
    return v1.x == v2.x && v1.y == v2.y;
}

bool operator!=(const Vector2D& v1, const Vector2D& v2) {
    return !(v1 == v2);
}

Vector2D operator+(const Vector2D& v1, const Vector2D& v2) {
    Vector2D v(v1);
    v += v2;
    return v;
}

Vector2D operator-(const Vector2D& v1, const Vector2D& v2) {
    return v1 + (-v2);
}

int operator*(const Vector2D& v1, const Vector2D& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

Vector2D operator*(const Vector2D& v, int scalar) {
    Vector2D v_new = v;
    v_new *= scalar;
    return v_new;
}

Vector2D operator*(int scalar, const Vector2D& v) {
    return v * scalar;
}


