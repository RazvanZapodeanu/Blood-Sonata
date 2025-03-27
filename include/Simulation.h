#ifndef SIMULATION_H
#define SIMULATION_H
#include "Environment.h"
#include <fstream>
#include <string>

class Simulation {
private:
    Environment environment;
    double time;
    double timeStep;
public:


    explicit Simulation(double width = 100.0, double height = 100.0, double timeStep = 0.1)
        : environment(width, height), time(0.0), timeStep(timeStep) {}


    Simulation(const Simulation& other)
        : environment(other.environment), time(other.time), timeStep(other.timeStep) {}




    ~Simulation() = default;


    [[nodiscard]] const Environment& getEnvironment() const { return environment; }
    [[nodiscard]] double getTime() const { return time; }
    [[nodiscard]] double getTimeStep() const { return timeStep; }


    void setTimeStep(double newTimeStep) { timeStep = newTimeStep; }


    void addParticle(const Particle& particle) {
        environment.addParticle(particle);
    }


    void step() {
        environment.update(timeStep);
        time += timeStep;
    }

    void run(int steps) {
        for (int i = 0; i < steps; ++i) {
            step();
        }
    }

    int loadFileData(const std::string& filename = "../Data/InputData.txt") {
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string name;
            double mass, radius, posX, posY, velX, velY;
            int red,green,blue;
            while (file >> name >> mass >> radius >> posX >> posY >> velX >> velY >> red >> green >> blue) {
                addParticle(Particle(name, mass, radius, Vector2D(posX, posY), Vector2D(velX, velY), red%256, green%256, blue%256));
            }

            file.close();
            return 1;
        } else {
            return 0;
        }
    }
    void reset() {
        environment.clearParticles();
        loadFileData();
    }

    friend std::ostream& operator<<(std::ostream& os, const Simulation& sim) {
        os << "Simulation State:\n";
        os << "  Time: " << sim.time << "\n";
        os << "  Time Step: " << sim.timeStep << "\n";
        os << sim.environment;
        return os;
    }
};


#endif
