#include "../include/Animation.h"
#include <iostream>

Animation::Animation(const std::vector<std::string>& framePaths, float frameDuration)
    : m_frameDuration(frameDuration)
    , m_totalTime(0.0f)
    , m_currentFrame(0)
    , m_frameCount(framePaths.size())
    , m_isLooping(true)
{

    m_frameTextures.resize(framePaths.size());

    for (size_t i = 0; i < framePaths.size(); ++i) {
        if (!m_frameTextures[i].loadFromFile(framePaths[i])) {
            throw std::runtime_error("Failed to load frame: " + framePaths[i]);
        }
    }
}

Animation::Animation(const Animation& other)
    : m_frameDuration(other.m_frameDuration)
    , m_totalTime(other.m_totalTime)
    , m_currentFrame(other.m_currentFrame)
    , m_frameCount(other.m_frameCount)
    , m_isLooping(other.m_isLooping)
    , m_frameTextures(other.m_frameTextures)
{
}

Animation& Animation::operator=(const Animation& other) {
    if (this != &other) {
        m_frameDuration = other.m_frameDuration;
        m_totalTime = other.m_totalTime;
        m_currentFrame = other.m_currentFrame;
        m_frameCount = other.m_frameCount;
        m_isLooping = other.m_isLooping;
        m_frameTextures = other.m_frameTextures;
    }
    return *this;
}

Animation::~Animation() {

}

void Animation::update(float deltaTime) {

    if (isFinished()) {
        return;
    }

    m_totalTime += deltaTime;


    static float debugTimer = 0.0f;
    debugTimer += deltaTime;
    if (debugTimer > 1.0f) {
        std::cout << "Animation update - frame: " << m_currentFrame
                  << "/" << m_frameCount << ", time: " << m_totalTime
                  << ", looping: " << m_isLooping << std::endl;
        debugTimer = 0.0f;
    }



    float frameIndex = m_totalTime / m_frameDuration;

    if (m_isLooping) {

        m_currentFrame = static_cast<int>(frameIndex) % m_frameCount;
    } else {

        int newFrame = std::min(static_cast<int>(frameIndex), m_frameCount - 1);


        if (newFrame != m_currentFrame) {
            std::cout << "Non-looping animation frame change: " << m_currentFrame << " -> " << newFrame << std::endl;
        }

        m_currentFrame = newFrame;
    }
}

void Animation::applyToSprite(sf::Sprite& sprite) const {
    if (m_currentFrame >= 0 && m_currentFrame < static_cast<int>(m_frameTextures.size())) {
        sprite.setTexture(m_frameTextures[m_currentFrame]);
        sprite.setTextureRect(sf::IntRect(sf::Vector2(0, 0),
        sf::Vector2(
            static_cast<int>(m_frameTextures[m_currentFrame].getSize().x),
            static_cast<int>(m_frameTextures[m_currentFrame].getSize().y))));
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(sf::Vector2f(bounds.size.x / 2.0f, bounds.size.y/7.0f));
    }
}

void Animation::reset() {
    m_totalTime = 0.0f;
    m_currentFrame = 0;
}

bool Animation::isFinished() const {
    return !m_isLooping && m_currentFrame >= m_frameCount - 1;
}

bool Animation::isLooping() const {
    return m_isLooping;
}

void Animation::setLooping(bool looping) {
    m_isLooping = looping;
}

std::ostream& operator<<(std::ostream& os, const Animation& animation) {
    os << "Animation: frames=" << animation.m_frameCount
       << ", current=" << animation.m_currentFrame
       << ", duration=" << animation.m_frameDuration
       << ", looping=" << (animation.m_isLooping ? "yes" : "no");
    
    return os;
}