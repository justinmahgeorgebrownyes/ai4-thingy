#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include <windows.h>

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"
#include <fstream>
#include "InputType.h"

PlayScene::PlayScene()
{
	PlayScene::Start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::Draw()
{
	DrawDisplayList();

	if(m_isGridEnabled)
	{
		for (const auto obstacle : m_pObstacles)
		{
			Util::DrawRect(obstacle->GetTransform()->position - glm::vec2(obstacle->GetWidth() * 0.5f,
				obstacle->GetHeight() * 0.5f), obstacle->GetWidth(), obstacle->GetHeight());
		}
		//7.3
	//	const auto detected = m_pStarShip->GetTree()->GetPlayerDetectedNode()->GetDetected();
	//	Util::DrawCircle(m_pStarShip->GetTransform()->position, 300.0f, detected ? glm::vec4(0, 1, 0, 1) : glm::vec4(1, 0, 0, 1));
	}

	SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 255, 255, 255, 255);
}

void PlayScene::Update()
{
	UpdateDisplayList();
	//m_checkAgentLOS(m_pStarShip, m_pTarget);
	//m_pStarShip->GetTree()->GetLOSNode()->SetLOS(m_pStarShip->checkAgentLOSTOTarget(m_pTarget, m_pObstacles));

	//setup rang combat
	if (deadSir == false) {
		m_pStarShip->GetTree()->GetEnemyHealthNode()->SetHealth(m_pStarShip->GetHealth() > 25);
		m_pStarShip->GetTree()->GetEnemyHitNode()->SetIsHit(false);
		m_pStarShip->checkAgentLOSTOTarget(m_pTarget, m_pObstacles);


		float distance = Util::Distance(m_pStarShip->GetTransform()->position, m_pTarget->GetTransform()->position);


		m_pStarShip->GetTree()->GetPlayerDetectedNode()->SetDetected(distance < 300);


		m_pStarShip->GetTree()->GetRangedCombatNode()->SetIsWithinCombatRange(distance >= 200 && distance <= 350);

	}

	
	//setup rang combat
	m_pStarShip2->GetTree()->GetEnemyHealthNode()->SetHealth(m_pStarShip2->GetHealth() > 25);
	m_pStarShip2->GetTree()->GetEnemyHitNode()->SetIsHit(false);
	m_pStarShip2->checkAgentLOSTOTarget(m_pTarget, m_pObstacles);

	//distance check
	float distance2 = Util::Distance(m_pStarShip2->GetTransform()->position, m_pTarget->GetTransform()->position);

	//radius detection
	m_pStarShip2->GetTree()->GetPlayerDetectedNode()->SetDetected(distance2 < 300);


	//within los distance
	m_pStarShip2->GetTree()->GetCloseCombatNode()->SetIsWithinCombatRange(distance2 >= 200 && distance2 <= 350);

	switch(m_LOSMode)
	{
	case LOSMode::TARGET:
		m_checkAllNodesWithTarget(m_pTarget);
		break;
	case LOSMode::SHIP:
		if (deadSir == false)
		m_checkAllNodesWithTarget(m_pStarShip);

		m_checkAllNodesWithTarget(m_pStarShip2);
		break;
	case LOSMode::BOTH:
		m_checkAllNodesWithBoth();
		break;
	}

	// Make a Decision
//	m_decisionTree->MakeDecision();


	//collison check between tarpeodok and target
	for (auto torpedo : m_pTorpedoesK) {

		CollisionManager::CircleAABBCheck(torpedo, m_pTarget);
//		CollisionManager::CircleAABBCheck(m_pTorpedoes, m_pStarShip);


	}
	for (auto torpedo : m_pTorpedoes) {
		
		if (CollisionManager::CircleAABBCheck(torpedo, m_pStarShip)) {

		//	m_pStarShip = nullptr;
			//deadSir = true;
		//	RemoveChild(m_pStarShip);
			//this->RemoveChild(m_pStarShip);
		}
		CollisionManager::CircleAABBCheck(torpedo, m_pStarShip2);
		//		CollisionManager::CircleAABBCheck(m_pTorpedoes, m_pStarShip);


	}

}

void PlayScene::Clean()
{
	RemoveAllChildren();
}

