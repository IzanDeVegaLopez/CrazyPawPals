#pragma once

#include "../../ecs/Component.h"
#include "../../sdlutils/SDLUtils.h"
#include "../wave_events/wave_event.hpp"

enum enemyType {
    none = 0,
    sarno_rata = 1,
    michi_mafioso = 2,
    plim_plim = 3,
    boom = 4,
    ratatouille = 5,
    catkuza = 6
};

class WaveManager : public ecs::Component {
    // _waves es un vector de pares (int, vector<int>)
    // Los 0 son espacios extra
    std::vector<std::pair<Uint32, std::vector<enemyType>>> _waves = {
        { 10000,{
            catkuza, catkuza}
        },
        { 10000,{
            catkuza, catkuza }
        }
    };
public:
    __CMPID_DECL__(ecs::cmp::WAVEMANAGER)
    WaveManager();
    virtual ~WaveManager() override;

    void update(uint32_t delta_time) override;
    void spawnWave();
    bool areAllEnemiesDead();
    void activateFog();
    void enterRewardsMenu();

private:
    void choose_new_event();
    Uint32 _currentWaveTime = 0; //tiempo actual (post calculo, inicial en constructor)
    Uint32 _waveTime; // cuánto dura la oleada (CONSTRUCTOR)

    int _currentWave = 0;
    std::unique_ptr<wave_event> _current_wave_event;

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