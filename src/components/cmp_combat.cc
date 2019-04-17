#include "cmp_combat.hh"
#include "cmp_torpedo.hh"

#include <iostream>

#include <scene.hh>

CmpCombat::CmpCombat(Entity* p) : Component(p), ammo_(5) {}

// fires missile
void CmpCombat::fire(sf::Vector2f direction)
{
	if (ammo_ == 0)
		return;
	if (fire_cooldown_ > 0.0f)
		return;

	std::cout << "FIRE" << std::endl;


	auto e = parent_->scene->make_entity();
	auto s = e->add_component<CmpShape>();
	auto t = e->add_component<CmpTorpedo>(direction);
	s->use_shape<sf::CircleShape>(12.f);
	s->shape().setFillColor(sf::Color::Red);
	e->move_to(parent_->position());

	--ammo_;
	fire_cooldown_ = 2.0f;


}

// checks ammo
unsigned CmpCombat::ammo()
{
	return ammo_;
}

// adds more ammo upon pickup
void CmpCombat::set_ammo(unsigned pickup)
{
	ammo_ += pickup;
}

void CmpCombat::update(const float& delta_time) {
	if(fire_cooldown_ > 0.0f)
		fire_cooldown_ -= delta_time;
}