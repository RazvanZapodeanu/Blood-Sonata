#include "../include/Camera.h"
#include <iostream>
#include <algorithm>

Camera::Camera(const sf::FloatRect& viewport)
    : m_view(viewport)
    , m_target(nullptr)
    , m_bounds(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 0.0f))
    , m_offset(0.0f, 0.0f)
{
}

Camera::~Camera() {

}

void Camera::update(float /* deltaTime */) {

    if (!m_target) {
        return;
    }
    

    sf::Vector2f targetPosition = m_target->getPosition();
    

    targetPosition += m_offset;
    

    sf::Vector2f clampedPosition = clampPosition(targetPosition);
    


    m_view.setCenter(sf::Vector2f(clampedPosition.x, m_view.getCenter().y));
}

void Camera::setTarget(Entity* target) {
    m_target = target;
}

sf::View Camera::getView() const {
    return m_view;
}

void Camera::setBounds(float left, float top, float width, float height) {
    m_bounds = sf::FloatRect(sf::Vector2f(left, top),sf::Vector2f( width, height));
}

void Camera::setOffset(float x, float y) {
    m_offset = sf::Vector2f(x, y);
}

sf::Vector2f Camera::clampPosition(const sf::Vector2f& position) const {

    if (m_bounds.size.x <= 0.0f || m_bounds.size.y <= 0.0f) {
        return position;
    }
    

    float halfViewWidth = m_view.getSize().x / 2.0f;
    float halfViewHeight = m_view.getSize().y / 2.0f;
    

    float x = std::max(m_bounds.position.x + halfViewWidth, std::min(position.x, m_bounds.position.x + m_bounds.size.x - halfViewWidth));
    float y = std::max(m_bounds.position.y + halfViewHeight, std::min(position.y, m_bounds.position.y + m_bounds.size.y - halfViewHeight));
    
    return {x, y};
}

std::ostream& operator<<(std::ostream& os, const Camera& camera) {
    os << "Camera: view center=(" << camera.m_view.getCenter().x << "," << camera.m_view.getCenter().y << ")"
       << ", view size=(" << camera.m_view.getSize().x << "," << camera.m_view.getSize().y << ")"
       << ", target=" << (camera.m_target ? "yes" : "no");
    
    return os;
}