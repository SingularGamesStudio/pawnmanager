//
// Created by goodm on 16.03.2023.
//

#ifndef PAWNMANAGER_PAWNMANAGERCLIENT_H
#define PAWNMANAGER_PAWNMANAGERCLIENT_H

#include "SFML/Graphics/RenderWindow.hpp"
#include "GameWindowManager.h"
#include "PawnRenderer.h"
#include "../Player.h"
#include "BuildingRenderer.h"
#include "ResourceRenderer.h"

class PawnManagerClient {
private:
    sf::RenderWindow window;
    sf::View view;
    PawnRenderer pawnRenderer;
    BuildingRenderer buildingRenderer;
    ResourceRenderer resourceRenderer;
    Player* player;
    double curTime = 0;
    int selectedBuilding;
public:
    static constexpr float renderScale = 1.5f;
    GameWindowManager winManager;
    PawnManagerClient();
    void run();
    ~PawnManagerClient();
private:
    void updateAndRender();
    void buildingRenderDfs(Building* b, sf::Vector2f center);
    void onMouseClick(int x, int y);
    bool onBuildingMouseClick(Building* b, sf::Vector2f pos);
};


#endif //PAWNMANAGER_PAWNMANAGERCLIENT_H
