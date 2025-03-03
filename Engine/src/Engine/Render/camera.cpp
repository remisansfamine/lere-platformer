#include "camera.hpp"

#include "imgui.h"

#include "render_manager.hpp"
#include "inputs_manager.hpp"
#include "application.hpp"
#include "time.hpp"

#include "transform.hpp"

namespace LowRenderer
{
	Camera::Camera(Engine::GameObject& gameObject)
		: Camera(gameObject, std::shared_ptr<Camera>(this))
	{
		m_transform = requireComponent<Physics::Transform>();
	}

	Camera::Camera(Engine::GameObject& gameObject, const std::shared_ptr<Camera>& ptr)
		: Component(gameObject, ptr)
	{
		LowRenderer::RenderManager::linkComponent(ptr);
	}

	Core::Maths::mat4 Camera::getViewMatrix() const
	{
		auto viewModel = m_transform->getParentModel();
		viewModel.e[3] = -viewModel.e[3];
		viewModel.e[7] = -viewModel.e[7];
		viewModel.e[11] = -viewModel.e[11];

		// Get the camera view matrix
		return Core::Maths::rotateZ(m_transform->m_rotation.z) * Core::Maths::rotateX(m_transform->m_rotation.x) *
			Core::Maths::rotateY(m_transform->m_rotation.y) * Core::Maths::translate(-m_transform->m_position) * viewModel;
	}

	Core::Maths::mat4 Camera::getProjection() const
	{
		// Get the camera projection using the aspect ration, fov, near and far parameters
		return Core::Maths::perspective(fovY * Core::Maths::DEG2RAD, aspect, near, far);
	}

	Core::Maths::mat4 Camera::getOrthographic() const
	{
		// Get the camera orthographic using the aspect ration, fov, near and far parameters
		return Core::Maths::orthographic(-10.f, 10.f, -10.f, 10.f, -1.f, 10.f);
	}

	Core::Maths::mat4 Camera::getViewProjection() const
	{
		return getProjection() * getViewMatrix();
	}

	Core::Maths::mat4 Camera::getViewOrthographic() const
	{
		return getOrthographic(); //* Core::Maths::translate(-Core::Maths::vec3(0.f, 0.f, 1.f));
	}

	bool setCursor(Core::Maths::vec2& deltaMouse)
	{
		static bool isCursorLock = false;

		// Set the cursor visibility
		if (Core::Input::InputManager::getButtonDown("LockCursor"))
		{
			isCursorLock = !isCursorLock;
			Core::Application::setCursor(isCursorLock);
		}

		if (!isCursorLock)
		{
			deltaMouse.x = 0.f;
			deltaMouse.y = 0.f;
		}

		return isCursorLock;
	}

	void Camera::fixedUpdate()
	{
		aspect = Core::Application::getAspect();

		/*float translationSpeed = 2.f * deltaTime;

		float forwardMove = Core::Input::InputManager::getAxis("Vertical");
		float strafeMove = Core::Input::InputManager::getAxis("Horizontal");
		float verticalMove = Core::Input::InputManager::getAxis("UpDown");

		float sin = sinf(m_transform->m_rotation.y), cos = cosf(m_transform->m_rotation.y);

		m_transform->m_position.x += (sin * forwardMove + cos * strafeMove) * translationSpeed;
		m_transform->m_position.z += (sin * strafeMove - cos * forwardMove) * translationSpeed;

		m_transform->m_position.y += verticalMove * translationSpeed;*/
	}

	void Camera::sendViewProjToProgram(const std::shared_ptr<Resources::ShaderProgram> program)
	{
		program->setUniform("viewProj", getViewProjection().e, 1, 1);
		program->setUniform("viewPos", m_transform->m_position.e);
	}

	void Camera::sendViewOrthoToProgram(const std::shared_ptr<Resources::ShaderProgram> program)
	{
		program->setUniform("viewOrtho", getViewOrthographic().e, 1, 1);
	}

	void Camera::sendProjToProgram(const std::shared_ptr<Resources::ShaderProgram> program)
	{
		program->setUniform("viewProj", getViewProjection().e, 1, 1);
	}

	void Camera::drawImGui()
	{
		if (ImGui::TreeNode("Camera"))
		{
			ImGui::DragFloat("Near plane :", &near);
			ImGui::DragFloat("Far plane :", &far);
			ImGui::DragFloat("FOV Y :", &fovY);
			ImGui::TreePop();
		}
	}

	std::string Camera::toString() const
	{
		return "COMP CAMERA " + std::to_string(near) + " " + std::to_string(far) + " " + std::to_string(fovY);
	}

	void Camera::parseComponent(Engine::GameObject& gameObject, std::istringstream& iss)
	{
		gameObject.addComponent<Camera>();
		auto cam = gameObject.getComponent<Camera>();

		iss >> cam->near;
		iss >> cam->far;
		iss >> cam->fovY;
	}
}