/*
    Peter Elliott

    lunar lander
    precalc.h
*/
#ifndef PRECALC_H
#define PRECALC_H

const float COS_TABLE[9] = { // values of cos from 0-π with a step of π/8
    1.0,
    0.9238795325112867,
    0.7071067811865476,
    0.38268343236508984,
    0.0,
    -0.3826834323650897,
    -0.7071067811865475,
    -0.9238795325112867,
    -1.0
};

const float SIN_TABLE[9] = { // values of sin from 0-π with a step of π/8
    0.0,
    0.3826834323650898,
    0.7071067811865475,
    0.9238795325112867,
    1.0,
    0.9238795325112867,
    0.7071067811865476,
    0.3826834323650899,
    0.0,
};

#endif
