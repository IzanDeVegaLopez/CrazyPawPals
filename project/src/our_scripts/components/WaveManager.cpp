#include <vector>
#include <random>
#include "WaveManager.h"
#include "ecs/Manager.h"
#include "game/Game.h"
#include "game/scenes/GameScene.h"
#include "sdlutils/SDLUtils.h"
#include "../components/Fog.h"
#include "../wave_events/no_event.hpp"
#include "../wave_events/ice_skating_event.hpp"

// 1 segundo = 1000 ticks (ms)
WaveManager::WaveManager() :
    _currentWaveTime(0),
    _waveTime(5000),
    _currentWave(0),
    _waveActive(false),
    _enemiesSpawned(0),
    _enemiesKilled(0),
    _totalSpawnTime(10000.0f),
    _current_wave_event(new no_event(this))
{
}

WaveManager::~WaveManager() {

}

void
WaveManager::initComponent() {
    //_tdi = Game::Instance()->get_mngr()->getComponent<transformless_dyn_image>(_ent);
    //assert(_tdi != nullptr);
    //TODO: cambiar esto por _ent posiblemente
	fog = Game::Instance()->get_mngr()->getComponent<Fog>(Game::Instance()->get_mngr()->getHandler(ecs::hdlr::FOGGROUP));
    assert(fog != nullptr);

    //std::cout << sdlutils().virtualTimer().currRealTime() << std::endl;
    //_currentWaveInitTime = sdlutils().virtualTimer().currRealTime();
    //choose_new_event();
}

void 
WaveManager::update(uint32_t delta_time) {
    _currentWaveTime = sdlutils().virtualTimer().currRealTime() - _currentWaveInitTime;
    //std::cout << sdlutils().virtualTimer().currRealTime()<< " / "<< _currentWaveInitTime << std::endl;

    if(_current_wave_event != nullptr)
        _current_wave_event->update(delta_time);

    if (areAllEnemiesDead()) {
        enterRewardsMenu();
    }

    if (_waveActive) {
        // Verificar si ha pasado un minuto de oleada
        // Esto solo pasa una vez por oleada, pero en todas pasa
        if (_currentWaveTime > _waveTime) {
            std::cout << "Se acabo el tiempo"<< std::endl;
            if (!areAllEnemiesDead()) {
                activateFog(); // Activar la niebla si no se han eliminado todos los enemigos
            }
            _waveActive = false; // Finalizar la oleada, (post oleada, matar enemigos restantes, aparece niebla)
        }
    }
    else if (!_waveActive && fog->getFogActive() == false){
        // Iniciar una nueva oleada
        spawnWave();
    }
}

