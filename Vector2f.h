#ifndef Vector2f_h
#define Vector2f_h
#include <math.h>

class Vector2f
{
    public:

        float x = 0;
        float y = 0;

        Vector2f (float x, float y)
        {
            this->x = x;
            this->y = y;  
        }

        Vector2f operator+ (Vector2f v)
        {
            return Vector2f(x + v.x, y + v.y);
        }

        Vector2f operator- (Vector2f v)
        {
            return Vector2f(x - v.x, y - v.y);
        }

        Vector2f operator* (float n)
        {
            return Vector2f(x * n, y * n);
        }

        Vector2f operator/ (float n)
        {
            return Vector2f(x / n, y / n);
        }

        float operator* (Vector2f v)
        {
            return x * v.x + y * v.y;
        }

        float length()
        {
            return sqrt(pow(x, 2) + pow(y, 2));
        }

        Vector2f normalize()
        {
            return Vector2f(x / length(), y / length());
        }
};

#endif