#include "../include/EffectManager.h"
#include "../include/exceptions/GameException.h"
#include <iostream>



Effect::Effect(const std::string& texturePath, const sf::Vector2f& position, float duration, bool loop)
    : m_position(position)
    , m_sprite(m_texture)
    , m_duration(duration)
    , m_currentTime(0.0f)
    , m_loop(loop)
    , m_isFinished(false)
    , m_frameCount(1)
    , m_frameSize(64, 64)
    , m_originalTexturePath(texturePath)
{
    if (!m_texture.loadFromFile(texturePath)) {
        throw ResourceException("Failed to load texture for effect: " + texturePath);
    }

    sf::Vector2u textureSize = m_texture.getSize();
    m_frameSize = sf::Vector2i(textureSize.x / m_frameCount, textureSize.y);

    m_sprite.setTexture(m_texture);
    m_sprite.setPosition(m_position);
    m_sprite.setScale(sf::Vector2f(2.0f, 2.0f));

    initializeFrames();
    m_sprite.setOrigin(sf::Vector2f(m_frameSize.x / 2.0f, m_frameSize.y / 2.0f));
}

Effect::Effect(const Effect& other)
    : m_sprite(m_texture)
    , m_position(other.m_position)
    , m_duration(other.m_duration)
    , m_currentTime(other.m_currentTime)
    , m_loop(other.m_loop)
    , m_isFinished(other.m_isFinished)
    , m_frameCount(other.m_frameCount)
    , m_frameSize(other.m_frameSize)
    , m_frames(other.m_frames)
    , m_originalTexturePath(other.m_originalTexturePath)
{
    m_texture = other.m_texture;
    m_sprite = other.m_sprite;
    m_sprite.setTexture(m_texture);
}

Effect& Effect::operator=(const Effect& other) {
    if (this != &other) {
        m_position = other.m_position;
        m_duration = other.m_duration;
        m_currentTime = other.m_currentTime;
        m_loop = other.m_loop;
        m_isFinished = other.m_isFinished;
        m_frameCount = other.m_frameCount;
        m_frameSize = other.m_frameSize;
        m_frames = other.m_frames;
        m_originalTexturePath = other.m_originalTexturePath;

        m_texture = other.m_texture;
        m_sprite = other.m_sprite;
        m_sprite.setTexture(m_texture);
    }

    return *this;
}

Effect::~Effect() {
}

void Effect::update(float deltaTime) {
    if (m_isFinished) {
        return;
    }

    m_currentTime += deltaTime;

    if (m_currentTime >= m_duration) {
        if (m_loop) {
            m_currentTime = 0.0f;
        } else {
            m_isFinished = true;
        }
    }

    updateFrame();
}

void Effect::render(sf::RenderWindow& window) {
    if (m_isFinished) {
        return;
    }

    window.draw(m_sprite);
}

bool Effect::isFinished() const {
    return m_isFinished;
}

void Effect::setFinished(bool finished) {
    m_isFinished = finished;
}

void Effect::reset(const sf::Vector2f& position) {
    m_position = position;
    m_currentTime = 0.0f;
    m_isFinished = false;

    m_sprite.setPosition(m_position);
    updateFrame();
}

void Effect::resetComplete(const sf::Vector2f& position, const std::string& texturePath, float duration, bool loop) {
    m_position = position;
    m_duration = duration;
    m_currentTime = 0.0f;
    m_loop = loop;
    m_isFinished = false;

    if (m_originalTexturePath != texturePath) {
        if (!m_texture.loadFromFile(texturePath)) {
            std::cerr << "Failed to load texture: " << texturePath << std::endl;
            return;
        }
        m_originalTexturePath = texturePath;

        sf::Vector2u textureSize = m_texture.getSize();
        m_frameSize = sf::Vector2i(textureSize.x / m_frameCount, textureSize.y);

        m_frames.clear();
        initializeFrames();

        m_sprite.setTexture(m_texture);
        m_sprite.setOrigin(sf::Vector2f(m_frameSize.x / 2.0f, m_frameSize.y / 2.0f));
    }

    m_sprite.setPosition(m_position);
    updateFrame();
}

void Effect::setPosition(const sf::Vector2f& position) {
    m_position = position;
    m_sprite.setPosition(m_position);
}

void Effect::setDuration(float duration) {
    m_duration = duration;
}

void Effect::setLoop(bool loop) {
    m_loop = loop;
}

void Effect::initializeFrames() {
    int framesPerRow = m_texture.getSize().x / m_frameSize.x;

    for (int i = 0; i < m_frameCount; ++i) {
        int row = i / framesPerRow;
        int col = i % framesPerRow;

        sf::IntRect rect(sf::Vector2(
            col * m_frameSize.x,
            row * m_frameSize.y),
            sf::Vector2(
            m_frameSize.x,
            m_frameSize.y)
        );

        m_frames.push_back(rect);
    }
}