// Spawnear enemigos fuera de la pantalla
void 
WaveManager::spawnWave() {
    std::random_device rd;
    std::mt19937 gen(rd());
	// _waves es un vector de pares (int, vector<int>)
	_numEnemies = _waves[_currentWave].second.size();
    if (_enemiesSpawned < _numEnemies) {

        // RANDOM
        std::random_device rd;
        std::mt19937 gen(rd());
        // rAng entre (0, 360)
        std::uniform_real_distribution<float> rAngGen(0.0f, 360.0f);
        // rn entre (-0.35, 0.35)
        std::uniform_real_distribution<float> rnGen(-0.35f, 0.35f);


        if (_currentWaveTime > _nextSpawn){
            float rAng = rAngGen(gen); // (0, 360)
            float rn = rnGen(gen); // (-0.35, 0.35)
            
            // Distancia
            float _min_distance = Game::Instance()->get_world_half_size().first + Game::Instance()->get_world_half_size().first * 0.1;
            float _op_dist = _min_distance * (-rn);

            //DEBUG
            // Medio de la pantalla + angulo * distancia
            Vector2D posVec = Vector2D(Game::Instance()->get_world_half_size().first + cos(rAng) * (_min_distance + _op_dist), Game::Instance()->get_world_half_size().second + sin(rAng) * (_min_distance + _op_dist));
            
            // FIXME: define enum values
            switch (_waves[_currentWave].second[_enemiesSpawned])
            {
                case none:
                    break;
		        case sarno_rata:
                    static_cast<GameScene*>(Game::Instance()->get_currentScene())->spawn_sarno_rata(posVec);
			        break;
                case michi_mafioso:
                    static_cast<GameScene*>(Game::Instance()->get_currentScene())->spawn_michi_mafioso(posVec);
			        break;
		        case plim_plim:
                    static_cast<GameScene*>(Game::Instance()->get_currentScene())->spawn_plim_plim(posVec);
                    break;
                case boom:
                    static_cast<GameScene*>(Game::Instance()->get_currentScene())->spawn_boom(posVec);
			        break;
                case ratatouille:
                    static_cast<GameScene*>(Game::Instance()->get_currentScene())->spawn_ratatouille(posVec);
                    break;
                case catkuza:
                    static_cast<GameScene*>(Game::Instance()->get_currentScene())->spawn_catkuza(posVec);
                    break;
                case super_michi_mafioso:
                    static_cast<GameScene*>(Game::Instance()->get_currentScene())->spawn_super_michi_mafioso(posVec);
                    break;
                default: {
                    assert(false && "unreachable");
                    exit(EXIT_FAILURE);
                    break;
			        //std::cout << "Enemigo no existe" << std::endl;
                }
            }
            // Tiempo
            _min_time = _totalSpawnTime / _numEnemies;
            _op_time = _min_time * rn;
            _nextSpawn = _currentWaveTime + (_min_time + _op_time);

            _enemiesSpawned++;
        }
    }
    else {
        _waveActive = true; // después de que se spawnee el último enemigo

        //std::cout << "WAVE ACTIVE" << std::endl;
    }
}

//Verifica si todos los enemigos estan muertos
bool 
WaveManager::areAllEnemiesDead() {
    return _enemiesKilled >= _numEnemies;
}

//Activa la niebla
void 
WaveManager::activateFog() {
    fog->setFog(true);
    std::cout << "Niebla activada!" << std::endl;
}


void 
WaveManager::enterRewardsMenu() {
    //std::cout << "Active time: " << sdlutils().virtualTimer().currRealTime() << std::endl;
    //std::cout << "Todos los enemigos eliminados. Entrando al menu de recompensas..." << std::endl;
    _current_wave_event->end_wave_callback();

    choose_new_event();

    // Esto tiene que ir después del menu de recompensas
    _currentWave++;
    _currentWaveTime = 0;
    _totalSpawnTime = _waves[_currentWave].first;
	_enemiesSpawned = 0;
	_enemiesKilled = 0;
    _numEnemies = 0;
    fog->setFog(false);

	for (int i : _waves[_currentWave].second) {
		if (i != 0) _numEnemies++;
	}
}

void WaveManager::show_wave_image()
{
    //_tdi->set_active(true);
}

void WaveManager::hide_wave_image()
{
    //_tdi->set_active(false);
}

void WaveManager::start_new_wave()
{
    _currentWaveInitTime = sdlutils().virtualTimer().currRealTime();
    choose_new_event();
}

void WaveManager::choose_new_event()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> rnd_gen(NONE,EVENTS_MAX);
    _current_event = events(rnd_gen(gen));
    //std::cout << "wave number: " << (i) << std::endl;
    switch(_current_event) {
    case NONE:
        _current_wave_event = (std::unique_ptr<wave_event>)new no_event(this);
        break;
    case ICE_SKATE:
        _current_wave_event = (std::unique_ptr<wave_event>)new ice_skating_event(this);
        break;
    default:
        std::cout << "event_choser_went_wrong" << std::endl;
    }

    //std::cout << i << std::endl;

    _current_wave_event->start_wave_callback();
    //TODO elegir evento y llamar a la función de iniciar
}