void PlayScene::HandleEvents()
{
	EventManager::Instance().Update();

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		Game::Instance().Quit();
	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_1))
	{
		Game::Instance().ChangeSceneState(SceneState::START);
	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_2))
	{
		Game::Instance().ChangeSceneState(SceneState::END);
	}	
	
	if (EventManager::Instance().KeyPressed(SDL_SCANCODE_F))
	{
		//torpedo will fie
		m_pTorpedoes.push_back(new Torpedo(5.0f));
		m_pTarget->SetCurrentDirection(glm::vec2(1.0f, 0.0f));
		m_pTorpedoes.back()->GetTransform()->position = m_pTarget->GetTransform()->position + m_pTarget->GetCurrentDirection() * 30.0f;
		SoundManager::Instance().SetSoundVolume(50);
		SoundManager::Instance().PlaySoundFX("torpedo");
		AddChild(m_pTorpedoes.back(), 2);

		//Game::Instance().ChangeSceneState(SceneState::END);
	}

	if (EventManager::Instance().KeyPressed(SDL_SCANCODE_K))
	{
		m_pStarShip->TakeDamage(10);
		m_pStarShip->GetTree()->GetEnemyHitNode()->SetIsHit(true);
		std::cout << "Starship at: " << m_pStarShip->GetHealth() << "% " << std::endl;
		//Game::Instance().ChangeSceneState(SceneState::END);
	}

	if (EventManager::Instance().KeyPressed(SDL_SCANCODE_R))
	{
		m_pStarShip->SetHealth(100);
		m_pStarShip->GetTree()->GetEnemyHitNode()->SetIsHit(false);
		m_pStarShip->GetTree()->GetPlayerDetectedNode()->SetDetected(false);
		m_pStarShip->GetTransform()->position = glm::vec2(40.0f, 40.0f);
		std::cout << "Conditions Reset" << std::endl;

		//Game::Instance().ChangeSceneState(SceneState::END);
	}
	
	GetPlayerInput();

	GetKeyboardInput();

	//if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_W)) {
	//	//m_pStarShip->GetTransform()->position.y += 1;
	//	m_pTarget->GetTransform()->position = m_pTarget->GetTransform()->position + glm::vec2(0.0f, -5.0f);
	//}
	//
	//if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_A)) {
	//	//m_pStarShip->GetTransform()->position.y += 1;
	//	m_pTarget->SetAnimationState(PlayerAnimationState::PLAYER_RUN_LEFT);
	//	m_playerFacingRight = false;
	//	m_pTarget->GetTransform()->position = m_pTarget->GetTransform()->position + glm::vec2(-5.0f, 0.0f);
	//}
	//
	//if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_S)) {
	//	//m_pStarShip->GetTransform()->position.y += 1;
	//	m_pTarget->GetTransform()->position = m_pTarget->GetTransform()->position + glm::vec2(0.0f, 5.0f);
	//}
	//
	//if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_D)) {
	//	//m_pStarShip->GetTransform()->position.y += 1;
	//	m_pTarget->SetAnimationState(PlayerAnimationState::PLAYER_RUN_RIGHT);
	//	m_playerFacingRight = true;
	//	m_pTarget->GetTransform()->position = m_pTarget->GetTransform()->position + glm::vec2(5.0f, 0.0f);
	//}



}

