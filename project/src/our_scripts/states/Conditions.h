#include <functional>
#include <string>
#include <unordered_map>
#include <iostream>

#include "../components/Health.h"
#include "../../sdlutils/SdlUtils.h"
#include "../components/Transform.h"

class ConditionManager {
private:
    std::unordered_map<std::string, uint32_t> last_used;  // Última vez que se uso el estado
    std::unordered_map<std::string, uint32_t> cooldowns; 
    std::unordered_map<std::string, uint32_t> timers; //temporizadores de los estados
    std::unordered_map<std::string, uint32_t> timerDurations;

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

    void set_cooldown(const std::string& state, uint32_t cooldown) {
        cooldowns[state] = cooldown;
        std::cout << state<<"   " << cooldowns[state] << std::endl;
        last_used[state] = cooldown;
    }
    
    // Comprobar si se puede usar la accion
    bool can_use(const std::string& action, uint32_t currentTime) {

        auto it = cooldowns.find(action);
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

    void start_timer(const std::string& action, uint32_t startTime, uint32_t duration) {
        timers[action] = startTime;
        timerDurations[action] = duration;
    }

    bool is_timer_ends(const std::string& action, uint32_t currentTime) {
        auto it = timers.find(action);
        if (it != timers.end()) {
            uint32_t duration = timerDurations[action];
            return (currentTime - it->second) >= duration;
        }
        return false;
    }
};