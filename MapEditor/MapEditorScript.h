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

    char sand;      // terrainNum = 1
    char grass;     // terrainNum = 2
    char ground;    // terrainNum = 3
public:
    MapEditorScript(GameObject* gameObject) : Behaviour(gameObject),
        map(""), mapSize(0), terrainNum(0), sand(' '), grass(' '), ground(' ')
    {
        mapSize = renderer->getCapacity();
        
        sand = '\xB0';
        grass = '\xB1';
        ground = '\xB2';

        for (int i = 0; i < mapSize; i++)
        {
            map.append("0");
        }
    }

    void update() override
    {
        setMap();
    }

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
                renderer->setShape(sand, i);
            }
            else if (map[i] == '2')
            {
                renderer->setShape(grass, i);
            }
            else if (map[i] == '3')
            {
                renderer->setShape(ground, i);
            }
        }
    }

    void setTerrain(int terrainNum)
    {
        this->terrainNum = terrainNum;
    }

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

    void setBlank(const Position& pos)
    {
        int offset = renderer->pos2Offset(pos - transform->getPos());

        map[offset] = '0';
    }

    void save() {
        ofstream mapFile;

        mapFile.open("map.txt", ofstream::trunc);

        for (int i = 0; i < mapSize; i++)
        {
            mapFile << map;
        }
        mapFile.close();
    }

    string getMap()
    {
        return map;
    } 
    
    string getEmptyMap()
    {
        for (int i = 0; i < mapSize; i++)
        {
            map[i] = '0';
        } 

        return map;
    }
};

