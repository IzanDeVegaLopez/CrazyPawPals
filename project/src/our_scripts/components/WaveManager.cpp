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
#include "../wave_events/star_shower_event.hpp"

// 1 segundo = 1000 ticks (ms)
WaveManager::WaveManager() :
    _currentWaveTime(0),
    _waveTime(5000), //60000 !!
    _currentWave(0),
    _waveActive(false),
    _enemiesSpawned(0),
    _enemiesKilled(0),
    _numEnemies(0),
    _current_wave_event(new no_event(this))
{
    event_system::event_manager::Instance()->suscribe_to_event(event_system::enemy_dead, this, &event_system::event_receiver::event_callback0);
}

WaveManager::~WaveManager() {
    event_system::event_manager::Instance()->unsuscribe_to_event(event_system::enemy_dead, this, &event_system::event_receiver::event_callback0);
}

void
WaveManager::initComponent() {
    //_tdi = Game::Instance()->get_mngr()->getComponent<transformless_dyn_image>(_ent);
    //assert(_tdi != nullptr);
    //TODO: cambiar esto por _ent posiblemente
	fog = Game::Instance()->get_mngr()->getComponent<Fog>(Game::Instance()->get_mngr()->getHandler(ecs::hdlr::FOGGROUP));
    assert(fog != nullptr);

    
    //_currentWaveInitTime = sdlutils().virtualTimer().currRealTime();
    //choose_new_event();
}
/*
void 
WaveManager::update(uint32_t delta_time) {
    _currentWaveTime = sdlutils().virtualTimer().currRealTime() - _currentWaveInitTime;
    

    //if(_current_wave_event != nullptr)
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
*/

bool WaveManager::can_spawn_next_enemy()
{
    return _next_spawn_time > sdlutils().currTime() && tokens_for_this_wave > 0;
}

bool WaveManager::is_wave_finished()
{
    //TODO: Necesitamos no notificar los enemigos que son creados por otros
    //std::cout << "enemies_killed: " << _enemiesKilled << "   numEnemies: " << _numEnemies << "    enemies_spawned: " << _enemiesSpawned << std::endl;
    return _enemiesKilled >= _numEnemies && _all_enemies_already_spawned;
}

//Chooses enemies in _enemy_types_for_current_wave
void WaveManager::initialize_next_wave_params(bool normal_wave)
{
    tokens_for_this_wave = _currentWave * spawn_tokens_gained_per_wave + spawn_tokens_at_wave_0;


    uint8_t cheaper_enemy;
    for (uint8_t i = 0; i < 3; ++i) {
        uint8_t j = 0;
        do {
            j = 0;
            //Chooses new random enemy
            _enemy_types_for_current_wave[i] = sdlutils().rand().nextInt(0, (int)super_michi_mafioso);
        } while (
            j < i && //This is false for (i==0)
            _enemy_types_for_current_wave[j] != _enemy_types_for_current_wave[i] && //This is false if enemy chosen for index 1 || 2 is alredy taken in index 0
            ++j < i && //This is false for (i==1)
            _enemy_types_for_current_wave[j] != _enemy_types_for_current_wave[i]  //This is false if enemy chosen for index 2 is alredy taken in index 1
        );
        cheaper_enemy = std::min(cheaper_enemy,enemy_spawn_data[_enemy_types_for_current_wave[i]].enemies_group_spawn_cost);
    }
    time_max_between_enemy_spawns_on_this_wave = max_spawn_wave_time / (tokens_for_this_wave / cheaper_enemy);
    _next_spawn_time = sdlutils().currTime() + time_max_between_enemy_spawns_on_this_wave;
    //Si no es normal wave spawnea tb un bos
}

