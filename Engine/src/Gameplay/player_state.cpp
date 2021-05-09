#include "player_state.hpp"

#include "imgui.h"

#include "inputs_manager.hpp"

namespace Gameplay
{
	PlayerState::PlayerState(Engine::GameObject& gameObject)
		: Component(gameObject, std::shared_ptr<PlayerState>(this))
	{

	}

	void PlayerState::start()
	{

	}

	void PlayerState::update()
	{
		if (Core::Input::InputManager::getButtonDown("Jump") && isGrounded)
			isJumping = true;

		horizontalMove = Core::Input::InputManager::getAxis("Horizontal");
		forwardMove = Core::Input::InputManager::getAxis("Forward");
	}

	void PlayerState::drawImGui()
	{
		if (ImGui::TreeNode("PlayerState"))
		{
			std::string horizontalStr = "Horizontal movement : " + std::to_string(horizontalMove);
			ImGui::Text(horizontalStr.c_str());
			std::string forwardStr = "Forward movement : " + std::to_string(forwardMove);
			ImGui::Text(forwardStr.c_str());
			ImGui::TreePop();
		}
	}

	void PlayerState::onCollisionEnter(std::shared_ptr<Physics::Collider> other)
	{
		isGrounded = true;
		colliderCount++;
	}

	void PlayerState::onCollisionExit(std::shared_ptr<Physics::Collider> other)
	{
		colliderCount--;

		if (colliderCount == 0)
			isGrounded = false;
	}
}