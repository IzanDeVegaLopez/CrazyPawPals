#pragma once
#include "../../../ecs/Component.h"
#include "../../../ecs/Component.h"

class Transform;
class Follow : public ecs::Component
{

public:
	__CMPID_DECL__(ecs::cmp::FOLLOW);

	Follow(GameStructs::EnemyFollow type = GameStructs::EnemyFollow::CLOSEST);
	~Follow() {};

	void initComponent() override;
	void act_follow();
	inline Transform* get_act_follow() const{
		return _act_follow;
	};
private:
	void search_closest_player();
	void search_furthest_player();
	void search_lowest_life_player();
	void search_highest_life_player();

	Transform *_my_tr;
	Transform *_act_follow;
	GameStructs::EnemyFollow _my_follow_type;
};