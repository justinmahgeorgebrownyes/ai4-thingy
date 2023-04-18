#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "LOSMode.h"
#include "Obstacle.h"
#include "Scene.h"
#include "Target.h"
#include "StarShip.h"
#include "PathNode.h"

#include "CloseCombatEnemy.h"
#include "RangedCombatEnemy.h"

// New for Lab 7.1
#include "Background.h"
#include "Torpedo.h"
#include "TorpedoK.h"
#include "Label.h"



class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene() override;

	// Scene LifeCycle Functions
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;
	virtual void HandleEvents() override;
	virtual void Start() override;
	RangedCombatEnemy* m_pStarShip;
	CloseCombatEnemy* m_pStarShip2;

	bool m_playerFacingRight{};
	

	bool deadSir = false;

	//lab 8
	void SpawnEnemyTorpedo();
	Target* GetTarget() const;

private:
	Label* m_pStartLabel{};
	Label* m_pInstructionsLabel{};
	// IMGUI Function
	int leCount;

	void GUI_Function();
	std::string m_guiTitle;
	glm::vec2 m_mousePosition;
	bool m_isGridEnabled;

	// Game Objects for the Scene
	Target* m_pTarget;
//	StarShip* m_pStarShip;
	std::vector<Obstacle*> m_pObstacles;

	// New for Lab 7.1
	Background* m_pBackground;

	//7.2
	//CloseCombatEnemy* m_pStarShip;
	


	void BuildObstaclePool();

	// Create our Division Scheme (Grid of PathNodes)
	std::vector<PathNode*> m_pGrid;
	void m_buildGrid();
	void m_toggleGrid(bool state) const;
	void m_clearNodes();

	// LOS functions
	bool m_checkAgentLOS(Agent* agent, DisplayObject* target_object) const;
	bool m_checkPathNodeLOS(PathNode* path_node, DisplayObject* target_object) const;
	void m_checkAllNodesWithTarget(DisplayObject* target_object) const;
	void m_checkAllNodesWithBoth() const;
	void m_setPathNodeLOSDistance(int distance) const;

	// LOS Variables (fields)
	LOSMode m_LOSMode{};
	int m_pathNodeLOSDistance;

	// Debugging Variables
	bool m_bDebugView;

	// Decision Tree
	//DecisionTree* m_decisionTree;


	//lab 8
	std::vector<Torpedo*> m_pTorpedoes;
	std::vector<TorpedoK*> m_pTorpedoesK;

	// Input Control
	int m_pCurrentInputType{};
	void GetPlayerInput();
	void GetKeyboardInput();
};

#endif /* defined (__PLAY_SCENE__) */