#include "SuperMichiMafiosoAttack.h"

#include "../../game/GameScene.h"
#include "../components/Health.h"
#include "../components/Transform.h"
#include "../components/WeaponSuperMichiMafioso.h"
#include "../components/MovementController.h"

SuperMichiMafiosoAttack::SuperMichiMafiosoAttack(Transform* tr, Transform* playerTr, WeaponSuperMichiMafioso* weapon, MovementController* mc) :
    _tr(tr), _playerTr(playerTr), _weapon(weapon), _mc(mc), 
	_timer(0), _attackCount(0), _currentPattern(PATTERN_NONE)
{}

void SuperMichiMafiosoAttack::enter() {
	_attackCount = 0;
	_timer = 0;
	_weapon->setAttackPattern(WeaponSuperMichiMafioso::ATTACK1);
	_currentPattern=PATTERN1;
	
}

void SuperMichiMafiosoAttack::update(uint32_t delta_time) {
	if (!_tr || !_playerTr || !_weapon || !_mc) return;
	
	Vector2D _target = _playerTr->getPos();
    float distanceToPlayer = (_playerTr->getPos() - _tr->getPos()).magnitude();

	Vector2D direction = (_target - _tr->getPos()).normalize();

   // _mc->set_input(direction);
    _timer += delta_time;
    if (_timer >= 2000) {
       //_weapon->setAttackPattern(WeaponSuperMichiMafioso::SPAWN_MICHI_MAFIOSO);
       // _weapon->shoot(_target);
        _timer = 0;
    }

    switch (_currentPattern) {
    case PATTERN1:
        if (_weapon->shoot(_target)) _attackCount++;

        if (_attackCount >= 3) {
            _currentPattern = PATTERN2;
            _weapon->setAttackPattern(WeaponSuperMichiMafioso::ATTACK2);
            _attackCount = 0;
        }
        break;

    case PATTERN2:
        if (_weapon->shoot(_target)) _attackCount++;

        if (distanceToPlayer >= 3.0f)_mc->set_input(direction);
        else {
            _mc->set_input({ 0,0 });
            if (_attackCount >= 1) {
                _currentPattern = PATTERN3;
                _weapon->setAttackPattern(WeaponSuperMichiMafioso::ATTACK3);
                _attackCount = 0;
            }
        }
        break;

    case PATTERN3:
        if (_weapon->shoot(_target)) _attackCount++;
        if(_attackCount>=1)_currentPattern = PATTERN1;
        break;

    default:
        break;
    }
}

void SuperMichiMafiosoAttack::exit() {
	_attackCount = 0;
	_timer = 0;
}
