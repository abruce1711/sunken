#include "cmp_camera.hh"

#include <iostream>

#include <level_loader.hh>
#include <engine.hh>

// Class overrides
CmpCamera::CmpCamera(Entity* const p)
: Component(p),
  buffer_  (p->position()),
  easing_  (4.0f),
  zoom     (engine::window_size().x)
{}

CmpCamera::~CmpCamera()
{
	// Make sure window goes back to default view
	engine::window()->setView(engine::window()->getDefaultView());
}

// Logic
void CmpCamera::update(const float& delta_time)
{
	// Interpolate between current buffer position and parent position
	const sf::Vector2f vector = parent_->position() - buffer_;
	buffer_ += vector * easing_ * delta_time;

	const auto zoom_delta = engine::mouse_wheel_delta * level::tile_size();
	zoom -= zoom_delta;

	if (zoom < zoom_delta)
		zoom = zoom_delta;
	if (level::loaded() && zoom > level::tile_size() * 8.0f)
		zoom = level::tile_size() * 8.0f;
}

void CmpCamera::render()
{
	sf::Vector2f size = sf::Vector2f(zoom, zoom);

	// Make view fill screen
	auto window = engine::window();
	auto w_size = sf::cast<float>(window->getSize());
	if (w_size.x < w_size.y)
		size.x *= w_size.x / w_size.y;
	else
		size.y *= w_size.y / w_size.x;

	// Set view variables and use it
	view_.setCenter(buffer_);
	view_.setSize(size);
	window->setView(view_);
}
