#ifndef _UTIL_H_
#define _UTIL_H_

#include "Geometry.h"

const float LAYER_BG = 0.99f;
const float LAYER_PLAYER_FIXED_WEAPON = 0.95f;
const float LAYER_PLAYER = 0.9f;
const float LAYER_PLAYER_BASIC_WEAPON = 0.89f;
const float LAYER_PLAYER_TAIL = 0.85f;

const float LAYER_MENU = 0.50f;

const float LAYER_HUD = 0.25f;
const float LAYER_FG = 0.01f;

float VolumeFromDistance(Point p1, Point p2, float maxDistance = 2560.0f);

#endif
