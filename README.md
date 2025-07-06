<div align="center">
 <img src="assets/icon.jpg" alt="Blood Sonata Icon" width="164" height="164">
</div>

# Blood Sonata

**Blood Sonata** is a 2D fighting game built with C++ and SFML as a university project.  
Features combat mechanics, AI opponents, and organized game architecture.

---

## Features

- **Combat System** — Multiple attack types with frame-based animations
- **AI Opponents** — Enemies with movement, attack patterns, and defensive behavior
- **Audio System** — Sound effects, background music, and audio management
- **Visual Feedback** — Simple effects for impacts and combat interactions
- **Game Architecture** — State management and resource handling

---

## Technologies

- C++17
- SFML 3.0 for graphics and audio
- Custom animation framework
- Object pooling for performance optimization
- Modular component architecture

---

## Gameplay

2D combat where players fight AI-controlled enemies. Combat includes punch/kick combinations, aerial attacks, and defensive parrying mechanics.

**Controls:** `WASD` movement • `J/K/L` combat • `F` parry • `S` enemy spawn

---

## Implementation

Built with separate manager systems for resources, audio, effects, input, and UI. Uses entity inheritance, animation state machines, and event-driven input handling.

---

## Status

**Functional** — All core systems implemented and working. Ongoing refinements to AI behavior and combat balance.
