#ifndef PARTICLE_H
#define PARTICLE_H

#if defined(__GNUC__) || defined(__clang__)
    #include <bits/algorithmfwd.h>
#else
    #include <algorithm>
#endif

#include "Vector2D.h"
#include "SimulationConfig.h"


class Particle {
private:
    static inline int nextId=1;
    int id;
    std::string name;
    double mass;
    double radius;
    Vector2D position;
    Vector2D velocity;
    int red,green,blue;
public:
    Particle (std::string name,
        const double mass,
        const double radius,
        const Vector2D& position = Vector2D(),
        const Vector2D& velocity =  Vector2D(),
        const int red=0, const int green=0, const int blue=0)
            :
    name(std::move(name)),
    mass(mass), radius(radius),
    position(position), velocity(velocity), red(red), green(green), blue(blue) {
        id=nextId;
    }
    Particle (const Particle& other) :
    id(nextId++),
    name(other.name),
    mass(other.mass),
    radius(other.radius),
    position(other.position),
    velocity(other.velocity),
    red(other.red),
    green(other.green),
    blue(other.blue) {}

    Particle& operator=(const Particle& other) {
        if (this!=&other) {
            name=other.name;
            mass=other.mass;
            radius=other.radius;
            position=other.position;
            velocity=other.velocity;
            red=other.red;
            green=other.green;
            blue=other.blue;
        }
        return *this;
    }
    virtual ~Particle() = default;

    [[nodiscard]] int getId() const { return id; }
    [[nodiscard]] const std::string& getName() const { return name; }
    [[nodiscard]] double getMass() const { return mass; }
    [[nodiscard]] double getRadius() const { return radius; }
    [[nodiscard]] const Vector2D& getPosition() const { return position; }
    [[nodiscard]] const Vector2D& getVelocity() const { return velocity; }
    [[nodiscard]] int getRed() const { return red;}
    [[nodiscard]] int getGreen() const { return green;}
    [[nodiscard]] int getBlue() const { return blue;}

    void setName(const std::string& name){this->name=name;}
    void setMass(double mass){this->mass=mass;}
    void setRadius(double radius){this->radius=radius;}
    void setPosition(const Vector2D& position) {
        this->position=position;
    }
    void setVelocity(const Vector2D& velocity) {
        this->velocity=velocity;
    }

    virtual void update(double deltaTime) {
        if (position.getY()==getRadius()) {
            velocity.setX(velocity.getX()*SimulationConfig::GroundFriction);
        }
        if (position.getY()!=0)
            velocity.setY(velocity.getY()-SimulationConfig::gravity*deltaTime);

        velocity.setX(velocity.getX()*SimulationConfig::AirFriction);
        velocity.setY(velocity.getY()*SimulationConfig::AirFriction);

        position.setX(std::clamp(getPosition().getX(), getRadius(), SimulationConfig::windowWidth - getRadius()));
        position.setY(std::clamp(getPosition().getY(), getRadius(), SimulationConfig::windowHeight - getRadius()));


        if (std::abs(velocity.getX()) < 0.01) velocity.setX(0);
        if (std::abs(velocity.getY()) < 0.01) velocity.setY(0);
        position.setX(position.getX()+velocity.getX()*deltaTime);
        position.setY(position.getY()+velocity.getY()*deltaTime);
    }

    void handleCollision(Particle& other) {

    double dx = other.position.getX() - position.getX();
    double dy = other.position.getY() - position.getY();
    double distance = std::sqrt(dx * dx + dy * dy);

    if (distance < 1e-6) return;

    Vector2D collisionDir(dx, dy);
    collisionDir.normalize();


    const double v1 = velocity.getX() * collisionDir.getX() + velocity.getY() * collisionDir.getY();
    const double v2 = other.velocity.getX() * collisionDir.getX() + other.velocity.getY() * collisionDir.getY();



    const double newV1 =(v1 * (mass - other.mass) + 2 * other.mass * v2) / (mass + other.mass);
    const double newV2 =(v2 * (other.mass - mass) + 2 * mass * v1) / (mass + other.mass);


    velocity.setX(SimulationConfig::elasticity*(velocity.getX() + (newV1 - v1) * collisionDir.getX()));
    velocity.setY(SimulationConfig::elasticity*(velocity.getY() + (newV1 - v1) * collisionDir.getY()));

    other.velocity.setX(SimulationConfig::elasticity*(other.velocity.getX() + (newV2 - v2) * collisionDir.getX()));
    other.velocity.setY(SimulationConfig::elasticity*(other.velocity.getY() + (newV2 - v2) * collisionDir.getY()));

    Vector2D tangent(-collisionDir.getY(), collisionDir.getX());

    double v1t = velocity.getX() * tangent.getX() + velocity.getY() * tangent.getY();
    double v2t = other.velocity.getX() * tangent.getX() + other.velocity.getY() * tangent.getY();


    double deltaVt = (v2t - v1t) * SimulationConfig::particleSurfaceFriction;

    velocity.setX(velocity.getX() + tangent.getX() * deltaVt * 0.5);
    velocity.setY(velocity.getY() + tangent.getY() * deltaVt * 0.5);
    other.velocity.setX(other.velocity.getX() - tangent.getX() * deltaVt * 0.5);
    other.velocity.setY(other.velocity.getY() - tangent.getY() * deltaVt * 0.5);

    double overlap = (radius + other.radius) - distance;
    if (overlap > 0) {
        double moveAmount = 1.001*overlap/2;

        position.setX(position.getX() - collisionDir.getX() * moveAmount);
        position.setY(position.getY() - collisionDir.getY() * moveAmount);
        other.position.setX(other.position.getX() + collisionDir.getX() * moveAmount);
        other.position.setY(other.position.getY() + collisionDir.getY() * moveAmount);
    }


}


    [[nodiscard]] bool collidesWith(const Particle& other) const {
        double dx = position.getX() - other.position.getX();
        double dy = position.getY() - other.position.getY();
        double distance = std::sqrt(dx * dx + dy * dy);
        return distance <= (radius + other.radius)*1.01;
    }

    friend std::ostream& operator<<(std::ostream& os, const Particle& p) {
        os << "Particle #" << p.id << " \"" << p.name << "\"\n";
        os << "  Mass: " << p.mass << "\n";
        os << "  Radius: " << p.radius << "\n";
        os << "  Position: " << p.position << "\n";
        os << "  Velocity: " << p.velocity << "\n";
        os << "  Color: (" << p.red << ", " << p.green << ", " << p.blue << ")\n";
        return os;
    }


};
#endif
