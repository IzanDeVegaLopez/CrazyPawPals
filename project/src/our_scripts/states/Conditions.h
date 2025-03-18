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

    // Comprobar si se puede usar la accion
    bool can_use(const std::string& action, uint32_t currentTime) {
        //std::cout << cooldowns.begin()->first << std::endl;

       /* for (auto e : cooldowns) {
            std::cout << e.first;
        }*/
        auto it = cooldowns.find(action);
        std::cout << it->first << std::endl;
        if (it == cooldowns.end()) {
            std::cout << "aaa" << std::endl;
            return true; // Si no tiene cooldown, siempre se puede usar
        }
        return (currentTime - last_used[action]) > cooldowns[action];
    }

    // Reiniciar cooldown cuando se usa la accion
    void reset_cooldown(const std::string& action, uint32_t currentTime) {
        last_used[action] = currentTime;
    }

    std::string chooseRandomPattern(uint32_t currentTime, const std::vector<std::string>& patterns) {
        std::vector<std::string> availablePatterns;
        for (const auto& pattern : patterns) {
            if (can_use(pattern, currentTime)) { // si no esta en cooldown
                availablePatterns.push_back(pattern);
            }
        }

        if (!availablePatterns.empty()) {
            int index =  sdlutils().rand().nextInt(0, availablePatterns.size()) % availablePatterns.size();
            return availablePatterns[index];
        }

        return "";  // Si todos estan en cooldown, devuelve vacío
    }
};