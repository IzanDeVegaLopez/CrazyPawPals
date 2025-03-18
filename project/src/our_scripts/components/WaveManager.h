#pragma once

#include "../../ecs/Component.h"
#include "../../sdlutils/SDLUtils.h"

class WaveManager : public ecs::Component {
    // _waves es un vector de pares (int, vector<int>)
    // Los 0 son espacios extra
    std::vector<std::pair<int, std::vector<int>>> _waves = {
        { 10000, {4, 4, 4, 4,4} },
        { 30000, {6} },
        { 15000,{5, 5, 5, 5, 5} },
        { 15000,{
            2, 2, 2, 2, 2, 0, 0, 0,
            1, 1, 3, 0, 0, 
            3, 3 }
        },
        { 15000,{
            3, 2, 2, 0,
            4, 3, 0,
            4, 2, 1, 0, 0,
            4, 4 }
        },
        { 15000,{
            4, 4, 4, 0,
            4, 4, 4, 4, 0, 0, 0,
            4, 4, 4, 1, 1, 1, 1 }
        }
    };
public:
    __CMPID_DECL__(ecs::cmp::WAVEMANAGER)
    WaveManager();
    virtual ~WaveManager();

    void update(uint32_t delta_time) override;
    void spawnWave();
    bool areAllEnemiesDead();
    void activateFog();
    void enterRewardsMenu();

private:
    Uint32 _currentWaveTime = 0; //tiempo actual (post calculo, inicial en constructor)
    Uint32 _waveTime; // cuánto dura la oleada (CONSTRUCTOR)

    int _currentWave = 0;

    bool _waveActive = false;
    bool _fogActive = false;

    int _numEnemies; // enemigos total en la oleada (post calculo)
    int _enemiesSpawned; // número de enemigos spawneados (post calculo)
	int _enemiesKilled; // número de enemigos muertos (post calculo)

    float _totalSpawnTime; // tiempo spawn total al prinicipio de la oleada (post calculo)
    float _nextSpawn; // cuándo se spawnea el siguiente enemigo (post calculo)

    // tiempo de spawn del siguiente enemigo (post calculo)
    float _min_time;
    float _op_time;
};