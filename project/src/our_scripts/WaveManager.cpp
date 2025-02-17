//#include <vector>
//#include "WaveManager.h"
//#include "EnemyPoolManager.h"
//#include "ecs/Manager.h"
//#include "sdlutils/SDLUtils.h"
//
//WaveManager::WaveManager(ecs::Manager* mngr, EnemyPoolManager* enemyPool)
//    : _mngr(mngr), _enemyPool(enemyPool), _lastSpawnTime(0), _spawnInterval(60000), _waveActive(false), _fogActive(false) {}
//
//
//void 
//WaveManager::update() {
//    Uint32 currentTime = SDL_GetTicks();
//
//    if (_waveActive) {
//        // Verificar si ha pasado un minuto desde el último spawn
//        if (currentTime - _lastSpawnTime >= _spawnInterval) {
//            if (!areAllEnemiesDead()) {
//                activateFog(); // Activar la niebla si no se han eliminado todos los enemigos
//            }
//            else {
//                enterRewardsMenu(); // Ir al menú de recompensas si se eliminaron todos los enemigos
//            }
//            _waveActive = false; // Finalizar la oleada
//        }
//    }
//    else {
//        // Iniciar una nueva oleada
//        spawnWave();
//        _waveActive = true;
//        _lastSpawnTime = currentTime;
//    }
//}
//
//// Spawnear enemigos fuera de la pantalla
//void 
//WaveManager::spawnWave() {
//    for (int i = 0; i < 10; ++i) { // 10 por ahora
//        float x = -100.0f; // Fuera de la pantalla a la izquierda
//        float y = sdlutils().rand().nextInt(0, sdlutils().height()); // Posición Y aleatoria
//        //_enemyPool->addEnemies(x, y);
//    }
//}
//
////Verifica si todos los enemigos estan muertos
//bool 
//WaveManager::areAllEnemiesDead() {
//    auto activeEnemies = _enemyPool->getActiveObjects();
//    return activeEnemies.empty();
//}
//
////Activa la niebla
//void 
//WaveManager::activateFog() {
//    _fogActive = true;
//    std::cout << "Niebla activada!" << std::endl;
//}
//
//
//void 
//WaveManager::enterRewardsMenu() {
//    std::cout << "Todos los enemigos eliminados. Entrando al menú de recompensas..." << std::endl;
//}
//
