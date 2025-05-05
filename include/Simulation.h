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


    explicit Simulation(double simWidth = 100.0, double simHeight = 100.0, double simTimeStep = 0.1)
        : environment(simWidth, simHeight), time(0.0), timeStep(simTimeStep) {}


    Simulation(const Simulation& other)
        : environment(other.environment), time(other.time), timeStep(other.timeStep) {}


    ~Simulation() = default;


    [[nodiscard]] const Environment& getEnvironment() const { return environment; }
    [[nodiscard]] double getTime() const { return time; }
    [[nodiscard]] double getTimeStep() const { return timeStep; }


    void setTimeStep(double newTimeStep) { timeStep = newTimeStep; }


    void setDetlaTime(float deltatime) {
        timeStep=deltatime;
    }


    void addParticle(const Particle& particle) {
        environment.addParticle(particle);
    }


    void step() {
        for (int i =0 ; i < 2 ; i++)
            environment.update(timeStep/2);
        time += timeStep;
    }


    void run(int steps) {
        for (int i = 0; i < steps; ++i) {
            step();
        }
    }


    int loadFileData(int index = -1) {
        std::string filename;
        if (index == -1) {
            filename = "./Data/InputData.txt";
        } else {
            filename = "./Data/InputData" + std::to_string(index) + ".txt";
        }

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


    void reset(const int fileNumber) {
        environment.clearParticles();
        loadFileData(fileNumber);
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
