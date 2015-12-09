#pragma once
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

#define PI 3.14159265359

/*
 * Simple math, contains some vectors conversions
 */
namespace wsm {
    /**
     * Simple vector stuct
     */
    struct vec2 {
    public:
        float x, y;
        /**
         * Constuct vector with x, y fields
         * \param x X
         * \param y Y
         */
        vec2(float x, float y);
        /**
         * Constucs (0, 0) vector;
         */
        vec2();
        /**
         * \param vec Vector to compare
         * \return true if fields x and y of this vectors equal
         */
        bool operator == (vec2 const &vec);
        /**
         * \param vec Vector to compare
         * \return false if fields x and y of this vectors equal
         */
        bool operator != (vec2 const &vec);
        /**
         * \param vec Vector to add
         * \return Sum of vectors
         */
        vec2 operator + (vec2 const &vec);
        /**
         * \param number Number to multiply
         * \return multiply of vector and number
         */
        vec2 operator * (double const &number);
    };
    /**
     * \param u first point
     * \param v second point, default (0, 0)
     * \return distanse between two points
     */
    float distance(vec2 u, vec2 v = vec2(0, 0));
    /**
     * \param from Angle in radians
     * \param to Angle in radians
     * \return true if shortest path to move is clockwise
     */
    bool nearest_arc(float from, float to);
    /**
     * \param u first vector
     * \param v second vector
     * \return Angle between two vectors (in radians)
     */
    float angle_bv(vec2 u, vec2 v);
    /**
     * \param u vector
     * \return Vector direction angle (in radians)
     */
    float vector_angle(vec2 u);
    /**
     * \param from Point from
     * \param to Point to
     * \return normal Vector that we need use to relocate from first vector to second
     */
    vec2 get_move_vector(vec2 from, vec2 to);
    /**
     * \param res_vector Vector to write result
     * \param normal Normal to plane
     * \param vector Vector
     * \return true if result finded, write into res_vector repulse vector
     */
    bool get_repulse_vector(vec2 &res_vector, vec2 normal, vec2 vector);
    /**
     * \param angle Angle (in radians)
     * \return Normal vector that directs at angle
     */
    vec2 get_vector_from_angle(float angle);
    /**
     * \param vec Vector
     * \param addvec Additional vector to find vertical near this vector
     * \return vertical vector near additianal vector
     */
    vec2 get_near_vertical_vector(vec2 vec, vec2 addvec);
    /**
     * \param vec Vector
     * \return Normal vector
     */
    vec2 normalize_vector(vec2 vec);
}