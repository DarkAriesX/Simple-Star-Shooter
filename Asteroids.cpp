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