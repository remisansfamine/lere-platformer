#include "box_collider.hpp"

#include "imgui.h"

#include "physic_manager.hpp"
#include "render_manager.hpp"

#include "collider_renderer.hpp"
#include "utils.hpp"

namespace Physics
{
	BoxCollider::BoxCollider(Engine::GameObject& gameObject, std::shared_ptr<BoxCollider> ptr)
		: Collider(gameObject, ptr)
	{
		PhysicManager::linkComponent(ptr);
		gameObject.addComponent<LowRenderer::ColliderRenderer>(ptr, "resources/obj/colliders/boxCollider.obj");
		LowRenderer::RenderManager::linkComponent(gameObject.getComponent<LowRenderer::ColliderRenderer>());
	}

	BoxCollider::BoxCollider(Engine::GameObject& gameObject)
		: BoxCollider(gameObject, std::shared_ptr<BoxCollider>(this))
	{
	}

	void BoxCollider::updateShape()
	{
		Core::Maths::mat4 globalModel = m_transform->getGlobalModel();
		m_center = Core::Maths::modelMatrixToPosition(globalModel) + box.center;
		extensions = Core::Maths::modelMatrixToScale(globalModel) * box.size;
	}

	void BoxCollider::drawImGui()
	{
		if (ImGui::TreeNode("Box Collider"))
		{
			ImGui::DragFloat3("Center :", &box.center.x);
			ImGui::DragFloat3("Size :", &box.size.x);
			ImGui::Checkbox("IsTrigger", &isTrigger);
			ImGui::Checkbox("IsDraw", &isDraw);
			//ImGui::DragFloat3("Position offset :", &m_positionOffset.x);

			ImGui::TreePop();
		}
	}

	std::string BoxCollider::toString() const
	{
		return "COMP BOXCOLLIDER " + Utils::vecToStringParsing(box.center) + 
									 Utils::vecToStringParsing(box.size) +
									 Utils::quatToStringParsing(box.quaternion) +
									 std::to_string(box.offsetRounding);
	}

	void BoxCollider::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		gameObject.addComponent<BoxCollider>();
		auto collider = gameObject.getComponent<BoxCollider>();

		iss >> collider->box.center.x;
		iss >> collider->box.center.y;
		iss >> collider->box.center.z;

		iss >> collider->box.size.x;
		iss >> collider->box.size.y;
		iss >> collider->box.size.z;

		iss >> collider->box.quaternion.x;
		iss >> collider->box.quaternion.y;
		iss >> collider->box.quaternion.z;
		iss >> collider->box.quaternion.w;

		iss >> collider->box.offsetRounding;
	}
}