#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>


class Animation {
public:

    Animation(const std::vector<std::string>& framePaths, float frameDuration);


    Animation(const Animation& other);


    Animation& operator=(const Animation& other);


    ~Animation();


    void update(float deltaTime);


    void applyToSprite(sf::Sprite& sprite) const;


    void reset();


    bool isFinished() const;


    bool isLooping() const;


    void setLooping(bool looping);


    friend std::ostream& operator<<(std::ostream& os, const Animation& animation);

private:

    std::vector<sf::Texture> m_frameTextures;


    float m_frameDuration;
    float m_totalTime;
    int m_currentFrame;
    int m_frameCount;


    bool m_isLooping;
};

#endif