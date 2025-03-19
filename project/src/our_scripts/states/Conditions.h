#include <functional>
#include <string>
#include <unordered_map>
#include <iostream>

#include "../components/Health.h"
#include "../../sdlutils/SdlUtils.h"
#include "../components/movement/Transform.h"

class ConditionManager {
private:
    struct Timers {
        uint32_t duration;
        uint32_t last_used;
    };

    std::unordered_map<std::string, Timers> timers; //temporizadores de los estados, cuanto duran
public:

    // Métodos para evaluar condiciones
    bool is_player_near(Transform* _player, Transform* _enemy, float _dist) const {
		if (!_player || !_enemy) return false;
        float distance = (_player->getPos() - _enemy->getPos()).magnitude();
        return distance < _dist;
    }

    bool is_enemy_dead(Health* _enemy) const {
        return _enemy->getHealth() <= 0;
    }

    void set_cooldown(const std::string& state, uint32_t cooldown) {
        timers[state].duration = cooldown;
        std::cout << state<<"   " << timers[state].duration << std::endl;
        timers[state].last_used = 0;
    }
    
    // Comprobar si se puede usar la accion
    bool can_use(const std::string& action, uint32_t currentTime) {

        auto it = timers.find(action);
        if (it == timers.end()) {
            std::cout << "aaa" << std::endl;
            return true; // Si no tiene cooldown, siempre se puede usar
        }
        return (currentTime - timers[action].last_used) > timers[action].duration;
    }

    // Reiniciar cooldown cuando se usa la accion
    void reset_cooldown(const std::string& action, uint32_t currentTime) {
        timers[action].last_used = currentTime;
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