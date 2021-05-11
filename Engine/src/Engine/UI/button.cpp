#include "button.hpp"

#include "inputs_manager.hpp"

#include "intersection.h"
#include "transform.hpp"
#include "rect2D.hpp"
#include "utils.hpp"

namespace UI
{
	Button::Button(Engine::GameObject& gameObject, const std::string& shaderProgramName, const std::string& texturePath)
		: Component(gameObject, std::shared_ptr<Button>(this))
	{
		m_transform = requireComponent<Physics::Transform>();
		m_image = requireComponent<LowRenderer::SpriteRenderer>(shaderProgramName, texturePath);
	}

	void Button::update()
	{
		Physics::Rect2D buttonRect = Physics::Rect2D(m_transform->m_position.xy, m_transform->m_scale.x, m_transform->m_scale.y);	
		Core::Maths::vec2 mousePos = Core::Input::InputManager::getMousePos();

		mousePos.x = Utils::remap(mousePos.x, 0.f, 1440.f, -10.f, 10.f);
		mousePos.y = Utils::remap(mousePos.y, 0.f, 900.f, 10.f, -10.f);

		m_image->m_color = Core::Maths::vec4(1.f, 1.f, 1.f, 1.f);

		if (!Physics::IntersectPointRect(mousePos, buttonRect))
			return;
		
		onHighlight();

		if (Core::Input::InputManager::getMouseButtonDown("LeftClick"))
			onClick();
		if (Core::Input::InputManager::getMouseButton("LeftClick"))
			onClickStay();
		if (Core::Input::InputManager::getMouseButtonUp("LeftClick"))
			onClickRelease();
	}

#pragma region FUNCTIONS_CALL
	void Button::onClick()
	{
		for (auto& function : functions[ButtonState::DOWN])
		{
			function();
		}
	}

	void Button::onClickRelease()
	{
		for (auto& function : functions[ButtonState::UP])
		{
			function();
		}
	}

	void Button::onHighlight()
	{
		for (auto& function : functions[ButtonState::HIGHLIGHT])
		{
			function();
		}
	}

	void Button::onClickStay()
	{
		for (auto& function : functions[ButtonState::STAY])
		{
			function();
		}
	}
#pragma endregion


	void Button::addListener(ButtonState state, std::function<void()> function)
	{
		functions[state].push_back(function);
	}

	std::shared_ptr<LowRenderer::SpriteRenderer> Button::getSprite()
	{
		return m_image;
	}
}