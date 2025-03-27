# 🌌 Aether

**Aether** is a particle simulation project built with C++ and SFML, focusing on clarity, modularity, and clean object-oriented design.

The simulation currently supports particle creation, visual rendering, and basic interaction. The goal is to offer a clear, educational system that can be gradually extended with more advanced mechanics.

---

## 📦 Features

- Particle rendering with color and position control
- Real-time particle generation via keyboard input
- Basic collision detection and response
- Configurable simulation parameters (e.g., delta time, FPS limit)
- Sound effects and custom background support

---

## 🔧 Technologies

- **C++17**
- **SFML 3+**
- CMake build system
- Object-Oriented Design Principles

---

## 🚧 Work in Progress

Aether is currently in an early functional state. Planned future additions include:

- Gravitational force between particles
- Two distinct simulation modes:
  - `space`: particles attract each other
  - `terra`: particles bounce and collide with ground
- A real-time control panel for tuning physics (gravity, elasticity, simulation speed)
- Configuration file system (e.g., `config.json`)

---

## 🎮 Controls

| Key        | Action                        |
|------------|-------------------------------|
| `Space`    | Spawn a new particle          |
| `P`        | Pause/resume simulation       |
| `R`        | Reset simulation              |
| `ESC`      | Exit application              |

---

## 📌 Notes

This project is a university assignment developed with clarity and extensibility in mind. While the core logic is simple, the architecture is designed to support future growth.

---

