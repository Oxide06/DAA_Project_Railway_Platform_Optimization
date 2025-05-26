#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include "../include/PlatformAllocator.h"

class SimulationUI {
private:
    sf::RenderWindow window;
    const TrainScheduler& scheduler;
    
    // Simulation state
    float simTime;
    bool isPlaying;
    float speedMultiplier;
    bool isFullscreen;

    // Graphics resources
    sf::Font font;
    sf::Texture trackTexture;
    sf::Texture trainTexture;
    std::vector<sf::Sprite> tracks;

    // Platform states
    struct PlatformState {
        bool occupied = false;
        sf::CircleShape indicator;
    };
    std::vector<PlatformState> platformStates;

    // Train status colors
    const sf::Color MOVING_COLOR;
    const sf::Color DOCKING_COLOR;
    const sf::Color STOPPED_COLOR;

    // UI controls
    sf::RectangleShape pauseBtn;
    sf::Text pauseLabel;
    sf::RectangleShape ffwBtn;
    sf::Text ffwLabel;
    sf::RectangleShape rewBtn;
    sf::Text rewLabel;
    sf::RectangleShape speedUpBtn;
    sf::Text speedUpLabel;
    sf::RectangleShape speedDownBtn;
    sf::Text speedDownLabel;

    // Animation functions
    float easeInOutCubic(float t);
    void toggleFullscreen();

    void handleInput();
    void drawTracks();
    void drawTrains();
    void drawClock();
    void drawControls();

public:
    SimulationUI(const TrainScheduler& scheduler);
    void run();
};