void PlayScene::Start()
{
	// Set GUI Title
	m_guiTitle = "Lab 8";

	m_pCurrentInputType = static_cast<int>(InputType::KEYBOARD_MOUSE);


	// Setup a few more fields
	m_LOSMode = LOSMode::TARGET;
	m_pathNodeLOSDistance = 1000; // 1000px distance
	m_setPathNodeLOSDistance(m_pathNodeLOSDistance);

	// Add Game Objects
	m_pBackground = new Background();
	AddChild(m_pBackground, 0);

	const SDL_Color blue = { 0, 0, 255, 255 };
	m_pInstructionsLabel = new Label("Enemies Remaining", "Consolas", 40, blue, glm::vec2(400.0f, 120.0f));
	m_pInstructionsLabel->SetParent(this);
	AddChild(m_pInstructionsLabel);

	m_pTarget = new Target();
	m_pTarget->GetTransform()->position = glm::vec2(500.0f, 300.0f);
	AddChild(m_pTarget, 2);

	//m_pStarShip = new CloseCombatEnemy();
	m_pStarShip2 = new CloseCombatEnemy();
	m_pStarShip2->GetTransform()->position = glm::vec2(100.0f, 40.0f);
	AddChild(m_pStarShip2, 2);


	m_pStarShip = new RangedCombatEnemy(this);
	m_pStarShip->GetTransform()->position = glm::vec2(400.0f, 40.0f);
	AddChild(m_pStarShip, 2);

	// Add Obstacles
	BuildObstaclePool();

	// Setup the Grid
	m_isGridEnabled = false;
	m_buildGrid();
	m_toggleGrid(m_isGridEnabled);

	// Create Decision Tree
	//m_decisionTree = new DecisionTree(m_pStarShip); // using our overloaded constructor
	//m_decisionTree->Display(); // optional step
	//m_decisionTree->MakeDecision(); // default = patrol

	// Preload Sounds

	SoundManager::Instance().Load("../Assets/Audio/yay.ogg", "yay", SoundType::SOUND_SFX);
	SoundManager::Instance().Load("../Assets/Audio/thunder.ogg", "thunder", SoundType::SOUND_SFX);
	SoundManager::Instance().Load("../Assets/Audio/torpedo.ogg", "torpedo", SoundType::SOUND_SFX);
	SoundManager::Instance().Load("../Assets/Audio/torpedo_k.ogg", "torpedo_k", SoundType::SOUND_SFX);

	// Preload Music
	SoundManager::Instance().Load("../Assets/Audio/Mutara.mp3", "mutara", SoundType::SOUND_MUSIC);
	SoundManager::Instance().Load("../Assets/Audio/Klingon.mp3", "klingon", SoundType::SOUND_MUSIC);
	SoundManager::Instance().SetMusicVolume(16);

	// Play Music
	SoundManager::Instance().PlayMusic("klingon");

	ImGuiWindowFrame::Instance().SetGuiFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::SpawnEnemyTorpedo()
{
	//set spam point front of our d7
	glm::vec2 spawn_point = m_pStarShip->GetTransform()->position + m_pStarShip->GetCurrentDirection() * 30.0f;
	glm::vec2 spawn_point2 = m_pStarShip2->GetTransform()->position + m_pStarShip2->GetCurrentDirection() * 30.0f;

	//set the direction of the torpedo
	glm::vec2 torpedo_direction = Util::Normalize(m_pTarget->GetTransform()->position - spawn_point);
	glm::vec2 torpedo_direction2 = Util::Normalize(m_pTarget->GetTransform()->position - spawn_point2);

	//spawn the torpedo
	m_pTorpedoesK.push_back(new TorpedoK(5.0f, torpedo_direction));
	m_pTorpedoesK.back()->GetTransform()->position = spawn_point;
	m_pTorpedoesK.back()->GetTransform()->position = spawn_point2;
	SoundManager::Instance().SetSoundVolume(50);
	SoundManager::Instance().PlaySoundFX("torpedo_k");




}

Target* PlayScene::GetTarget() const
{
	return m_pTarget;
}

void PlayScene::GUI_Function()
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("GAME3001 - W2023 - Lab 6.2", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar );

	ImGui::Separator();

	// Debug Properties
	if(ImGui::Checkbox("Toggle Grid", &m_isGridEnabled))
	{
		m_toggleGrid(m_isGridEnabled);
	}

	ImGui::Separator();

	static int LOS_mode = static_cast<int>(m_LOSMode);
	ImGui::Text("Path Node LOS");
	ImGui::RadioButton("Target", &LOS_mode, static_cast<int>(LOSMode::TARGET)); ImGui::SameLine();
	ImGui::RadioButton("StarShip", &LOS_mode, static_cast<int>(LOSMode::SHIP)); ImGui::SameLine();
	ImGui::RadioButton("Both Target & StarShip", &LOS_mode, static_cast<int>(LOSMode::BOTH));

	m_LOSMode = static_cast<LOSMode>(LOS_mode);

	ImGui::Separator();

	if(ImGui::SliderInt("Path Node LOS Distance", &m_pathNodeLOSDistance, 0, 1000))
	{
		m_setPathNodeLOSDistance(m_pathNodeLOSDistance);
	}

	ImGui::Separator();

	// StarShip Properties
	static int shipPosition[] = { static_cast<int>(m_pStarShip->GetTransform()->position.x),
		static_cast<int>(m_pStarShip->GetTransform()->position.y) };
	if(ImGui::SliderInt2("Ship Position", shipPosition, 0, 800))
	{
		m_pStarShip->GetTransform()->position.x = static_cast<float>(shipPosition[0]);
		m_pStarShip->GetTransform()->position.y = static_cast<float>(shipPosition[1]);
	}

	// allow the ship to rotate
	static int angle;
	if(ImGui::SliderInt("Ship Direction", &angle, -360, 360))
	{
		m_pStarShip->SetCurrentHeading(static_cast<float>(angle));
	}

	ImGui::Separator();

	// Target Properties
	static int targetPosition[] = { static_cast<int>(m_pTarget->GetTransform()->position.x),
		static_cast<int>(m_pTarget->GetTransform()->position.y) };
	if (ImGui::SliderInt2("Target Position", targetPosition, 0, 800))
	{
		m_pTarget->GetTransform()->position.x = static_cast<float>(targetPosition[0]);
		m_pTarget->GetTransform()->position.y = static_cast<float>(targetPosition[1]);
	}

	ImGui::Separator();

	// Add Obstacle position control for each obstacle
	for(unsigned i = 0; i < m_pObstacles.size(); ++i)
	{
		int obstaclePosition[] = { static_cast<int>(m_pObstacles[i]->GetTransform()->position.x),
		static_cast<int>(m_pObstacles[i]->GetTransform()->position.y) };
		std::string label = "Obstacle " + std::to_string(i + 1) + " Position";
		if(ImGui::SliderInt2(label.c_str(), obstaclePosition, 0, 800))
		{
			m_pObstacles[i]->GetTransform()->position.x = static_cast<float>(obstaclePosition[0]);
			m_pObstacles[i]->GetTransform()->position.y = static_cast<float>(obstaclePosition[1]);
			m_buildGrid();
		}
	}


	ImGui::End();
}

