#include "../include/SimulationUI.h"
#include <iomanip>
#include <sstream>
#include <iostream>

SimulationUI::SimulationUI(const TrainScheduler& scheduler) 
    : scheduler(scheduler), simTime(0), isPlaying(false), speedMultiplier(1.0f) {
    window.create(sf::VideoMode(2000, 1800), "Railway Platform Simulation");
    if (!font.loadFromFile("/home/garv/Desktop/DAA_PROJECT/DAA_PBL/src/Textures/fonts/arial.ttf")) {
        std::cerr << "Error loading font\n";
    }
}

void SimulationUI::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
            
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::Space: isPlaying = !isPlaying; break;
                case sf::Keyboard::Up: speedMultiplier = std::min(4.0f, speedMultiplier + 0.2f); break;
                case sf::Keyboard::Down: speedMultiplier = std::max(0.2f, speedMultiplier - 0.2f); break;
                case sf::Keyboard::Right: simTime += 10; break;
                case sf::Keyboard::Left: simTime = std::max(0.0f, simTime - 10); break;
            }
        }
    }
}

void SimulationUI::drawPlatforms() {
    const float startX = 50.f;
    const float platformHeight = 80.f;
    const float spacing = 30.f;
    const float platformWidth = window.getSize().x - 2 * startX;

    sf::Text platformText("", font, 24);
    platformText.setFillColor(sf::Color::White);

    for (int i = 0; i < scheduler.getPlatformCount(); ++i) {
        sf::RectangleShape platform(sf::Vector2f(platformWidth, platformHeight));
        float yPos = 100.f + i * (platformHeight + spacing);
        platform.setPosition(startX, yPos);
        platform.setOutlineThickness(2);
        platform.setOutlineColor(sf::Color::White);
        platform.setFillColor(sf::Color(50, 50, 50));
        window.draw(platform);

        platformText.setString("Platform " + std::to_string(i+1));
        platformText.setPosition(startX + 20, yPos + (platformHeight - 30)/2);
        window.draw(platformText);
    }
}

void SimulationUI::drawTrains() {
    const float startX = 50.f;
    const float platformHeight = 80.f;
    const float spacing = 30.f;
    const float platformWidth = window.getSize().x - 2 * startX;
    const float trainWidth = 150.f;
    const float trainHeight = 25.f;

    sf::Text trainText("", font, 18);
    trainText.setFillColor(sf::Color::White);
    trainText.setOutlineColor(sf::Color::Black);
    trainText.setOutlineThickness(1);

    for (const auto& assignment : scheduler.getAssignments()) {
        if (simTime >= assignment.arrival && simTime < assignment.departure) {
            int platformIndex = assignment.platform - 1;
            float platformY = 100.f + platformIndex * (platformHeight + spacing);
            float trainY = platformY + (platformHeight - trainHeight)/2;

            // Calculate horizontal position
            float duration = assignment.departure - assignment.arrival;
            if (duration <= 0) duration = 1;
            float elapsed = simTime - assignment.arrival;
            float progress = std::min(elapsed / duration, 1.0f);
            
            float xPos = (startX + platformWidth - trainWidth) - 
                         (platformWidth - trainWidth) * progress;

            // Draw train
            sf::RectangleShape train(sf::Vector2f(trainWidth, trainHeight));
            train.setPosition(xPos, trainY);
            train.setFillColor(sf::Color(100, 250, 100));
            window.draw(train);

            // Center text on train
            trainText.setString(assignment.trainID);
            sf::FloatRect textBounds = trainText.getLocalBounds();
            trainText.setOrigin(textBounds.left + textBounds.width/2.0f, 
                              textBounds.top + textBounds.height/2.0f);
            trainText.setPosition(xPos + trainWidth/2, trainY + trainHeight/2);
            window.draw(trainText);
        }
    }
}

void SimulationUI::drawClock() {
    // Background panel
    sf::RectangleShape bg(sf::Vector2f(300, 50));
    bg.setPosition(40, 20);
    bg.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(bg);

    int totalMinutes = static_cast<int>(simTime);
    int hours = totalMinutes / 60;
    int minutes = totalMinutes % 60;

    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << hours << ":"
       << std::setw(2) << std::setfill('0') << minutes;

    sf::Text clockText("Time: " + ss.str(), font, 24);
    clockText.setPosition(50, 30);
    clockText.setFillColor(sf::Color::Yellow);
    window.draw(clockText);

    sf::Text speedText("Speed: " + std::to_string(speedMultiplier) + "x", font, 24);
    speedText.setPosition(250, 30);
    speedText.setFillColor(sf::Color::Cyan);
    window.draw(speedText);
}

void SimulationUI::drawControls() {
    // Background panel
    sf::RectangleShape bg(sf::Vector2f(350, 120));
    bg.setPosition(790, 20);
    bg.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(bg);

    sf::Text controlsText(
        "Controls:\n"
        "Space - Play/Pause\n"
        "Up/Down - Adjust speed (0.2x-4x)\n"
        "Left/Right - Step time (10min)\n",
        font, 20
    );
    controlsText.setPosition(800, 30);
    controlsText.setFillColor(sf::Color::White);
    window.draw(controlsText);
}

void SimulationUI::run() {
    sf::Clock deltaClock;
    while (window.isOpen()) {
        handleInput();

        if (isPlaying) {
            float delta = deltaClock.restart().asSeconds();
            simTime += delta * speedMultiplier * 60;
        }

        window.clear(sf::Color(40, 40, 40));
        drawPlatforms();
        drawTrains();
        drawClock();
        drawControls();
        window.display();
    }
}