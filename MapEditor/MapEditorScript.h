#pragma once
#include "Behaviour.h"
#include "Renderer.h"
#include "Transform.h"
#include <fstream>

class MapEditorScript :
    public Behaviour
{
private:
    string map;
    
    int mapSize;
    int terrainNum;

    char grass;      // terrainNum = 1
    char sand;     // terrainNum = 2
    char rock;    // terrainNum = 3
public:
    MapEditorScript(GameObject* gameObject) : Behaviour(gameObject),
        map(""), mapSize(0), terrainNum(0), grass(' '), sand(' '), rock(' ')
    {
        // ∏  ≈©±‚
        mapSize = renderer->getCapacity();
        
        // terrain √ ±‚»≠
        grass = '\xB0';
        sand = '\xB1';
        rock = '\xB2';

        // ∏  √ ±‚»≠
        for (int i = 0; i < mapSize; i++)
        {
            map.append("0");
        }
    }

    void update() override
    {
        setMap();
    }

    // ∏ ¿« terrainNumø° µ˚∂Û terrain √‚∑¬
    void setMap()
    {
        for (int i = 0; i < mapSize; i++)
        {
            if (map[i] == '0')
            {
                renderer->setShape(' ', i);
            }
            else if (map[i] == '1')
            {
                renderer->setShape(grass, i);
            }
            else if (map[i] == '2')
            {
                renderer->setShape(sand, i);
            }
            else if (map[i] == '3')
            {
                renderer->setShape(rock, i);
            }
        }
    }

    // terrainNum ºº∆√
    void setTerrain(int terrainNum)
    {
        this->terrainNum = terrainNum;
    }

    // terrainNumø° µ˚∂Û terrain ∫Ø∞Ê
    void editMap(const Position& pos)
    {
        int offset = renderer->pos2Offset(pos - transform->getPos());

        if (terrainNum == 0)
        {
            map[offset] = '0';
        }
        else if (terrainNum == 1)
        {
            map[offset] = '1';
        }
        else if (terrainNum == 2)
        {
            map[offset] = '2';
        }
        else if (terrainNum == 3)
        {
            map[offset] = '3';
        }
    }

    // ∫Ûƒ≠¿∏∑Œ ºº∆√
    void setBlank(const Position& pos)
    {
        int offset = renderer->pos2Offset(pos - transform->getPos());

        map[offset] = '0';
    }

    // ∏  √ ±‚»≠
    void clear()
    {
        for (int i = 0; i < mapSize; i++)
        {
            map[i] = '0';
        }
    }

    // ∏  ¿˙¿Â
    void save() {
        ofstream mapFile;

        mapFile.open("map.txt", ofstream::trunc);
        mapFile << map;
        mapFile.close();
        
        exit(0);
    }

    // ∏  ∫“∑Øø¿±‚
    void load() {
        ifstream mapFile;
        mapFile.open("map.txt");
        clear();
        if (mapFile.is_open() == false) {
            return;
        }
        if (!mapFile.eof()) {
            mapFile >> map;
        }
        mapFile.close();
    }
};

