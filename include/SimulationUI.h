#pragma once
#include <SFML/Graphics.hpp>
#include "../include/PlatformAllocator.h"

class SimulationUI {
private:
    sf::RenderWindow window;
    const TrainScheduler& scheduler;
    float simTime; 
    bool isPlaying;
    float speedMultiplier;
    sf::Font font;

    void handleInput();
    void drawPlatforms();
    void drawTrains();
    void drawClock();
    void drawControls();

public:
    SimulationUI(const TrainScheduler& scheduler);
    void run();
};