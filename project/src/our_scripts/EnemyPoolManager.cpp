
//// This file is part of the course TPV2@UCM - Samir Genaim
//
//#include "EnemyPoolManager.h"
//
//#include "../sdlutils/InputHandler.h"
//#include "../ecs/Manager.h"
//#include "../ecs/ecs.h"
//#include "../sdlutils/SDLUtils.h"
//#include "../our_scripts/components/Image.h"
//#include "../our_scripts/Enemy.h"
//
//
//template<typename T>
//EnemyPoolManager::EnemyPoolManager(std::vector<T> _objs) : _pool(_objs.size())
//{
//	
//}
//
//EnemyPoolManager::~EnemyPoolManager() {
//}
//
////
////std::vector<Enemy*> EnemyPoolManager::getActiveObjects() const {
////	return _pool.getActiveObjects();
////}
//
//
//void EnemyPoolManager::update() {
//	
//	for (auto o : _pool) {
//		if (_pool.is_used(o)) {
//			
//			if (false) {
//				_pool.free(o);
//			}
//		}
//	}
//
//	addEnemies(1);
//}
//
//void EnemyPoolManager::render() {
//	for (auto o : _pool) {
//		if (_pool.is_used(o)) {
//			//o->getImage()->render();
//		}
//	}
//}
//
//
//void EnemyPoolManager::addEnemies(uint16_t n) {
//	for (auto i = 0u; i < n; i++) {
//		/*Enemy* e = _pool.alloc();
//		if (e == nullptr)
//			break;*/
//
//		// Inicializar el enemigo con valores aleatorios
//		auto& rand = sdlutils().rand();
//		//const Vector2D& pos, float width, float height, float speed, const std::string& imageName
//		//e->init(
//		//	Vector2D(rand.nextInt(0, sdlutils().width()), // Pos aleatoria en X
//		//		rand.nextInt(0, sdlutils().height())),         // Pos aleatoria en Y
//		//		rand.nextInt(50, 150),
//		//		rand.nextInt(50, 150),
//		//		float(rand.nextInt(1.0f, 3.0f))/100,
//		//		"enemy"
//		//	);
//	}
//}
