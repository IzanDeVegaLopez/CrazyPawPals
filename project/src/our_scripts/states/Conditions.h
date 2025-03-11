#include <functional>
#include <string>

#include "../components/Health.h"
#include "../../sdlutils/SdlUtils.h"
#include "../components/Transform.h"

class ConditionManager {
    private:
    std::unordered_map<std::string, uint32_t> last_used;  // Última vez que se uso el estado
    std::unordered_map<std::string, uint32_t> cooldowns; 

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

    void setCooldown(const std::string& state, uint32_t cooldown) {
        cooldowns[state] = cooldown;
    }
    
    // Comprobar si se puede usar la accion
    bool canUse(const std::string& action, uint32_t currentTime) {
        if (cooldowns.find(action) == cooldowns.end()) return true; // Si no tiene cooldown, siempre se puede usar
        return (currentTime - last_used[action]) > cooldowns[action];
    }

    // Reiniciar cooldown cuando se usa la accion
    void resetCooldown(const std::string& action, uint32_t currentTime) {
        last_used[action] = currentTime;
    }

    std::string chooseRandomPattern(uint32_t currentTime, const std::vector<std::string>& patterns) {
        std::vector<std::string> availablePatterns;
        for (const auto& pattern : patterns) {
            if (canUse(pattern, currentTime)) { // si no esta en cooldown
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