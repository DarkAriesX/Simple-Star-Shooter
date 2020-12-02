#include "Asteroids.hpp"

#include <array>
#include <cmath>
#include <iostream>


void control(sf::RenderWindow& window, sf::CircleShape& player, sf::Event& event);
void fire(sf::RenderWindow& window, sf::CircleShape& player, sf::Event& event, 
            sf::CircleShape& bullet);
void asteroidMove(sf::RenderWindow& window, sf::CircleShape& player, 
                    std::array<sf::CircleShape, 4>& asteroid, sf::CircleShape& bullet, 
                    sf::Text& scoreText);
int main() {
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
void control(sf::RenderWindow& window, sf::CircleShape& player, sf::Event& event) {
    while(window.isOpen()) {
        float accelleration = 5;
        while(window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                switch(event.key.code) {
                    case (sf::Keyboard::Left):
                        player.rotate(-accelleration);
                        accelleration*=2.5;
                        break;
                    case (sf::Keyboard::Right):
                        player.rotate(accelleration);
                        accelleration*=2.5;
                        break;
                }
            }
        }
            if ((sf::Joystick::isButtonPressed(0, 4)) && (sf::Joystick::isButtonPressed(0, 5))) {
                player.rotate(0);
            } else if (sf::Joystick::isButtonPressed(0, 4)) {
                player.rotate(-0.001);
            } else if (sf::Joystick::isButtonPressed(0, 5)) {
                player.rotate(0.001);
            } else if (sf::Joystick::isButtonPressed(0, 7)) {
                window.close();
            }
    }
}
void fire(sf::RenderWindow& window, sf::CircleShape& player, sf::Event& event, 
            sf::CircleShape& bullet) {
    sf::SoundBuffer* buffer = new sf::SoundBuffer;;
    buffer->loadFromFile("sounds/shot.ogg");
    sf::Sound* shot = new sf::Sound;
    shot->setBuffer(*buffer);
    while(window.isOpen()) {
        if (sf::Joystick::isButtonPressed(0, 0)) {
            fireShot(bullet, player, shot);
        }
        while(window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    fireShot(bullet, player, shot);
                }
            }
        }
    }  
}
void asteroidMove(sf::RenderWindow& window, sf::CircleShape& player, 
                    std::array<sf::CircleShape, 4>& asteroid, sf::CircleShape& bullet, 
                    sf::Text& scoreText) {
    sf::FloatRect bulletBox;
    sf::FloatRect asteroidBox;
    sf::FloatRect playerBox;
    int score = 0;
    sf::Clock clock[4];
    sf::SoundBuffer* buffer = new sf::SoundBuffer;;
    buffer->loadFromFile("sounds/boom.ogg");
    sf::Sound* boom = new sf::Sound;
    boom->setBuffer(*buffer);
    int asteroidClockIterator = 0;
    while(window.isOpen()) {
        float a{};
        float b{};
        for (auto& el : asteroid) {
            if (clock[asteroidClockIterator].getElapsedTime() >= sf::seconds(2)) {
                if ((el.getPosition().x >= 400) && (el.getPosition().y >= 300)) {
                    a = (el.getPosition().x - player.getPosition().x) * -1;
                    b = (el.getPosition().y - player.getPosition().y) * -1;
                } else if ((el.getPosition().x >= 400) && (el.getPosition().y < 300)) {
                    a = (el.getPosition().x - player.getPosition().x) * -1;
                    b = (el.getPosition().y - player.getPosition().y) * -1;
                } else if ((el.getPosition().x < 400) && (el.getPosition().y < 300)) {
                    a = (el.getPosition().x - player.getPosition().x) * -1;
                    b = (el.getPosition().y - player.getPosition().y) * -1;
                } else {
                    a = (el.getPosition().x - player.getPosition().x) * -1;
                    b = (el.getPosition().y - player.getPosition().y) * -1;
                }
                a = a/5000;
                b = b/5000;
                el.move(a, b);
            }
            bulletBox = bullet.getGlobalBounds();
            asteroidBox = el.getGlobalBounds();
            playerBox = player.getGlobalBounds();
            if (playerBox.intersects(asteroidBox)) {
                el.setPosition(player.getPosition());
                std::this_thread::sleep_for(std::chrono::seconds(3600));
            } else if(bulletBox.intersects(asteroidBox)) {
                boom->play();
                clock[asteroidClockIterator].restart();
                el.setPosition(generatePosition(side()));
                bullet.setPosition(1000, 1000);
                score++;
                scoreText.setString(generateScoreText(score));
            }
            asteroidClockIterator++;
            if (asteroidClockIterator > 3) {
                asteroidClockIterator = 0;
            }
        }
        std::this_thread::sleep_for(std::chrono::microseconds(400 - score));
    }
}
