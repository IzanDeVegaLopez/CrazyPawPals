#pragma once

#include "../ecs/Component.h"
#include "../sdlUtils/SDLUtils.h"

class WaveManager : public ecs::Component {

    std::vector<std::vector<int>> _waves;
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

    Uint32 currentTime; //tiempo actual (post calculo)
    Uint32 _waveTime; // cuánto dura la oleada (CONSTRUCTOR)
    Uint32 _endWaveTime; // cuándo termina la oleada (post calculo)

    int _currentWave = 0;

    bool _waveActive = false;
    bool _fogActive = false;

    int _numEnemies; // enemigos total en la oleada (CONSTRUCTOR)
    int _enemiesSpawned; // número de enemigos spawneados (post calculo)

    float _totalSpawnTime; // tiempo spawn total al prinicipio de la oleada (CONSTRUCTOR)
    float _nextSpawn; // cuándo se spawnea el siguiente enemigo (post calculo)

    // tiempo de spawn del siguiente enemigo (post calculo)
    float _min_time;
    float _op_time;
};