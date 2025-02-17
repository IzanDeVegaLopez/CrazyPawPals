#include <vector>
#include "WaveManager.h"
#include "EnemyPoolManager.h"
#include "ecs/Manager.h"
#include "sdlutils/SDLUtils.h"

WaveManager::WaveManager(ecs::Manager* mngr, EnemyPoolManager* enemyPool)
    : _mngr(mngr), _enemyPool(enemyPool),
    _totalSpawnTime(10000.f), _waveTime(60000 - _totalSpawnTime),
    _numEnemies(10), 
    _waveActive(false), _fogActive(false) {}

void 
WaveManager::update() {
    Uint32 currentTime = SDL_GetTicks();

    if (_waveActive) {
        // Verificar si ha pasado un minuto desde el �ltimo spawn
        if (currentTime > _endWaveTime) {
            if (!areAllEnemiesDead()) {
                activateFog(); // Activar la niebla si no se han eliminado todos los enemigos
            }
            else {
                enterRewardsMenu(); // Ir al men� de recompensas si se eliminaron todos los enemigos
            }
            _waveActive = false; // Finalizar la oleada
        }
    }
    else {
        // Iniciar una nueva oleada
        spawnWave();
    }
}

// Spawnear enemigos fuera de la pantalla
void 
WaveManager::spawnWave() {
    srand(time(0)); //hay que poner esto en el principio del programa una sola vez
    
    if (_enemiesSpawned < _numEnemies) {
        if (currentTime > _nextSpawn){
            float rAng = rand() / RAND_MAX * 360; // (0, 360)
            float rn = -0.35 + rand() / RAND_MAX * (0.35 - (-0.35)); // (-0.35, 0.35)
            
            // Distancia
            float _min_distance = sdlutils().width() + sdlutils().width() * 0.1;
            float _op_dist = _min_distance * (-rn);

            // Medio de la pantalla + angulo * distancia
            Vector2D posVec = Vector2D(sdlutils().width() / 2 + cos(rAng) * (_min_distance + _op_dist), sdlutils().height() / 2 + sin(rAng) * (_min_distance + _op_dist));

            // spawnEnemy(posVec); //!!!!!!!!!!
            //_enemyPool->addEnemies(x, y); //!!!!!!!!!!
            _enemiesSpawned++;

            // Tiempo
            _min_time = _totalSpawnTime / _numEnemies;
            _op_time = _min_time * rn;
            _nextSpawn = currentTime + (_min_time + _op_time);
        }
    }
    else _waveActive = true; // después de que se spawnee el último enemigo
    _endWaveTime = currentTime + _totalSpawnTime; // se calcula cuándo va a terminar la oleada
}

//Verifica si todos los enemigos estan muertos
bool 
WaveManager::areAllEnemiesDead() {
    auto activeEnemies = _enemyPool->getActiveObjects();
    return activeEnemies.empty();
}

//Activa la niebla
void 
WaveManager::activateFog() {
    _fogActive = true;
    std::cout << "Niebla activada!" << std::endl;
}


void 
WaveManager::enterRewardsMenu() {
    std::cout << "Todos los enemigos eliminados. Entrando al menu de recompensas..." << std::endl;
}

