#pragma once
#include "../ecs/Manager.h"


class EnemyPoolManager;

class WaveManager {
public:
    WaveManager(ecs::Manager* mngr, EnemyPoolManager* enemyPool);

    void update();

    void spawnWave();

    bool areAllEnemiesDead();

    void activateFog();

    void enterRewardsMenu();

private:
    ecs::Manager* _mngr;
    EnemyPoolManager* _enemyPool;

    Uint32 currentTime; //tiempo actual (post calculo)
    Uint32 _waveTime; // cuánto dura la oleada (CONSTRUCTOR)
    Uint32 _endWaveTime; // cuándo termina la oleada (post calculo)


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