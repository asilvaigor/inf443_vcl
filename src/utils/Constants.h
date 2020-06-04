//
// Created by igor on 06/05/2020.
//

#ifndef PGM_CONSTANTS_H
#define PGM_CONSTANTS_H


struct Constants {
    // Camera
    static const float FPS;
    static const float CAMERA_ANGLE;
    static const float CAMERA_Z_NEAR;
    static const float CAMERA_Z_FAR;

    // Billboard
    static const float BILLBOARD_RATIO_THRESHOLD;
    static const float BILLBOARD_TEXTURE_SIZE_RATIO;

    // Bear
    static const float BEAR_CHARGE_FACTOR;

    // Bear filming companion
    static const float FLOATING_MARGIN;
    static const float FLOATING_RADIUS;
    static const float TIME_FACTOR;

    // Bird
    static const float BIRD_TURN_FACTOR;
    static const float BIRD_MAX_TURN_FACTOR;
    static const float BIRD_ANIMATION_MAX_TIME;
    static const float BIRD_ANIMATION_SPEED;

    // Boid
    static const float BIRD_MIN_SCALE;
    static const float BIRD_MAX_SCALE;
    static const float CENTERING_FACTOR ;
    static const float VISUAL_RANGE;
    static const float MIN_DISTANCE;
    static const float AVOID_FACTOR;
    static const float MATCHING_FACTOR;
    static const float TURN_FACTOR;
    static const float BIRD_MAX_SPEED_LIMIT;
    static const float BIRD_MIN_SPEED_LIMIT;
    static const float TERRAIN_MARGIN;

    // Companion follower
    static const float AXIS_MARKER_MESH_SCALE;

    // Dipole companion
    static const float DIPOLE_FRICTION_FACTOR;
    static const float DIPOLE_DISTANCE;
    static const float DIPOLE_OFFSET;
    static const float DIPOLE_CHARGE;
};


#endif //PGM_CONSTANTS_H
