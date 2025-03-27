#include "../include/Simulation.h"
#include "../include/ResourceManager.h"
#include "../include/SimulationConfig.h"
#include <iostream>


int main() {
    ResourceManager::load();

    Simulation sim(SimulationConfig::windowWidth, SimulationConfig::windowHeight, SimulationConfig::deltaTime);
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(SimulationConfig::windowWidth, SimulationConfig::windowHeight)),"Particles");
    window.setFramerateLimit(SimulationConfig::targetFPS);


    sf::Sprite background(ResourceManager::getBackground());
    background.setScale(sf::Vector2f(
        static_cast<float>(SimulationConfig::windowWidth) /  static_cast<float>(ResourceManager::getBackground().getSize().x),
        static_cast<float>(SimulationConfig::windowHeight) / static_cast<float>(ResourceManager::getBackground().getSize().y)
        ));


    if (!sim.loadFileData()) {
        std::cerr<<"Failed to load from file\n";
    }


    while (window.isOpen()) {
        while (std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                const auto key=keyPressed->scancode;
                if (key == sf::Keyboard::Scancode::Escape)
                    window.close();
                else if (key == sf::Keyboard::Scancode::Space) {
                    sim.addParticle(sim.getEnvironment().generateParticle());
                    ResourceManager::getSpawnSound().play();
                }
                else if (key == sf::Keyboard::Scancode::P) {
                    SimulationConfig::paused=!SimulationConfig::paused;
                }
                else if (key == sf::Keyboard::Scancode::R) {
                    sim.reset();
                }
            }
        }
        if (!SimulationConfig::paused) { sim.step(); }

        const auto& particles = sim.getEnvironment().getParticles();
        window.clear();
        window.draw(background);
        for (const auto& particle:particles) {
            sf::CircleShape shape(static_cast<float>(particle->getRadius()));
            shape.setPosition(sf::Vector2f(static_cast<float>(particle->getPosition().getX()-particle->getRadius()),
                static_cast<float>(SimulationConfig::windowHeight-particle->getPosition().getY()-particle->getRadius())));
            shape.setFillColor(sf::Color(particle->getRed(),particle->getGreen(),particle->getBlue()));
            window.draw(shape);

        }


        window.display();
    }
    return 0;
}



