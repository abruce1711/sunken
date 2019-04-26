#include "scene_game.hh"

#include <iostream>

#include <renderer.hh>
#include <level_loader.hh>
#include <engine.hh>

#include "../sunken.hh"
#include "../components/cmp_camera.hh"
#include "../components/cmp_combat_enemy.hh"
#include "../components/cmp_combat_player.hh"
#include "../components/cmp_health_enemy.hh"
#include "../components/cmp_health_player.hh"
#include "../components/cmp_movement_battleship.hh"
#include "../components/cmp_movement_player.hh"
#include "../components/cmp_movement_submarine.hh"
#include "../components/cmp_music.hh"
#include "../components/cmp_pickup_ammo.hh"
#include "../components/cmp_pickup_health.hh"

#include "../components/cmp_health_player.hh"
#include "../components/cmp_health_enemy.hh"
#include "../components/cmp_movement_battleship.hh"
#include "../components/cmp_combat_battleship.hh"

#include "../components/cmp_shape.hh"
#include "../components/cmp_sonar.hh"
#include "../components/cmp_sound.hh"
#include "../components/cmp_text.hh"


void SceneGame::load()
{
	level::load("res/levels/example.txt", 64.0f);

	// Player
	{
		const auto tiles    = level::find_tiles(level::Tile::Player);
		const auto position = level::tile_position(tiles[rand() % tiles.size()]);
		const auto size     = sf::Vector2f(level::tile_size(), level::tile_size()) / 8.0f;

		auto p = make_entity();
		p->move_to(position);
		p->add_tag("player");

		auto s = p->add_component<CmpShape>();
		s->use_shape<sf::RectangleShape>(size);
		s->shape().setOrigin(size / 2.0f);
		s->shape().setFillColor(sf::Color::Yellow);

		auto c = p->add_component<CmpCamera>();
		c->zoom = level::tile_size() * 4.0f;

		p->add_component<CmpCombatPlayer>();
		p->add_component<CmpMovementPlayer>();
		p->add_component<CmpHealthPlayer>();
	}

	// Enemy submarines
	{
		for (const auto tile : level::find_tiles(level::Tile::Submarine))
		{
			const auto position = level::tile_position(tile);
			const auto size     = sf::Vector2f(level::tile_size(), level::tile_size()) / 8.0f;

			auto e = make_entity();
			e->move_to(position);
			e->add_tag("submarine");

			auto s = e->add_component<CmpShape>();
			s->use_shape<sf::RectangleShape>(size);
			s->shape().setOrigin(size / 2.0f);
			s->shape().setFillColor(sf::Color::Blue);

			e->add_component<CmpMovementSubmarine>();
			e->add_component<CmpCombatEnemy>();
			e->add_component<CmpHealthEnemy>();
			e->add_tag("enemy");
		}
	}

	// Enemy battleships
	{
		for (const auto tile : level::find_tiles(level::Tile::Battleship))
		{
			const auto position = level::tile_position(tile);
			const auto size = sf::Vector2f(level::tile_size() / 2.0f, level::tile_size() / 8.0f);

			level::Tile t = level::tile_at(position);

			auto e = make_entity();
			e->move_to(position + sf::Vector2f(0.f, level::tile_size() - size.y) / 2.0f);

			auto s = e->add_component<CmpShape>();
			s->use_shape<sf::RectangleShape>(size);
			s->shape().setOrigin(size / 2.0f);
			s->shape().setFillColor(sf::Color::Blue);

			e->add_component<CmpMovementBattleship>();
			e->add_component<CmpCombatBattleship>();
			e->add_component<CmpHealthEnemy>();
			e->add_tag("enemy");
			e->add_tag("battleship");
		}
	}

	// Objective

	{
		for (const auto tile : level::find_tiles(level::Tile::Objective))
		{
			const auto position = level::tile_position(tile);
			const auto size = sf::Vector2f(35.f, 35.f);

			auto e = make_entity();
			e->move_to(position);
			e->add_tag("objective");

			auto s = e->add_component<CmpShape>();
			s->use_shape<sf::RectangleShape>(size);
			s->shape().setFillColor(sf::Color::Red);

			e->add_component<CmpCombatEnemy>();
			e->add_component<CmpHealthEnemy>();
			e->add_tag("enemy");
		}
	}

	// Pickups
	{
		for (const auto t : level::find_tiles(level::Tile::Ammo))
		{
			sf::Vector2f position = level::tile_position(t);

			auto e = make_entity();
			e->move_to(position);
			e->add_tag("pickup");

			e->add_component<CmpPickupAmmo>();
		}

		for (const auto t : level::find_tiles(level::Tile::Health))
		{
			sf::Vector2f position = level::tile_position(t);

			auto e = make_entity();
			e->move_to(position);
			e->add_tag("pickup");

			e->add_component<CmpPickupHealth>();
		}
	}

	// HUD
	{
		auto e = make_entity();
		e->add_tag("hud");

		auto t = e->add_component<CmpText>();
		t->text.setFillColor(sf::Color::White);
		t->text.setOutlineColor(sf::Color::Black);
		t->text.setOutlineThickness(2.0f);
	}

	// Victory
	{
		auto e = make_entity();
		e->add_tag("victory");

		auto t = e->add_component<CmpText>("Victory");
		t->text.setOrigin(t->text.getLocalBounds().width, t->text.getLocalBounds().height);
		t->text.setFillColor(sf::Color::White);
		t->text.setOutlineColor(sf::Color::Black);
		t->text.setOutlineThickness(2.0f);

		e->visible(false);
	}

	// Music
	{
		auto music = make_entity();
		music->add_tag("music");

		auto m = music->add_component<CmpMusic>("unseen-horrors.ogg");
		if (m->loaded())
		{
			m->music()->setVolume(25);
			m->music()->play();
		}
		else
			music->delete_please();
	}

	// Sonar entity
	{
		auto e = make_entity();
		e->add_tag("sonar");

		auto s = e->add_component<CmpShape>();
		s->use_shape<sf::CircleShape>(0.0f, 64);
		s->shape().setFillColor(sf::Color::Transparent);
		s->shape().setOutlineThickness(1.0f);

		auto p = e->add_component<CmpSound>("sonar.ogg");
		p->sound().setVolume(50);

		e->add_component<CmpSonar>();
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	loaded(true);
}

void SceneGame::unload()
{
	level::unload();
	Scene::unload();
}



// Logic
void SceneGame::update(const float& delta_time)
{
	if (engine::keyboard[sf::Keyboard::Escape])
		return engine::change_scene(&scene_menu);

	// HUD
	{
		auto hud = entities_.find("hud").front();
		auto player = entities_.find("player").front();
		if (hud && player)
		{
			auto text = hud->compatible_components<CmpText>().front();
			auto health = player->compatible_components<CmpHealthPlayer>().front();
			auto ammo = player->compatible_components<CmpCombatPlayer>().front();
			if (text && ammo && health)
			{
				std::string output = "Health: ";
				output += to_decimal_place(0, health->health());
				output += "\tAmmo: ";
				output += std::to_string(ammo->ammo());
				text->text.setString(output);

				auto window = engine::window();
				auto view = window->getView();
				auto top_left = view.getCenter() - view.getSize() / 2.0f;

				text->text.setScale(view.getSize() / window->getSize());
				hud->move_to(top_left);
			}
		}
	}

	// Victory
	{
		auto v = entities_.find("victory").front();
		if (v && entities_.find("objective").empty())
		{
			for (auto& e : entities_.find("enemy"))
				e->delete_please();

			auto player = entities_.find("player").front();
			if (player)
			{
				static float pause = 2.5f;
				pause -= delta_time;
				if (pause < 0.0f)
					return engine::change_scene(&scene_menu);

				auto window = engine::window();
				auto view = window->getView();
				
				v->visible(true);
				v->move_to(view.getCenter());

				auto text = v->compatible_components<CmpText>().front();
				if (text)
					text->text.setScale(view.getSize() / window->getSize());
			}
		}
	}

	Scene::update(delta_time);
}

void SceneGame::render()
{
	renderer::target(nullptr);

	level::render();
	entities_.render();

	renderer::render();
	renderer::draw();
}
