#pragma once

#include "KamataEngine.h"

struct CollisionMapInfo {
	Vector3 move;
	bool landing = false;
	bool wallContact = false;
	bool ceilingCollisionFlag = false;
};

enum Corner { kRightBottom, kLeftBottom, kRightTop, kLeftTop, knumCorner };