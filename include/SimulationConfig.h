#ifndef SIMULATIONCONFIG_H
#define SIMULATIONCONFIG_H

struct SimulationConfig {

    static inline float gravity = 0.0f;
    static inline float elasticity = 0.75f;
    static inline float deltaTime = 0.1f;
    static inline int targetFPS = 60;
    static inline float GroundFriction = 0.9f;
    static inline float AirFriction = 0.99f;
    static inline float particleGravity = 100.0f;
    static inline float GravityRadius = 20.0f;
    static inline unsigned int windowWidth = 1920;
    static inline unsigned int windowHeight = 760;
    static inline float particleSurfaceFriction = 0.92f;

    static inline bool paused = false;

};

#endif
