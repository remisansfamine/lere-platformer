#include "graph.hpp"

#include "imgui.h"

#include "debug.hpp"
#include "physic_manager.hpp"

namespace Core::Engine
{
	Graph::Graph()
	{
		Core::Debug::Log::info("Creating the Graph");
	}

	Graph::~Graph()
	{
		Core::Debug::Log::info("Destroying the Graph");
	}

	void Graph::loadScene(const std::string& scenePath)
	{
		curScene.load(scenePath);
	}

	void Graph::loadSaveGame()
	{
		instance()->isLoadingSavedScene = true;
	}

	void Graph::loadNewGame()
	{
		instance()->isStartingNewGame = true;
	}

	void Graph::loadMainMenu()
	{
		instance()->isLoadingMainMenu = true;
	}

	void Graph::saveCurrentScene()
	{
		instance()->curScene.save();
	}

	void Graph::init()
	{
		Graph* graph = instance();
	}

	Resources::Scene& Graph::getCurScene()
	{
		return instance()->curScene;
	}

	void Graph::draw()
	{
		// Draw the scene
		instance()->curScene.draw();
	}

	void Graph::update()
	{
		Graph* graph = instance();

		if (graph->isStartingNewGame)
		{
			graph->loadScene("resources/scenes/defaultScene.scn");
			saveCurrentScene();
			graph->isStartingNewGame = false;
		}
		else if (graph->isLoadingSavedScene)
		{
			graph->loadScene("resources/scenes/savedScene.scn");
			graph->isLoadingSavedScene = false;
		}
		else if (graph->isLoadingMainMenu)
		{
			graph->loadScene("resources/scenes/mainMenu.scn");
			graph->isLoadingMainMenu = false;
		}

		// Update the scene
		graph->curScene.update();

		// Update rigidbodies and colliders
		Physics::PhysicManager::update();
	}

	void Graph::fixedUpdate()
	{
		// Fixed update the scene
		instance()->curScene.fixedUpdate();
	}

	void Graph::drawImGui()
	{
		if (ImGui::Begin("Inspector"))
			instance()->curScene.drawImGui();

		ImGui::End();
	}

	::Engine::GameObject* Graph::findGameObjectWithName(const std::string& gameObjectName)
	{
		return instance()->curScene.findGameObjectWithName(gameObjectName);
	}
}