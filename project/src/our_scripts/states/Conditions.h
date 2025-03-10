#include <functional>
#include <string>

#include "../components/Health.h"
#include "../components/Transform.h"

class ConditionManager {
public:
    // Métodos para evaluar condiciones
    bool isPlayerNear(Transform* _player, Transform* _enemy, float _dist) const {
		if (!_player || !_enemy) return false;
        float distance = (_player->getPos() - _enemy->getPos()).magnitude();
        return distance < _dist;
    }

    bool is_enemy_dead(Health* _enemy) const {
        return _enemy->getHealth() <= 0;
    }

    
};