void PlayScene::BuildObstaclePool()
{
	std::ifstream inFile("../Assets/data/obstacles.txt");

	while(!inFile.eof())
	{
		std::cout << "Obstacle " << std::endl;
		auto obstacle = new Obstacle();
		float x, y, w, h; // declare variables the same as how the file is organized
		inFile >> x >> y >> w >> h; // read data from file line by line
		obstacle->GetTransform()->position = glm::vec2(x, y);
		obstacle->SetWidth(static_cast<int>(w));
		obstacle->SetHeight(static_cast<int>(h));
		AddChild(obstacle, 0);
		m_pObstacles.push_back(obstacle);
	}
	inFile.close();
}

void PlayScene::m_buildGrid()
{
	constexpr auto tile_size = Config::TILE_SIZE;
	constexpr auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);

	m_clearNodes(); // we will need to clear nodes because we will rebuild/redraw the grid if we move an obstacle

	// lay out a grid of path_nodes
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			auto path_node = new PathNode();
			path_node->GetTransform()->position = glm::vec2(static_cast<float>(col) * tile_size + offset.x,
				static_cast<float>(row) * tile_size + offset.y);

			// only show grid where there are no obstacles
			bool keep_node = true;
			for (const auto obstacle : m_pObstacles)
			{
				// add the Obstacle Buffer TODO: this can be improved
				const auto buffer = new Obstacle();
				buffer->GetTransform()->position = obstacle->GetTransform()->position;
				buffer->SetWidth(obstacle->GetWidth() + 40);
				buffer->SetHeight(obstacle->GetHeight() + 40);

				// determine which path_nodes to keep
				if(CollisionManager::AABBCheck(path_node, buffer))
				{
					keep_node = false;
				}
			}
			if(keep_node)
			{
				AddChild(path_node);
				m_pGrid.push_back(path_node);
			}
			else
			{
				delete path_node;
			}
		}

		// if Grid is supposed to be hidden - make it so!
		m_toggleGrid(m_isGridEnabled);
	}
}

void PlayScene::m_toggleGrid(const bool state) const
{
	for (const auto path_node : m_pGrid)
	{
		path_node->SetVisible(state);
	}
}

