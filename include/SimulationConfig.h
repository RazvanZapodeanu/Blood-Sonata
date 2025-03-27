#ifndef SIMULATIONCONFIG_H
#define SIMULATIONCONFIG_H

struct SimulationConfig {

    static inline float gravity = 0.0f;
    static inline float elasticity = 0.75f;
    static inline float deltaTime = 0.1f;
    static inline int targetFPS = 60;
    static inline float GroundFriction = 0.9;
    static inline float AirFriction = 0.99;
    static inline double stabilityThreshold = 0.1;

    static inline int windowWidth = 800;
    static inline int windowHeight = 600;

    static inline bool paused = false;

};

#endif
