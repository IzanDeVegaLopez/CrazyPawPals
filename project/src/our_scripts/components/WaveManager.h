#pragma once

#include "../../ecs/Component.h"
#include "../../sdlutils/SDLUtils.h"

enum enemyType {
    none = 0,
    sarno_rata = 1,
    michi_mafioso = 2,
    plim_plim = 3,
    boom = 4,
    ratatouille = 5
};

class WaveManager : public ecs::Component {
    // _waves es un vector de pares (int, vector<int>)
    // Los 0 son espacios extra
    std::vector<std::pair<Uint32, std::vector<enemyType>>> _waves = {
        { 10000,{
            sarno_rata, none,
            sarno_rata, sarno_rata, none,
            sarno_rata, michi_mafioso, none, none,
            michi_mafioso, sarno_rata, sarno_rata, none, none,
            michi_mafioso, michi_mafioso }
        },
        { 15000,{
            sarno_rata, michi_mafioso, none,
            michi_mafioso, plim_plim, none,
            sarno_rata, sarno_rata, plim_plim, none, none,
            michi_mafioso, plim_plim, plim_plim }
        },
        { 15000,{
            michi_mafioso, michi_mafioso, michi_mafioso, michi_mafioso, michi_mafioso, none, none, none,
            sarno_rata, sarno_rata, plim_plim, none, none,
            plim_plim, plim_plim }
        },
        { 15000,{
            plim_plim, michi_mafioso, michi_mafioso, none,
            boom, plim_plim, none,
            boom, michi_mafioso, sarno_rata, none, none,
            boom, boom }
        },
        { 15000,{
            boom, boom, boom, none,
            boom, boom, boom, boom, none, none, none,
            boom, boom, boom, sarno_rata, sarno_rata, sarno_rata, sarno_rata }
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