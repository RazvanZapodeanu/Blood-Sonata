#include "../include/SoundManager.h"
#include <iostream>

SoundManager& SoundManager::getInstance() {
    static SoundManager instance;
    return instance;
}

SoundManager::SoundManager()
    : m_soundVolume(100.0f)
    , m_musicVolume(100.0f)
{
}

SoundManager::~SoundManager() {

    stopAllSounds();
    stopAllMusic();
}

void SoundManager::loadSound(const std::string& id, const std::string& filePath) {

    if (m_sounds.find(id) != m_sounds.end()) {
        std::cout << "Sound " << id << " already exists in manager" << std::endl;
        return;
    }
    

    auto soundInfo = std::make_unique<SoundInfo>();
    

    if (!soundInfo->buffer.loadFromFile(filePath)) {
        std::cerr << "Error loading sound: " << filePath << std::endl;
        return;
    }



    soundInfo->sound.setBuffer(soundInfo->buffer);
    

    soundInfo->sound.setVolume(m_soundVolume);
    

    m_sounds[id] = std::move(soundInfo);
}

void SoundManager::loadMusic(const std::string& id, const std::string& filePath) {

    if (m_music.find(id) != m_music.end()) {
        std::cout << "Music " << id << " already exists in manager" << std::endl;
        return;
    }
    

    auto music = std::make_unique<sf::Music>();
    

    if (!music->openFromFile(filePath)) {
        std::cerr << "Error loading music: " << filePath << std::endl;
        return;
    }
    

    music->setVolume(m_musicVolume);


    m_music[id] = std::move(music);
}

void SoundManager::playSound(const std::string& id) {

    auto it = m_sounds.find(id);
    if (it == m_sounds.end()) {
        std::cerr << "Sound " << id << " doesn't exist in manager" << std::endl;
        return;
    }
    

    it->second->sound.play();
}

void SoundManager::playMusic(const std::string& id, bool loop) {

    auto it = m_music.find(id);
    if (it == m_music.end()) {
        std::cerr << "Music " << id << " doesn't exist in manager" << std::endl;
        return;
    }
    

    it->second->setLooping(loop);
    

    it->second->play();
}

void SoundManager::stopSound(const std::string& id) {

    auto it = m_sounds.find(id);
    if (it == m_sounds.end()) {
        std::cerr << "Sound " << id << " doesn't exist in manager" << std::endl;
        return;
    }
    

    it->second->sound.stop();
}

void SoundManager::stopMusic(const std::string& id) {

    auto it = m_music.find(id);
    if (it == m_music.end()) {
        std::cerr << "Music " << id << " doesn't exist in manager" << std::endl;
        return;
    }
    

    it->second->stop();
}

void SoundManager::stopAllSounds() {

    for (auto& pair : m_sounds) {
        pair.second->sound.stop();
    }
}

void SoundManager::stopAllMusic() {

    for (auto& pair : m_music) {
        pair.second->stop();
    }
}

void SoundManager::setSoundVolume(float volume) {

    m_soundVolume = std::max(0.0f, std::min(100.0f, volume));
    

    for (auto& pair : m_sounds) {
        pair.second->sound.setVolume(m_soundVolume);
    }
}

void SoundManager::setMusicVolume(float volume) {

    m_musicVolume = std::max(0.0f, std::min(100.0f, volume));
    

    for (auto& pair : m_music) {
        pair.second->setVolume(m_musicVolume);
    }
}

void SoundManager::setSoundVolume(const std::string& id, float volume) {

    auto it = m_sounds.find(id);
    if (it == m_sounds.end()) {
        std::cerr << "Sound " << id << " doesn't exist in manager" << std::endl;
        return;
    }
    

    float clampedVolume = std::max(0.0f, std::min(100.0f, volume));
    

    it->second->sound.setVolume(clampedVolume);
}

void SoundManager::setMusicVolume(const std::string& id, float volume) {

    auto it = m_music.find(id);
    if (it == m_music.end()) {
        std::cerr << "Music " << id << " doesn't exist in manager" << std::endl;
        return;
    }
    

    float clampedVolume = std::max(0.0f, std::min(100.0f, volume));
    

    it->second->setVolume(clampedVolume);
}

std::ostream& operator<<(std::ostream& os, const SoundManager& soundManager) {
    os << "SoundManager: sounds=" << soundManager.m_sounds.size()
       << ", music=" << soundManager.m_music.size()
       << ", sound volume=" << soundManager.m_soundVolume
       << ", music volume=" << soundManager.m_musicVolume;
    
    return os;
}