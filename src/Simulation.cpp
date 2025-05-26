#include "../include/SimulationUI.h"
#include <iomanip>
#include <sstream>
#include <iostream>
#include <cmath>

SimulationUI::SimulationUI(const TrainScheduler& scheduler) 
    : scheduler(scheduler), simTime(0), isPlaying(false), speedMultiplier(1.0f), isFullscreen(true),
      MOVING_COLOR(100, 250, 100), DOCKING_COLOR(255, 255, 100), STOPPED_COLOR(250, 100, 100) {
    window.create(sf::VideoMode::getDesktopMode(), "Railway Platform Simulation", sf::Style::Fullscreen);
    
    // Load resources
    if (!font.loadFromFile("/home/garv/Desktop/DAA_PROJECT/DAA_PBL/src/Textures/fonts/arial.ttf")) {
        std::cerr << "Error loading font\n";
    }
    if (!trackTexture.loadFromFile("/home/garv/Desktop/DAA_PROJECT/DAA_PBL/src/Textures/track.png")) {
        std::cerr << "Error loading track texture\n";
    }
    if (!trainTexture.loadFromFile("/home/garv/Desktop/DAA_PROJECT/DAA_PBL/src/Textures/train.png")) {
        std::cerr << "Error loading train texture\n";
    }

    // Initialize tracks and platform states
    const float trackBaseY = 150.f;
    const float trackVerticalSpacing = 260.f;
    const float trackScaleX = (window.getSize().x - 200.f) / trackTexture.getSize().x;
    
    platformStates.resize(scheduler.getPlatformCount());
    for(int i = 0; i < scheduler.getPlatformCount(); ++i) {
        // Track setup
        sf::Sprite track(trackTexture);
        track.setPosition(100.f, trackBaseY + i * trackVerticalSpacing);
        track.setScale(trackScaleX, 0.4f);
        tracks.push_back(track);

        // Platform indicators
        platformStates[i].indicator.setRadius(15.f);
        platformStates[i].indicator.setOrigin(15.f, 15.f);
        platformStates[i].indicator.setPosition(
            track.getPosition().x - 40.f,
            track.getPosition().y + track.getGlobalBounds().height/2
        );
    }
}

float SimulationUI::easeInOutCubic(float t) {
    return t < 0.5 ? 4 * t * t * t : 1 - std::pow(-2 * t + 2, 3) / 2;
}

void SimulationUI::toggleFullscreen() {
    static sf::VideoMode fullscreenMode = sf::VideoMode::getDesktopMode();
    static sf::VideoMode windowedMode(1280, 720);

    if (isFullscreen) {
        window.create(windowedMode, "Railway Platform Simulation", sf::Style::Default);
    } else {
        window.create(fullscreenMode, "Railway Platform Simulation", sf::Style::Fullscreen);
    }
    isFullscreen = !isFullscreen;
}

void SimulationUI::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
            
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::Space:
                    isPlaying = !isPlaying;
                    pauseLabel.setString(isPlaying ? "Pause" : "Play");
                    break;
                case sf::Keyboard::Right:
                    simTime += 30;
                    break;
                case sf::Keyboard::Left:
                    simTime = std::max(0.f, simTime - 30);
                    break;
                case sf::Keyboard::Up:
                    speedMultiplier = std::min(4.f, speedMultiplier + 0.5f);
                    break;
                case sf::Keyboard::Down:
                    speedMultiplier = std::max(0.25f, speedMultiplier - 0.5f);
                    break;
                case sf::Keyboard::F11:
                    toggleFullscreen();
                    break;
            }
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            auto mousePos = sf::Mouse::getPosition(window);
            
            if (pauseBtn.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                isPlaying = !isPlaying;
                pauseLabel.setString(isPlaying ? "Pause" : "Play");
            }
            if (ffwBtn.getGlobalBounds().contains(mousePos.x, mousePos.y)) 
                simTime += 30;
            if (rewBtn.getGlobalBounds().contains(mousePos.x, mousePos.y)) 
                simTime = std::max(0.f, simTime - 30);
            if (speedUpBtn.getGlobalBounds().contains(mousePos.x, mousePos.y)) 
                speedMultiplier = std::min(4.f, speedMultiplier * 2);
            if (speedDownBtn.getGlobalBounds().contains(mousePos.x, mousePos.y)) 
                speedMultiplier = std::max(0.25f, speedMultiplier / 2);
        }
    }
}

void SimulationUI::drawTracks() {
    // Update platform occupancy states
    for(size_t i = 0; i < platformStates.size(); ++i) {
        bool occupied = false;
        for (const auto& assignment : scheduler.getAssignments()) {
            if (assignment.platform == static_cast<int>(i+1) && 
                simTime >= assignment.arrival && 
                simTime < assignment.departure) {
                occupied = true;
                break;
            }
        }
        platformStates[i].indicator.setFillColor(occupied ? 
            sf::Color(255, 50, 50) : sf::Color(50, 255, 50));
    }

    // Draw all track elements
    for(size_t i = 0; i < tracks.size(); ++i) {
        window.draw(tracks[i]);
        
        sf::Text platformLabel("Platform " + std::to_string(i+1), font, 28);
        platformLabel.setPosition(115.f, tracks[i].getPosition().y + 100.f);
        platformLabel.setFillColor(sf::Color::White);
        window.draw(platformLabel);

        window.draw(platformStates[i].indicator);
    }
}

