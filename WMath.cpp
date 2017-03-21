#include <cmath>
#include "WMath.h"

using namespace wsm;
vec2::vec2(float x, float y) {
    this->x = x;
    this->y = y;

}
bool vec2::operator == (vec2 const &vec) {
    return this->x == vec.x && this->y == vec.y;
}

bool vec2::operator != (vec2 const  &vec) {
    return !(this->x == vec.x && this->y == vec.y);
}

vec2 vec2::operator + (vec2 const &vec) {
    return vec2(this->x + vec.x, this->y + vec.y);
}

vec2 vec2::operator * (double const &number) {
    return vec2(this->x * number, this->y * number);
}

vec2::vec2() {
    x = 0;
    y = 0;
}

float wsm::distance(vec2 u, vec2 v) {
    float x = u.x - v.x, y = u.y - v.y;
    return std::sqrt(x * x + y * y);
}

bool wsm::nearest_arc(float from, float to) {
    if (std::abs(from - to) > PI) {
        if (to < from) {
            return true;
        } else {
            return false;
        }
    } else {
        if (to > from) {
            return true;
        } else {
            return false;
        }
    }
}

float wsm::angle_bv(vec2 u, vec2 v) {
    float lenght_u = distance(u), length_v = distance(v);
    float cos_a = (u.x * v.x + u.y * v.y) / (lenght_u * length_v);
    return std::acos(cos_a);
}

float wsm::vector_angle(vec2 vec) {
    float angle = std::atan(-vec.y / vec.x) + PI / 2;
    if (vec.x < 0) {
        angle += PI;
    }
    return angle;
}

bool wsm::get_repulse_vector(vec2 &res_vector, vec2 normal, vec2 vector) {
    float angle = angle_bv(normal, vector);
    if (angle > PI / 2) {
        return false;
    }
    float thetta = ((PI / 2) - angle) * 2;
    float alpha = vector_angle(normal), betta = vector_angle(vector);
    if (nearest_arc(alpha, betta)) {
        thetta = -thetta;
    }
    float nx = vector.x * std::cos(thetta) - vector.y * std::sin(thetta);
    float ny = vector.x * std::sin(thetta) + vector.y * std::cos(thetta);
    res_vector = vec2(nx, ny);
}

vec2 wsm::get_move_vector(vec2 from, vec2 to) {
    float dx = to.x - from.x, dy = to.y - from.y;
    float length = distance(from, to);
    return vec2(dx / length, dy / length);
}

vec2 wsm::get_vector_from_angle(float angle) {
    angle = -(angle - PI / 2);
    if (angle > PI * 2) {
        angle -= PI * 2;
    }
    if (angle < 0) {
        angle += PI * 2;
    }
    return vec2(std::cos(angle), std::sin(angle));
}

vec2 wsm::get_near_vertical_vector(vec2 vec, vec2 addvec) {
    if (angle_bv(vec2(vec.y, -vec.x), addvec) < PI / 2) {
        return vec2(vec.y, -vec.x);
    } else {
        return vec2(-vec.y, vec.x);
    }
}

vec2 wsm::normalize_vector(vec2 vec) {
    float abs = distance(vec);
    return vec2(vec.x / abs, vec.y / abs);
}

