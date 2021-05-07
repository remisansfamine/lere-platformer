#pragma once

#include "collider.hpp"
#include "sphere.h"

namespace Physics
{
	class SphereCollider : public Collider
	{
	private:
		SphereCollider(Engine::GameObject& gameObject, std::shared_ptr<SphereCollider> ptr);
	public:
		SphereCollider(Engine::GameObject& gameObject);

		void updateShape() override;
		void drawImGui() override;

		Sphere sphere;
	};
}