#ifndef CHECKERS_VECTOR_HPP
#define CHECKERS_VECTOR_HPP

struct Vector2D {
public:
    int x;
    int y;

    Vector2D();
    Vector2D(int x, int y);

    Vector2D& operator+=(const Vector2D& v);
    Vector2D& operator-=(const Vector2D& v);
    Vector2D& operator*=(int scalar);
    Vector2D operator-() const;
};

bool operator==(const Vector2D& v1, const Vector2D& v2);
bool operator!=(const Vector2D& v1, const Vector2D& v2);
int operator*(const Vector2D& v1, const Vector2D& v2);
Vector2D operator*(const Vector2D& v, int scalar);
Vector2D operator*(int scalar, const Vector2D& v);
Vector2D operator+(const Vector2D& v1, const Vector2D& v2);
Vector2D operator-(const Vector2D& v1, const Vector2D& v2);

#endif