void PlayScene::m_clearNodes()
{
	m_pGrid.clear();
	for (const auto display_object : GetDisplayList())
	{
		if(display_object->GetType() == GameObjectType::PATH_NODE)
		{
			RemoveChild(display_object);
		}
	}
}

bool PlayScene::m_checkAgentLOS(Agent* agent, DisplayObject* target_object) const
{
	bool has_LOS = false; // default - No LOS
	agent->SetHasLOS(has_LOS);

	// if ship to target distance is less than or equal to the LOS Distance (Range)
	const auto agent_to_range = Util::GetClosestEdge(agent->GetTransform()->position, target_object);
	if (agent_to_range <= agent->GetLOSDistance())
	{
		// we are in within LOS Distance 
		std::vector<DisplayObject*> contact_list;
		for (auto display_object : GetDisplayList())
		{
			const auto agent_to_object_distance = Util::GetClosestEdge(agent->GetTransform()->position, display_object);
			if (agent_to_object_distance > agent_to_range) { continue;  } // target is out of range
			if((display_object->GetType() != GameObjectType::AGENT)
				&& (display_object->GetType() != GameObjectType::PATH_NODE)
				&& (display_object->GetType() != GameObjectType::TARGET))
			{
				contact_list.push_back(display_object);
			}
		}

		const glm::vec2 agent_LOS_endPoint = agent->GetTransform()->position + agent->GetCurrentDirection() * agent->GetLOSDistance();
		has_LOS = CollisionManager::LOSCheck(agent, agent_LOS_endPoint, contact_list, target_object);

		const auto LOSColour = (target_object->GetType() == GameObjectType::AGENT) ? glm::vec4(0, 0, 1, 1) : glm::vec4(0, 1, 0, 1);
		agent->SetHasLOS(has_LOS, LOSColour);
	}
	return has_LOS;
}

bool PlayScene::m_checkPathNodeLOS(PathNode* path_node, DisplayObject* target_object) const
{
	// check angle to the target so we can still use LOS Distance for path_nodes
	const auto target_direction = target_object->GetTransform()->position - path_node->GetTransform()->position;
	const auto normalized_direction = Util::Normalize(target_direction); // changes direction vector to a unit vector (magnitude of 1)
	path_node->SetCurrentDirection(normalized_direction);
	return m_checkAgentLOS(path_node, target_object);
}

void PlayScene::m_checkAllNodesWithTarget(DisplayObject* target_object) const
{
	for (const auto path_node : m_pGrid)
	{
		m_checkPathNodeLOS(path_node, target_object);
	}
}

void PlayScene::m_checkAllNodesWithBoth() const
{
	for (const auto path_node : m_pGrid)
	{
		const bool LOSWithStarShip = m_checkPathNodeLOS(path_node, m_pStarShip);
		const bool LOSWithTarget = m_checkPathNodeLOS(path_node, m_pTarget);
		path_node->SetHasLOS(LOSWithStarShip && LOSWithTarget, glm::vec4(0, 1, 1, 1));
	}
}

void PlayScene::m_setPathNodeLOSDistance(const int distance) const
{
	for (const auto path_node : m_pGrid)
	{
		path_node->SetLOSDistance(static_cast<float>(distance));
	}
}

