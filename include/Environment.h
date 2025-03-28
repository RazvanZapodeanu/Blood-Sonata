#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include "Particle.h"
#include "ResourceManager.h"
#include "SimulationConfig.h"
class Environment {
private:
    double width;
    double height;
    std::vector<std::shared_ptr<Particle>> particles;
public:
    explicit Environment(const double width=100.0, const double height=100.0)
        :width(width), height(height) {}
    Environment(const Environment& other)
        :width(other.width), height(other.height) {
        for (const auto& p:other.particles) {
            particles.push_back(std::make_shared<Particle>(*p));
        }
    }

    Environment& operator=(const Environment& other) {
        if (this!=&other) {
            width=other.width;
            height=other.height;

            particles.clear();

            for (const auto& p:other.particles) {
                particles.push_back(std::make_shared<Particle>(*p));
            }
        }
        return *this;
    }

    ~Environment() = default;

    [[nodiscard]] double getWidth() const { return width; }
    [[nodiscard]] double getHeight() const { return height; }
    [[nodiscard]] const std::vector<std::shared_ptr<Particle>>& getParticles() const { return particles; }

    void setWidth(double newWidth) { width = newWidth; }
    void setHeight(double newHeight) { height = newHeight; }


    void addParticle(const Particle& particle) {
        particles.push_back(std::make_shared<Particle>(particle));
    }
    [[nodiscard]] Particle generateParticle() const {
        std::string randomName = "Particle_" + std::to_string(rand() % 1000);
        double randomMass = rand() % 100 + 1;
        double randomRadius = rand() % 11 + 5;
        Vector2D randomPosition;
        do{
            randomPosition.setX(std::round(static_cast<float>(rand())/RAND_MAX*SimulationConfig::windowWidth)*100/100.0f);
            randomPosition.setY(std::round(static_cast<float>(rand())/RAND_MAX*SimulationConfig::windowHeight)*100/100.0f);
        }while (isOverlapping(randomPosition,randomRadius));
        Vector2D randomVelocity(rand() % 200 - 100, rand() % 200 - 100);


        return {randomName, randomMass, randomRadius, randomPosition, randomVelocity,
            rand()%256, rand()%256, rand()%256};
    }

    [[nodiscard]] bool isOverlapping(const Vector2D& newPosition, const double& newRadius) const {
        for (const auto& particle:particles) {
            double dx=newPosition.getX()-particle->getPosition().getX();
            double dy=newPosition.getY()-particle->getPosition().getY();
            double distance=std::sqrt(dx*dx+dy*dy);
            if (distance < (newRadius+particle->getRadius()))
                return true;
        }
        return false;
    }
    void update(double const dt) {
        pullGravity(dt);
        for (auto& p : particles) {
            p->update(dt);


            Vector2D pos = p->getPosition();
            Vector2D vel = p->getVelocity();
            const double r = p->getRadius();

            if (pos.getX() < r) {
                p->setPosition(Vector2D(r, pos.getY()));
                p->setVelocity(Vector2D(-vel.getX()*SimulationConfig::elasticity, vel.getY()));
            }
            if (pos.getX() > width - r) {
                p->setPosition(Vector2D(width - r, pos.getY()));
                p->setVelocity(Vector2D(-vel.getX()*SimulationConfig::elasticity, vel.getY()));
            }

            if (pos.getY() < r) {
                p->setPosition(Vector2D(pos.getX(), r));
                p->setVelocity(Vector2D(vel.getX(), -vel.getY()*SimulationConfig::elasticity));
            }
            if (pos.getY() > height - r) {
                p->setPosition(Vector2D(pos.getX(), height - r));
                p->setVelocity(Vector2D(vel.getX(), -vel.getY()*SimulationConfig::elasticity));
            }

        }


        for (size_t i = 0; i < particles.size(); ++i) {
            for (size_t j = i + 1; j < particles.size(); ++j) {
                if (particles[i]->collidesWith(*particles[j])) {
                    Vector2D ItoJvelocity(particles[i]->getVelocity().getX()-particles[j]->getVelocity().getX(),particles[i]->getVelocity().getY()-particles[j]->getVelocity().getY());
                    double speed = std::sqrt(ItoJvelocity.getX()*ItoJvelocity.getX()+ItoJvelocity.getY()*ItoJvelocity.getY());


                    if (speed > 50.0) {
                        float volume = static_cast<float>(std::min(speed / 300.0, 1.0) * 100.0);

                        ResourceManager::getHitSound().setVolume(volume);
                        ResourceManager::getHitSound().play();

                    }

                    Vector2D ItoJposition(particles[i]->getPosition().getX()-particles[j]->getPosition().getX(),particles[i]->getPosition().getY()-particles[j]->getPosition().getY());

                    if (ItoJvelocity.dot(ItoJposition)<0)
                        particles[i]->handleCollision(*particles[j]);

                }
            }
        }
    }
    void pullGravity(const double dt) {
        const double G = SimulationConfig::particleGravity;
        const double softening = 1.0;

        std::vector<std::pair<double, double>> acc(particles.size(), {0.0, 0.0});

        for (size_t i = 0; i < particles.size(); ++i) {
            for (size_t j = i + 1; j < particles.size(); ++j) {
                auto& a = particles[i];
                auto& b = particles[j];

                double dx = particles[j]->getPosition().getX() - particles[i]->getPosition().getX();
                double dy = particles[j]->getPosition().getY() - particles[i]->getPosition().getY();

                double distSq = dx * dx + dy * dy + softening * softening;
                double dist = std::sqrt(distSq);

                double forceMag =std::min( G * particles[i]->getMass() * particles[j]->getMass() / (distSq * dist),5000.0);

                double fx = forceMag * dx / dist;
                double fy = forceMag * dy / dist;

                acc[i].first  += fx / particles[i]->getMass();
                acc[i].second += fy / particles[i]->getMass();

                acc[j].first  -= fx / particles[j]->getMass();
                acc[j].second -= fy / particles[j]->getMass();
            }
        }

        for (size_t i = 0; i < particles.size(); ++i) {
            auto vel = particles[i]->getVelocity();
            double vx = vel.getX() + acc[i].first * dt;
            double vy = vel.getY() + acc[i].second * dt;

            double px = particles[i]->getPosition().getX() + vx * dt;
            double py = particles[i]->getPosition().getY() + vy * dt;

            particles[i]->setVelocity(Vector2D(vx, vy));
            particles[i]->setPosition(Vector2D(px, py));
        }
    }

    void clearParticles() {
        particles.clear();
    }
    friend std::ostream& operator<<(std::ostream& os, const Environment& env) {
        os << "Environment:\n";
        os << "  Width: " << env.width << "\n";
        os << "  Height: " << env.height << "\n";
        os << "  Particles (" << env.particles.size() << "):\n";
        for (const auto& p : env.particles) {
            if (p) {
                os << "    " << *p << "\n";
            }
        }
        return os;
    }

};


#endif
