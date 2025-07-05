#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <memory>


class SoundManager {
public:

    static SoundManager& getInstance();
    

    void loadSound(const std::string& id, const std::string& filePath);
    

    void loadMusic(const std::string& id, const std::string& filePath);
    

    void playSound(const std::string& id);
    

    void playMusic(const std::string& id, bool loop = true);
    

    void stopSound(const std::string& id);
    

    void stopMusic(const std::string& id);
    

    void stopAllSounds();
    

    void stopAllMusic();
    

    void setSoundVolume(float volume);
    

    void setMusicVolume(float volume);
    

    void setSoundVolume(const std::string& id, float volume);
    

    void setMusicVolume(const std::string& id, float volume);
    

    friend std::ostream& operator<<(std::ostream& os, const SoundManager& soundManager);

private:

    SoundManager();
    

    SoundManager(const SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete;
    

    ~SoundManager();
    

    struct SoundInfo {
        sf::SoundBuffer buffer{};
        sf::Sound sound;
        SoundInfo() : sound(buffer) {};
    };
    

    std::map<std::string, std::unique_ptr<SoundInfo>> m_sounds;
    

    std::map<std::string, std::unique_ptr<sf::Music>> m_music;
    

    float m_soundVolume;
    float m_musicVolume;
};

#endif