void Effect::updateFrame() {
    float frameTime = m_duration / static_cast<float>(m_frameCount);
    int frameIndex = static_cast<int>(m_currentTime / frameTime);

    frameIndex = std::min(frameIndex, m_frameCount - 1);

    if (frameIndex >= 0 && frameIndex < static_cast<int>(m_frames.size())) {
        m_sprite.setTextureRect(m_frames[frameIndex]);
    }
}

std::ostream& operator<<(std::ostream& os, const Effect& effect) {
    os << "Effect: position=(" << effect.m_position.x << "," << effect.m_position.y << ")"
       << ", duration=" << effect.m_duration
       << ", current time=" << effect.m_currentTime
       << ", loop=" << (effect.m_loop ? "yes" : "no")
       << ", finished=" << (effect.m_isFinished ? "yes" : "no");

    return os;
}

// Implementare pentru clasa EffectManager

EffectManager& EffectManager::getInstance() {
    static EffectManager instance;
    return instance;
}

EffectManager::EffectManager()
    : m_effectPool(20)
{
}

EffectManager::~EffectManager() {
}

void EffectManager::loadEffect(const std::string& id, const std::string& texturePath, float duration, bool loop) {
    if (m_effectTypes.find(id) != m_effectTypes.end()) {
        std::cout << "Type of effect " << id << " already exists in manager" << std::endl;
        return;
    }

    EffectInfo info;
    info.texturePath = texturePath;
    info.duration = duration;
    info.loop = loop;

    m_effectTypes[id] = info;

    for (int i = 0; i < 3; ++i) {
        try {
            auto effect = std::make_unique<Effect>(texturePath, sf::Vector2f(0.0f, 0.0f), duration, loop);
            effect->setFinished(true);
            m_effectPool.addObject(std::move(effect));
            std::cout << "Pre-populated pool effect " << i << " for " << id << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Failed to create pool effect for " << id << ": " << e.what() << std::endl;
        }
    }
}

void EffectManager::createEffect(const std::string& id, const sf::Vector2f& position) {
    auto it = m_effectTypes.find(id);
    if (it == m_effectTypes.end()) {
        std::cerr << "Type of effect " << id << " doesn't exist in manager!" << std::endl;
        return;
    }

    const EffectInfo& info = it->second;

    std::unique_ptr<Effect> effect = m_effectPool.acquire();

    if (effect) {
        effect->resetComplete(position, info.texturePath, info.duration, info.loop);
        std::cout << "Reusing pooled effect for " << id << " at (" << position.x << ", " << position.y << ")" << std::endl;
    } else {
        try {
            effect = std::make_unique<Effect>(info.texturePath, position, info.duration, info.loop);
            std::cout << "Created new effect for " << id << " at (" << position.x << ", " << position.y << ")" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Failed to create effect " << id << ": " << e.what() << std::endl;
            return;
        }
    }

    m_activeEffects.push_back(std::move(effect));
    std::cout << "Active effects count: " << m_activeEffects.size() << std::endl;
}

void EffectManager::update(float deltaTime) {
    for (auto& effect : m_activeEffects) {
        effect->update(deltaTime);
    }
}

void EffectManager::render(sf::RenderWindow& window) {
    for (auto& effect : m_activeEffects) {
        effect->render(window);
    }
}

void EffectManager::updateAndCleanup(float deltaTime) {
    for (auto& effect : m_activeEffects) {
        effect->update(deltaTime);
    }

    cleanupFinishedEffects();
}

void EffectManager::cleanupFinishedEffects() {
    size_t initialCount = m_activeEffects.size();

    for (auto it = m_activeEffects.begin(); it != m_activeEffects.end();) {
        if ((*it)->isFinished()) {
            std::cout << "Cleaning up finished effect" << std::endl;
            m_effectPool.release(std::move(*it));
            it = m_activeEffects.erase(it);
        } else {
            ++it;
        }
    }

    size_t finalCount = m_activeEffects.size();
    if (initialCount != finalCount) {
        std::cout << "Cleaned up " << (initialCount - finalCount) << " effects. Active: " << finalCount << std::endl;
    }
}

void EffectManager::clearAllEffects() {
    for (auto& effect : m_activeEffects) {
        m_effectPool.release(std::move(effect));
    }

    m_activeEffects.clear();
    std::cout << "Cleared all effects" << std::endl;
}

std::ostream& operator<<(std::ostream& os, const EffectManager& effectManager) {
    os << "EffectManager: effect types=" << effectManager.m_effectTypes.size()
       << ", active effects=" << effectManager.m_activeEffects.size()
       << ", pool size=" << effectManager.m_effectPool.getSize();

    return os;
}