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
void control(sf::RenderWindow& window, sf::CircleShape& player, sf::Event& event);
void fire(sf::RenderWindow& window, sf::CircleShape& player, sf::Event& event, 
            sf::CircleShape& bullet);
void asteroidMove(sf::RenderWindow& window, sf::CircleShape& player, 
                    std::array<sf::CircleShape, 4>& asteroid, sf::CircleShape& bullet, 
                    sf::Text& scoreText);