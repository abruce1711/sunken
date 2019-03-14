#pragma once

#include <memory>
#include <vector>

#include <SFML/System.hpp>

// Forward declare
struct Component;

struct Entity {
	Entity();
	virtual ~Entity();

	// Logic
	virtual void update(const float& delta_time);
	virtual void render();

	// Position
	sf::Vector2f position() const;
	void move_to(const sf::Vector2f& position);
	void move_by(const sf::Vector2f& movement);

	// Rotation
	float rotation() const;
	void rotate_to(const float& rotation);
	void rotate_by(const float& rotation);

	// Scale
	sf::Vector2f scale() const;
	void scale_to(const sf::Vector2f& scale);
	void scale_by(const sf::Vector2f& scale);

	// Visibility
	bool is_visible() const;
	void visible(const bool& visible);

	// Life
	bool is_alive() const;
	void alive(const bool& alive);

	// Deletion
	bool is_for_deletion() const;
	void delete_please();

	// Components
	template <typename T, typename... Targs>
	T* const add_component(Targs... params)
	{
		static_assert(
			std::is_base_of<Component, T>::value,
			"T != Component");

		std::unique_ptr<T> component(std::make_unique<T>(this, params...));
		components_.push_back(std::move(component));
		return (T*)components_.back().get();
	}

protected:
	sf::Vector2f position_;
	float rotation_;
	sf::Vector2f scale_;

	bool alive_;
	bool visible_;
	bool for_deletion_;

	std::vector<std::unique_ptr<Component>> components_;
};

struct Component {
	Component() = delete;
	virtual ~Component();
	
	// Logic
	virtual void update(const float& delta_time) = 0;
	virtual void render() = 0;

	// Deletion
	bool is_for_deletion() const;

protected:
	explicit Component(Entity* const p);

	bool for_deletion_;
	Entity* const parent_;
};

