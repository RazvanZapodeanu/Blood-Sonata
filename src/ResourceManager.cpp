#include "../include/ResourceManager.h"
#include <iostream>

void ResourceManager::load() {
    if (!spawnBuffer.loadFromFile("../assets/sounds/spawnParticle.wav"))
        std::cerr << "Failed to load spawn sound\n";
    if (!hitBuffer.loadFromFile("../assets/sounds/collisionSound.wav"))
        std::cerr << "Failed to load hit sound\n";
    if (!background.loadFromFile("../assets/images/background.jpg"))
        std::cerr << "Failed to load background image\n";
    if (!music.openFromFile("../assets/sounds/ambience.ogg"))
        std::cerr << "Failed to load ambient music\n";

    spawnSound = std::make_unique<sf::Sound>(spawnBuffer);
    hitSound = std::make_unique<sf::Sound>(hitBuffer);
    music.setLooping(true);
    music.setVolume(20);
    music.play();
    spawnSound->setVolume(20);
}
sf::Sound& ResourceManager::getSpawnSound()   { return *spawnSound; }
sf::Sound& ResourceManager::getHitSound()     { return *hitSound;   }
sf::Music& ResourceManager::getMusic()        { return music;      }
sf::Texture& ResourceManager::getBackground() { return background; }
