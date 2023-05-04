//
// Created by goodm on 18.03.2023.
//

#ifndef PAWNMANAGER_BUILDINGRENDERER_H
#define PAWNMANAGER_BUILDINGRENDERER_H

#include "../IDmanager.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "../Entities/Buildings/BuildingRegisty.h"

class Building;

class BuildingRenderer {
    sf::RenderWindow& window;

public:
    explicit BuildingRenderer(sf::RenderWindow& window);
    void drawBuilding(ptr<Building> b, sf::Vector2f pos);
    void drawBuilding(float radius, sf::Color color, sf::Vector2f pos);
    void drawBuilding(BuildingIdea& idea, sf::Vector2f pos);
    void drawEdge(ptr<Building> a, ptr<Building> b, sf::Vector2f center);
};


#endif//PAWNMANAGER_BUILDINGRENDERER_H
