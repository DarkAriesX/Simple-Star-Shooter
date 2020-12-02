#include "Asteroids.hpp"

#include <random>

std::random_device rd;
std::mt19937 seed(rd());

int side() {
    std::uniform_int_distribution<> genSide(1, 4);
    return genSide(seed);
}
sf::Vector2f generatePosition(const int& side) {
    float posX{};
    float posY{};
    if (side == 1) {
        posX = -20;
        std::uniform_int_distribution<> genPosY(-20, 620);
        posY = genPosY(seed);
        return {posX, posY};
    } else if (side == 2) {
        std::uniform_int_distribution<> genPosX(-20, 820);
        posX = genPosX(seed);
        posY = -20;
        return {posX, posY};
    } else if (side == 3) {
        posX = 820;
        std::uniform_int_distribution<> genPosY(-20, 620);
        posY = genPosY(seed);
        return {posX, posY};
    } else {
        std::uniform_int_distribution<> genPosX(-20, 820);
        posX = genPosX(seed);
        posY = 620;
        return {posX, posY};
    }
}
std::string generateScoreText(const int& score) {
    return "Score: " + std::to_string(score);
}
void fireShot(sf::CircleShape& bullet, sf::CircleShape& player, sf::Sound*& shot) {
    bullet.setPosition(400, 300);
    int convRot = player.getRotation();
    float a{};
    float b{};
    if ((convRot >= 0) && (convRot < 90)) {
        a = -2 * std::sin(convRot*3.14/180);
        b = std::sqrt((std::pow(2, 2)) - (std::pow(a, 2)));
    } else if ((convRot >= 90) && (convRot < 180)) {
        a = -2 * std::sin(convRot*3.14/180);
        b = std::sqrt((std::pow(2, 2)) - (std::pow(a, 2)))* -1;
    } else if ((convRot >= 180) && (convRot < 270)) {
        a = -2 * std::sin(convRot*3.14/180);
        b = std::sqrt((std::pow(2, 2)) - (std::pow(a, 2)))* -1;
    } else {
        a = -2 * std::sin(convRot*3.14/180);
        b = std::sqrt((std::pow(2, 2)) - (std::pow(a, 2)));
    }
    shot->play();
    sf::Vector2f fire(a, b);
    for (int i = 0; i <= 320; i++) {
        bullet.move(sf::Vector2f(0, 0) - fire);
        std::this_thread::sleep_for(std::chrono::microseconds(700));
    }
    bullet.setPosition(1000, 1000);
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