void PlayScene::GetPlayerInput()
{
	switch (m_pCurrentInputType)
	{
	case static_cast<int>(InputType::GAME_CONTROLLER):
	{
		// handle player movement with GameController
		if (SDL_NumJoysticks() > 0)
		{
			if (EventManager::Instance().GetGameController(0) != nullptr)
			{
				constexpr auto dead_zone = 10000;
				if (EventManager::Instance().GetGameController(0)->STICK_LEFT_HORIZONTAL > dead_zone)
				{
					m_pTarget->SetAnimationState(PlayerAnimationState::PLAYER_RUN_RIGHT);
					m_playerFacingRight = true;
				}
				else if (EventManager::Instance().GetGameController(0)->STICK_LEFT_HORIZONTAL < -dead_zone)
				{
					m_pTarget->SetAnimationState(PlayerAnimationState::PLAYER_RUN_LEFT);
					m_playerFacingRight = false;
				}
				else
				{
					if (m_playerFacingRight)
					{
						m_pTarget->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_RIGHT);
					}
					else
					{
						m_pTarget->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_LEFT);
					}
				}
			}
		}
	}
	break;
	case static_cast<int>(InputType::KEYBOARD_MOUSE):
	{
		// handle player movement with mouse and keyboard
		if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_A))
		{
			m_pTarget->SetAnimationState(PlayerAnimationState::PLAYER_RUN_LEFT);
			m_playerFacingRight = false;


			m_pTarget->GetTransform()->position = m_pTarget->GetTransform()->position + glm::vec2(-5.0f, 0.0f);


		}
		else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_D))
		{
			m_pTarget->SetAnimationState(PlayerAnimationState::PLAYER_RUN_RIGHT);
			m_playerFacingRight = true;


			m_pTarget->GetTransform()->position = m_pTarget->GetTransform()->position + glm::vec2(5.0f, 0.0f);



		}
		else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_W))
		{
			//m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_RIGHT);
			//m_playerFacingRight = true;


			m_pTarget->GetTransform()->position = m_pTarget->GetTransform()->position + glm::vec2(0.0f, -5.0f);



		}
		else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_S))
		{
			//m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_RIGHT);
			//m_playerFacingRight = true;


			m_pTarget->GetTransform()->position = m_pTarget->GetTransform()->position + glm::vec2(0.0f, 5.0f);



		}

		else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_H))
		{
			//m_pPlayer->SetAnimationState(PlayerAnimationState::PLAYER_RUN_RIGHT);
			//m_playerFacingRight = true;
			if (m_isGridEnabled == true) {
				m_isGridEnabled = false;
			}
			else if (m_isGridEnabled == false) {
				m_isGridEnabled = true;
			}
			m_toggleGrid(m_isGridEnabled);

			//m_pPlayer->GetTransform()->position = m_pPlayer->GetTransform()->position + glm::vec2(0.0f, 5.0f);



		}

		else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_K))
		{


		}

		else
		{
			if (m_playerFacingRight)
			{
				m_pTarget->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_RIGHT);
			}
			else
			{
				m_pTarget->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_LEFT);
			}
		}
	}
	break;
	case static_cast<int>(InputType::ALL):
	{
		if (SDL_NumJoysticks() > 0)
		{
			if (EventManager::Instance().GetGameController(0) != nullptr)
			{
				constexpr auto dead_zone = 10000;
				if (EventManager::Instance().GetGameController(0)->STICK_LEFT_HORIZONTAL > dead_zone
					|| EventManager::Instance().IsKeyDown(SDL_SCANCODE_D))
				{
					m_pTarget->SetAnimationState(PlayerAnimationState::PLAYER_RUN_RIGHT);
					m_playerFacingRight = true;
				}
				else if (EventManager::Instance().GetGameController(0)->STICK_LEFT_HORIZONTAL < -dead_zone
					|| EventManager::Instance().IsKeyDown(SDL_SCANCODE_A))
				{
					m_pTarget->SetAnimationState(PlayerAnimationState::PLAYER_RUN_LEFT);
					m_playerFacingRight = false;
				}
				else
				{
					if (m_playerFacingRight)
					{
						m_pTarget->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_RIGHT);
					}
					else
					{
						m_pTarget->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_LEFT);
					}
				}
			}
		}
		else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_A))
		{
			m_pTarget->SetAnimationState(PlayerAnimationState::PLAYER_RUN_LEFT);
			m_playerFacingRight = false;
		}
		else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_D))
		{
			m_pTarget->SetAnimationState(PlayerAnimationState::PLAYER_RUN_RIGHT);
			m_playerFacingRight = true;
		}
		else
		{
			if (m_playerFacingRight)
			{
				m_pTarget->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_RIGHT);
			}
			else
			{
				m_pTarget->SetAnimationState(PlayerAnimationState::PLAYER_IDLE_LEFT);
			}
		}
	}
	break;
	}
}

void PlayScene::GetKeyboardInput()
{
	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		Game::Instance().Quit();
	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_1))
	{
		Game::Instance().ChangeSceneState(SceneState::START);
	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_2))
	{
		Game::Instance().ChangeSceneState(SceneState::PLAY);
	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_3))
	{
		Game::Instance().ChangeSceneState(SceneState::NODE);
	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_4))
	{
		Game::Instance().ChangeSceneState(SceneState::END);
	}
	//if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_H)) {

	//}
}
