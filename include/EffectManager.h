#ifndef EFFECT_H
#define EFFECT_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>

class Effect {
private:
    sf::Texture m_texture;
    sf::Sprite m_sprite;
    sf::Vector2f m_position;
    float m_duration;
    float m_currentTime;
    bool m_loop;
    bool m_isFinished;
    int m_frameCount;
    sf::Vector2i m_frameSize;
    std::vector<sf::IntRect> m_frames;
    std::string m_originalTexturePath;

    void initializeFrames();
    void updateFrame();

public:
    Effect(const std::string& texturePath, const sf::Vector2f& position, float duration, bool loop = false);
    Effect(const Effect& other);
    Effect& operator=(const Effect& other);
    ~Effect();

    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    bool isFinished() const;
    void setFinished(bool finished);
    void reset(const sf::Vector2f& position);
    void resetComplete(const sf::Vector2f& position, const std::string& texturePath, float duration, bool loop);
    void setPosition(const sf::Vector2f& position);
    void setDuration(float duration);
    void setLoop(bool loop);

    friend std::ostream& operator<<(std::ostream& os, const Effect& effect);
};

#include "../include/templates/Pool.h"
#include <unordered_map>
#include <memory>

struct EffectInfo {
    std::string texturePath;
    float duration;
    bool loop;
};

class EffectManager {
private:
    static EffectManager* instance;
    Pool<Effect> m_effectPool;
    std::vector<std::unique_ptr<Effect>> m_activeEffects;
    std::unordered_map<std::string, EffectInfo> m_effectTypes;

    EffectManager();

public:
    static EffectManager& getInstance();
    ~EffectManager();

    void loadEffect(const std::string& id, const std::string& texturePath, float duration, bool loop = false);
    void createEffect(const std::string& id, const sf::Vector2f& position);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    void updateAndCleanup(float deltaTime);
    void cleanupFinishedEffects();
    void clearAllEffects();
    size_t getActiveEffectCount() const { return m_activeEffects.size(); }

    friend std::ostream& operator<<(std::ostream& os, const EffectManager& effectManager);
};

#endif