void SimulationUI::drawTrains() {
    const float trainScale = 0.3f;
    const float trackStart = 100.f;
    const float trackEnd = window.getSize().x - 100.f;
    const float trainWidth = trainTexture.getSize().x * trainScale;

    for(const auto& assignment : scheduler.getAssignments()) {
        if(simTime >= assignment.arrival && simTime < assignment.departure) {
            int platformIndex = assignment.platform - 1;
            if(platformIndex >= 0 && platformIndex < static_cast<int>(tracks.size())) {
                sf::Sprite train(trainTexture);
                
                float duration = assignment.departure - assignment.arrival;
                float rawProgress = (simTime - assignment.arrival) / duration;
                float animatedProgress = 0.f;
                sf::Color trainColor = MOVING_COLOR;

                if (rawProgress < 0.2f) {
                    float phaseProgress = rawProgress / 0.2f;
                    animatedProgress = easeInOutCubic(phaseProgress) * 0.4f;
                    trainColor = DOCKING_COLOR;
                }
                else if (rawProgress > 0.8f) {
                    float phaseProgress = (rawProgress - 0.8f) / 0.2f;
                    animatedProgress = 0.6f + easeInOutCubic(phaseProgress) * 0.4f;
                    trainColor = DOCKING_COLOR;
                }
                else {
                    animatedProgress = 0.4f + (rawProgress - 0.2f) * 0.2f;
                    trainColor = STOPPED_COLOR;
                }

                float availableSpace = trackEnd - trackStart - trainWidth;
                float xPos = trackStart + (animatedProgress * availableSpace);
                xPos = std::clamp(xPos, trackStart, trackEnd - trainWidth);

                float trackY = tracks[platformIndex].getPosition().y;
                float trainY = trackY + (trackTexture.getSize().y * 0.425f)/2 - 
                             (trainTexture.getSize().y * trainScale)/2;
                
                train.setPosition(xPos, trainY);
                train.setScale(trainScale, trainScale);
                train.setColor(trainColor);
                window.draw(train);
            }
        }
    }
}

void SimulationUI::drawControls() {
    const float buttonY = window.getSize().y - 100.f;
    auto createButton = [&](float x, const std::string& text, int width = 100) {
        sf::RectangleShape btn(sf::Vector2f(width, 50));
        btn.setPosition(x, buttonY);
        btn.setFillColor(sf::Color(70, 70, 70));
        btn.setOutlineThickness(1);
        btn.setOutlineColor(sf::Color::White);
        
        sf::Text label;
        label.setFont(font);
        label.setString(text);
        label.setCharacterSize(24);
        label.setPosition(x + 15, buttonY + 10);
        label.setFillColor(sf::Color::White);
        
        return std::make_pair(btn, label);
    };

    std::tie(pauseBtn, pauseLabel) = createButton(200, isPlaying ? "Pause" : "Play");
    std::tie(ffwBtn, ffwLabel) = createButton(350, ">>");
    std::tie(rewBtn, rewLabel) = createButton(500, "<<");
    std::tie(speedUpBtn, speedUpLabel) = createButton(650, "+Speed", 140);
    std::tie(speedDownBtn, speedDownLabel) = createButton(800, "-Speed", 140);
    
    sf::RectangleShape speedDisplay(sf::Vector2f(100, 50));
    speedDisplay.setPosition(950, buttonY);
    speedDisplay.setFillColor(sf::Color(50, 50, 50));
    speedDisplay.setOutlineThickness(1);
    speedDisplay.setOutlineColor(sf::Color::White);
    
    sf::Text speedText;
    speedText.setFont(font);
    speedText.setString(std::to_string(speedMultiplier) + "x");
    speedText.setCharacterSize(24);
    speedText.setPosition(980, buttonY + 10);
    speedText.setFillColor(sf::Color::Cyan);

    auto drawControl = [&](auto& btn, auto& label) {
        window.draw(btn);
        window.draw(label);
    };

    drawControl(pauseBtn, pauseLabel);
    drawControl(ffwBtn, ffwLabel);
    drawControl(rewBtn, rewLabel);
    drawControl(speedUpBtn, speedUpLabel);
    drawControl(speedDownBtn, speedDownLabel);
    window.draw(speedDisplay);
    window.draw(speedText);
}

void SimulationUI::drawClock() {
    int totalMinutes = static_cast<int>(simTime);
    int hours = totalMinutes / 60;
    int minutes = totalMinutes % 60;

    sf::Text timeDisplay;
    timeDisplay.setFont(font);
    timeDisplay.setString(
        (hours < 10 ? "0" : "") + std::to_string(hours) + ":" +
        (minutes < 10 ? "0" : "") + std::to_string(minutes));
    timeDisplay.setCharacterSize(40);
    timeDisplay.setFillColor(sf::Color::White);
    timeDisplay.setPosition(window.getSize().x/2 - 60, 20.f);
    
    window.draw(timeDisplay);
}

void SimulationUI::run() {
    sf::Clock deltaClock;
    while (window.isOpen()) {
        handleInput();

        if (isPlaying) {
            float delta = deltaClock.restart().asSeconds();
            simTime += delta * speedMultiplier * 60;
        }

        window.clear(sf::Color::Black);
        drawTracks();
        drawTrains();
        drawClock();
        drawControls();
        window.display();
    }
}