#include <vector>
#include <random>
#include "WaveManager.h"
#include "EnemyPoolManager.h"
#include "ecs/Manager.h"
#include "game/Game.h"
#include "sdlutils/SDLUtils.h"

WaveManager::WaveManager() : 
    _totalSpawnTime(25000.f),
    _waveActive(false), _fogActive(false),
    _currentWave(0), _enemiesSpawned(0)
{
    _waveTime = 60000 - _totalSpawnTime;
}

WaveManager::~WaveManager() {

}

void 
WaveManager::update(uint32_t delta_time) {
    Uint32 currentTime = SDL_GetTicks();

    if (_waveActive) {
        // Verificar si ha pasado un minuto desde el �ltimo spawn
        if (SDL_GetTicks() > _endWaveTime) {
            std::cout << "Se acabo el tiempo"<< std::endl;
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
    std::random_device rd;
    std::mt19937 gen(rd());
	_numEnemies = _waves[_currentWave].size();
    if (_enemiesSpawned < _numEnemies) {

        // RANDOM
        std::random_device rd;
        std::mt19937 gen(rd());
        // rAng entre (0, 360)
        std::uniform_real_distribution<float> rAngGen(0.0f, 360.0f);
        // rn entre (-0.35, 0.35)
        std::uniform_real_distribution<float> rnGen(-0.35f, 0.35f);

        // DEBUG
		//std::cout << SDL_GetTicks() << "/" << _nextSpawn << std::endl;
        
        if (SDL_GetTicks() > _nextSpawn){
            float rAng = rAngGen(gen); // (0, 360)
            float rn = rnGen(gen); // (-0.35, 0.35)
            
            // Distancia
            float _min_distance = sdlutils().width() + sdlutils().width() * 0.1;
            float _op_dist = _min_distance * (-rn);

            // Medio de la pantalla + angulo * distancia
            Vector2D posVec = Vector2D(sdlutils().width() / 2 + cos(rAng) * (_min_distance + _op_dist), sdlutils().height() / 2 + sin(rAng) * (_min_distance + _op_dist));
			
            switch (_waves[_currentWave][_enemiesSpawned])
            {
			case 1:
				//Game::Instance()->createSarnoRata(posVec);
				break;
            case 2:
				//Game::Instance()->createMichiMafioso(posVec);
				break;
			case 3:
				//Game::Instance()->createSarnoRata(posVec); // plim plim
                break;
            case 4:
				//Game::Instance()->createMichiMafioso(posVec); // boom
				break;
            default:
				std::cout << "Enemigo no existe" << std::endl;
                break;
            }
            std::cout << "Enemy " << _waves[_currentWave][_enemiesSpawned] << " spawned at: " << posVec.getX() << ", " << posVec.getY() << " with " << rAng << "º + " << (_min_distance + _op_dist) << "m, rn = " << rn << std::endl;
			std::cout << std::endl;
            _enemiesSpawned++;

            // Tiempo
            _min_time = _totalSpawnTime / _numEnemies;
            _op_time = _min_time * rn;
            _nextSpawn = SDL_GetTicks() + (_min_time + _op_time);
        }
    }
    else {
        _waveActive = true; // después de que se spawnee el último enemigo
        _endWaveTime = SDL_GetTicks() + _totalSpawnTime; // se calcula cuándo va a terminar la oleada

        std::cout << "WAVE ACTIVE" << std::endl;
    }
}

//Verifica si todos los enemigos estan muertos
bool 
WaveManager::areAllEnemiesDead() {
    return true;
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

