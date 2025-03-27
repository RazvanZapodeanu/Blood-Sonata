#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include "SFML/Audio/Music.hpp"
#include "SFML/Audio/Sound.hpp"
#include "SFML/Audio/SoundBuffer.hpp"
#include "SFML/Graphics.hpp"

class ResourceManager {
public:
    static void load();
    static sf::Sound& getSpawnSound();
    static sf::Sound& getHitSound();
    static sf::Music& getMusic();
    static sf::Texture& getBackground();
private:
    static inline sf::SoundBuffer spawnBuffer;
    static inline sf::SoundBuffer hitBuffer;
    static inline std::unique_ptr<sf::Sound> spawnSound;
    static inline std::unique_ptr<sf::Sound> hitSound;
    static inline sf::Music music;
    static inline sf::Texture background;
};
#endif
