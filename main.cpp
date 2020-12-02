#include "Asteroids.hpp"

#include <array>
#include <cmath>
#include <iostream>
#include <X11/Xlib.h>

int main() {
    XInitThreads();
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Asteroids", sf::Style::Default, settings);
    window.display();

    sf::Event event;

    sf::CircleShape player(20.f, 3);
    player.setPosition(400, 300);
    player.setScale(0.5, 1.25);
    player.setFillColor(sf::Color::Black);
    player.setOutlineThickness(5);
    player.setOutlineColor(sf::Color::White);
    player.setOrigin(player.getRadius(), player.getRadius());

    sf::CircleShape bullet(8.f, 5);
    bullet.setOrigin(bullet.getRadius(), bullet.getRadius());
    bullet.setFillColor(sf::Color::White);
    bullet.setPosition(1000, 1000);

    std::array<sf::CircleShape, 4> asteroid;
    for (auto& el : asteroid) {
        el.setRadius(20.f);
        el.setPointCount(8);
        el.setFillColor(sf::Color::White);
        el.setOrigin(el.getRadius(), el.getRadius());
        el.setPosition(generatePosition(side()));
    }

    sf::RectangleShape table(sf::Vector2f(800, 50));
    table.setFillColor(sf::Color::Black);
    table.setOutlineThickness(5);
    table.setOutlineColor(sf::Color::White);

    sf::RectangleShape gameOverScreen(sf::Vector2f(800, 600));
    gameOverScreen.setFillColor(sf::Color::Black);

    sf::Font font;
    font.loadFromFile("fonts/Hack-Regular.ttf");

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setCharacterSize(40);
    scoreText.setString(generateScoreText(0));

    std::thread* controlThread = new std::thread(control, std::ref(window), std::ref(player), std::ref(event));
    controlThread->detach();

    std::thread* fireThread = new std::thread(fire, std::ref(window), std::ref(player), 
                            std::ref(event), std::ref(bullet));
    fireThread->detach();

    std::thread* asteroidMoveThread = new std::thread(asteroidMove, std::ref(window), std::ref(player), 
                                    std::ref(asteroid), std::ref(bullet), std::ref(scoreText));
    asteroidMoveThread->detach();

    sf::SoundBuffer* buffer = new sf::SoundBuffer;;
    buffer->loadFromFile("sounds/background.ogg");
    sf::Sound* background = new sf::Sound;
    background->setBuffer(*buffer);
    background->play();
    while(window.isOpen()) {  
        window.clear();
        window.draw(bullet);
        for (auto& el : asteroid) {
            window.draw(el);
        }
        window.draw(player);
        window.draw(table);
        for (auto& el : asteroid) {
            if (player.getGlobalBounds().intersects(el.getGlobalBounds())) {
                window.draw(gameOverScreen);
                scoreText.setOrigin(scoreText.getLocalBounds().width / 2, 100);
                scoreText.setPosition(400, 300);
            }
        }
        window.draw(scoreText);
        window.display();
    }
    delete controlThread;
    delete fireThread;
    delete asteroidMoveThread;
    return 0;
}
