#include "../include/Simulation.h"
#include "../include/ResourceManager.h"
#include "../include/SimulationConfig.h"
#include <iostream>
#include "imgui.h"
#include "imgui-SFML.h"

int main() {
    ResourceManager::load();
    const int FileNumberToLoad=1;
    Simulation sim(SimulationConfig::windowWidth, SimulationConfig::windowHeight, SimulationConfig::deltaTime);
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(SimulationConfig::windowWidth, SimulationConfig::windowHeight)),"Particles");
    window.setFramerateLimit(SimulationConfig::targetFPS);
    if (!ImGui::SFML::Init(window)) {
        std::cerr << "ImGui::SFML::Init failed!\n";
        return -1;
    }
    sf::Clock deltaClock;

    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = 1.3f;

    sf::Sprite background(ResourceManager::getBackground());
    background.setScale(sf::Vector2f(
        static_cast<float>(SimulationConfig::windowWidth) /  static_cast<float>(ResourceManager::getBackground().getSize().x),
        static_cast<float>(SimulationConfig::windowHeight) / static_cast<float>(ResourceManager::getBackground().getSize().y)
        ));


    if (!sim.loadFileData(FileNumberToLoad)) {
        std::cerr<<"Failed to load from file\n";
    }


    while (window.isOpen()) {

        while (const auto event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);

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
                    sim.reset(FileNumberToLoad);
                }
            }


        }
        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Simulation Controls");

        ImGui::SliderFloat("Gravity", &SimulationConfig::gravity, -50.0f, 50.0f);
        ImGui::SliderFloat("Particle Gravity", &SimulationConfig::particleGravity, 0.0f, 1000.0f);
        ImGui::SliderFloat("Gravity Radius", &SimulationConfig::GravityRadius,0.0f,2500.0f);
        ImGui::SliderFloat("Elasticity", &SimulationConfig::elasticity, 0.0f, 1.5f);
        if (ImGui::SliderFloat("Delta Time", &SimulationConfig::deltaTime, 0.001f, 1.0f))
            {sim.setDetlaTime(SimulationConfig::deltaTime);}
        ImGui::SliderInt("Target FPS", &SimulationConfig::targetFPS, 1, 2000);
        ImGui::SliderFloat("Ground Friction", &SimulationConfig::GroundFriction, 0.0f, 1.0f);
        ImGui::SliderFloat("Air Friction", &SimulationConfig::AirFriction, 0.0f, 1.0f);

        ImGui::Text("Paused: %s", SimulationConfig::paused ? "true" : "false");
        if (ImGui::Button(SimulationConfig::paused ? "Resume" : "Pause"))
            SimulationConfig::paused = !SimulationConfig::paused;

        if (ImGui::Button("Reset Simulation"))
            sim.reset(FileNumberToLoad);

        ImGui::End();
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

        ImGui::SFML::Render(window);
        window.display();
        float dt = deltaClock.getElapsedTime().asSeconds();
        while (dt < 1.0f / SimulationConfig::targetFPS) {
            dt = deltaClock.getElapsedTime().asSeconds();
        }
        deltaClock.restart();
    }
    ImGui::SFML::Shutdown();
    return 0;
}