void WaveManager::spawn_next_group_of_enemies()
{
    //ONLY ENTERS HERE IF TOKENS LEFT > 0 
    //rest tokens
    uint8_t index = sdlutils().rand().nextInt(0, 3);
    //tokens can only be -1 at worst at end of the round (cause I know that there will always be at least a 2 cost enemy on the group)
    while ((tokens_for_this_wave - enemy_spawn_data[_enemy_types_for_current_wave[index]].enemies_group_spawn_cost) < -1) {
        index = ++index % 3;
        std::cout << (tokens_for_this_wave - enemy_spawn_data[_enemy_types_for_current_wave[index]].enemies_group_spawn_cost) << std::endl;
    }
    tokens_for_this_wave -= enemy_spawn_data[_enemy_types_for_current_wave[index]].enemies_group_spawn_cost;
    //spawn enemies
    enemy_spawn_caller* esc;
    
    switch ((enemyType)_enemy_types_for_current_wave[index])
    {
        case sarno_rata:
            esc = new enemy_spawn_caller([](Vector2D v) {GameScene::spawn_sarno_rata(v); });
            break;
        case michi_mafioso:
            esc = new enemy_spawn_caller([](Vector2D v) {GameScene::spawn_michi_mafioso(v); });
            break;
        case plim_plim:
            esc = new enemy_spawn_caller([](Vector2D v) {GameScene::spawn_plim_plim(v); });
            break;
        case boom:
            esc = new enemy_spawn_caller([](Vector2D v) {GameScene::spawn_boom(v); });
            break;
        case ratatouille:
            esc = new enemy_spawn_caller([](Vector2D v) {GameScene::spawn_ratatouille(v); });
            break;
        case catkuza:
            esc = new enemy_spawn_caller([](Vector2D v) {GameScene::spawn_catkuza(v); });
            break;
        case super_michi_mafioso:
            esc = new enemy_spawn_caller([](Vector2D v) {GameScene::spawn_super_michi_mafioso(v); });
            break;
        case rata_basurera:
            esc = new enemy_spawn_caller([](Vector2D v) {GameScene::spawn_rata_basurera(v); });
            break;
        default: {
            assert(false && "unreachable");
            exit(EXIT_FAILURE);
            break;
        }
    }
    for (uint8_t i = 0; i < enemy_spawn_data[_enemy_types_for_current_wave[index]].number_of_enemies_simultaneous_spawn; ++i) {
        esc->spawn_callback();
    }
    delete esc;
    _numEnemies += enemy_spawn_data[_enemy_types_for_current_wave[index]].number_of_enemies_simultaneous_spawn;
    //sets next spawn time
    _next_spawn_time = sdlutils().currTime() + (uint32_t)(time_max_between_enemy_spawns_on_this_wave * (sdlutils().rand().nextInt(0,100) * 0.3+0.7));

    _all_enemies_already_spawned = tokens_for_this_wave <= 0;
}
//---------------------------------------------------------------------------------------------------------------------------------
void WaveManager::update(uint32_t delta_time) {
    _currentWaveTime = sdlutils().virtualTimer().currRealTime() - _currentWaveInitTime;

    _current_wave_event->update(delta_time);
    //tries spawning enemies
    if (can_spawn_next_enemy())
        spawn_next_group_of_enemies();

    if (is_wave_finished())
        endwave();

    if (_currentWaveTime > 50 * 1000 && !is_wave_finished()) {
        activateFog();
    }

    //spawns fog after 50 secs
    //when all enemies are killed wave finishes
}
//---------------------------------------------------------------------------------------------------------------------------------

//Verifica si todos los enemigos estan muertos
bool 
WaveManager::areAllEnemiesDead() {
    return _enemiesKilled >= _numEnemies;
}

//Activa la niebla
void 
WaveManager::activateFog() {
    fog->setFog(true);
    //std::cout << "Niebla activada!" << std::endl;
}


void 
WaveManager::enterRewardsMenu() {
    Game::Instance()->change_Scene(Game::REWARDSCENE);
}

void WaveManager::start_new_wave()
{
    _currentWaveInitTime = sdlutils().virtualTimer().currRealTime();
    //Si es oleada de boss es true
    initialize_next_wave_params(_currentWave%5==0);

    // Esto tiene que ir después del menu de recompensas
    _currentWaveTime = 0;
    _enemiesSpawned = 0;
    _enemiesKilled = 0;
    _numEnemies = 0;
    fog->setFog(false);

    auto& mngr = *Game::Instance()->get_mngr();
    auto enemies = mngr.getEntities(ecs::grp::ENEMY);
    for (auto e : enemies) {
        mngr.setAlive(e, false);
    }

    choose_new_event();
}

void WaveManager::reset_wave_manager()
{
    _currentWave = 0;
}


void WaveManager::endwave()
{
    //std::cout << "oleada superada con éxito meow" << std::endl;
    _current_wave_event->end_wave_callback();
    _currentWave++;
    _all_enemies_already_spawned = false;
    fog->setFog(false);
    enterRewardsMenu();

}

void WaveManager::event_callback0(const Msg& m)
{
    (void)m;
    _enemiesKilled++;
}

void WaveManager::choose_new_event()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> rnd_gen(NONE,EVENTS_MAX - 1);
    _current_event = events(rnd_gen(gen));
    
    switch(_current_event) {
    case NONE:
        _current_wave_event = (std::unique_ptr<wave_event>)new no_event(this);
        break;
    case ICE_SKATE:
        _current_wave_event = (std::unique_ptr<wave_event>)new ice_skating_event(this);
        break;
    case STAR_SHOWER: {
        constexpr static const rect_f32 event_area = {
            .position = { 0.0f, 0.0f },
            .size = { 32.0f, 16.0f },
        };
        constexpr static const size_t min_drops_inclusive = 5;
        constexpr static const size_t max_drops_exclusive = 23;
        _current_wave_event = std::make_unique<star_shower_event>(
            *this,
            event_area,
            star_drop_descriptor{
                .drop_position = { 0.0f, 0.0f },
                .damage_amount = 3,
                .drop_radius = 0.25f,
                .fall_time = 1.25f,
                .spawn_distance = 16.0f,
            },
            star_drop_descriptor{
                .drop_position = { 0.0f, 0.0f },
                .damage_amount = 24,
                .drop_radius = 2.0f,
                .fall_time = 8.0f,
                .spawn_distance = 32.0f,
            },
            min_drops_inclusive,
            max_drops_exclusive
        );
        break;
    }
    default: {
        assert(false && "unrachable"); // event_choser_went_wrong
        std::exit(EXIT_FAILURE);
    }
    }

    

    _current_wave_event->start_wave_callback();
    //TODO elegir evento y llamar a la función de iniciar
}

void enemy_spawn_caller::spawn_callback()
{
    //Choose random spawn pos
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> rAngGen(0.0f, 360.0f);
    float rAng = rAngGen(gen); // (0, 360)
    Vector2D posVec = Vector2D(cos(rAng) * Game::Instance()->get_world_half_size().first, Game::Instance()->get_world_half_size().second * sin(rAng));
    //spawn
    spawn_call(posVec);
}
