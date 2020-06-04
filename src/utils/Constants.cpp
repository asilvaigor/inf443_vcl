//
// Created by igor on 06/05/2020.
//

#include <cmath>
#include "Constants.h"

// Camera
const float Constants::FPS = 60.0f;
const float Constants::CAMERA_ANGLE = 40 * M_PI / 180;
const float Constants::CAMERA_Z_NEAR = 0.01f;
const float Constants::CAMERA_Z_FAR = 400.0f;

// Billboard
const float Constants::BILLBOARD_RATIO_THRESHOLD = 0.2;
const float Constants::BILLBOARD_TEXTURE_SIZE_RATIO = 100;

// Bear
const float Constants::BEAR_CHARGE_FACTOR = 0.3f;

// Bear filming companion
const float Constants::FLOATING_MARGIN = 7.5f;
const float Constants::FLOATING_RADIUS = 10.0f;
const float Constants::TIME_FACTOR = 0.2f;

// Bird
const float Constants::BIRD_TURN_FACTOR = 0.05f;
const float Constants::BIRD_MAX_TURN_FACTOR = 100.0f;
const float Constants::BIRD_ANIMATION_MAX_TIME = 1;
const float Constants::BIRD_ANIMATION_SPEED = 0.03f;

// Boid
const float Constants::BIRD_MIN_SCALE = 1.0f;
const float Constants::BIRD_MAX_SCALE = 2.0f;
const float Constants::CENTERING_FACTOR = 0.00005f;
const float Constants::VISUAL_RANGE = 10.0f;
const float Constants::MIN_DISTANCE = 2.0f;
const float Constants::AVOID_FACTOR = 0.01f;
const float Constants::MATCHING_FACTOR = 0.1f;
const float Constants::TURN_FACTOR = 0.001f;
const float Constants::BIRD_MAX_SPEED_LIMIT = 1.0f;
const float Constants::BIRD_MIN_SPEED_LIMIT = 0.5f;
const float Constants::TERRAIN_MARGIN = 20.0f;

// Companion follower
const float Constants::AXIS_MARKER_MESH_SCALE = 2.0f;

// Dipole companion
const float Constants::DIPOLE_FRICTION_FACTOR = 0.01f;
const float Constants::DIPOLE_DISTANCE = 5.0f;
const float Constants::DIPOLE_OFFSET = 8.0f;
const float Constants::DIPOLE_CHARGE = 15.0f;

