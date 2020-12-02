#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <chrono>
#include <string>
#include <thread>

int side();
sf::Vector2f generatePosition(const int& side);
std::string generateScoreText(const int& score);
void fireShot(sf::CircleShape& bullet, sf::CircleShape& player, sf::Sound*& shot);