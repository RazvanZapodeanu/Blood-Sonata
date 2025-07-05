#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Graphics.hpp>
#include "Entity.h"


class Camera {
public:

    Camera(const sf::FloatRect& viewport);
    

    ~Camera();
    

    void update(float deltaTime);
    

    void setTarget(Entity* target);
    

    sf::View getView() const;
    

    void setBounds(float left, float top, float width, float height);
    

    void setOffset(float x, float y);
    

    friend std::ostream& operator<<(std::ostream& os, const Camera& camera);

private:

    sf::View m_view;
    

    Entity* m_target;
    

    sf::FloatRect m_bounds;
    

    sf::Vector2f m_offset;
    

    sf::Vector2f clampPosition(const sf::Vector2f& position) const;
};

#endif