#pragma once

#include "TinyXML2/tinyxml2.h"
#include "EnemyManager.h"
#include "GridItem.h"
#include "IHero.h"
#include "GameDefs.h"
#import <SpriteKit/SpriteKit.h>
#include <string>
#include <vector>
#include <stack>

struct CellData
{
	int row;
	int column;
	int R, G, B;
};

struct GridPoint
{
    int id = -1;
    int x = -1;
    int y = -1;
    int weight = 0;
    double priority = 0;
    void SetID(unsigned int rowSize)
    {
        id = y * rowSize + x;
    }
    bool operator<(const GridPoint& rhs) const
    {
        return rhs.priority < priority;
    }
    bool operator== (const GridPoint& rhs) const
    {
        return (rhs.x == x && rhs.y == y);
    }
    bool operator!= (const GridPoint& rhs) const
    {
        return (rhs.x != x || rhs.y != y);
    }
};

class IHero;
class LevelManager
{
public:
    int m_tempOffsetX = 10;
    int m_tempOffsetY = 660;
    
	LevelManager();
	~LevelManager();
	
	void LoadLevel(const std::string& level, SKScene* scene);
	void Update();
	void UpdateRender();
	void Clear();

	void UseAbility(bool isTap, double angle, bool useSingleScreenControls);
	void RotateHero(double angle) const;
	void MoveHero(double direction) const;
    void MoveToPoint(int x, int y);
    
	double GetBeatPause() const
	{
		return m_beatPause;
	}
	double GetBeatBuffer() const
	{
		return m_beatBuffer;
	}
	int GetRows() const
	{
		return m_rows;
	}
	int GetColumns() const
	{
		return m_columns;
	}
    
    void SetMaterial(MaterialType type);
    
//	const std::pair<int, int>& GetHeroPos() const
//	{
//		return m_hero->GetPos();
//	}
//	IHero* GetHero()
//	{
//		return m_hero;
//	}
	std::vector<CellData> GetDrawingData() const
	{
		return m_drawData;
	}
//	AbilityInfo GetAttackPattern(bool isTap, float angle = 0) const
//	{
//		if (isTap)
//			return m_hero->Tap();
//
//		return m_hero->DoubleTap();
//	}

private:
    void ProcessUpdate();
    void LoadTmx();
    void CreateCollisionBodies(tinyxml2::XMLElement* collisionRoot);
    SKSpriteNode* CreateGridSprite(int tileIndex, int worldIndex);
    void PerformPathfinding(const GridPoint& start, const GridPoint& end);
    std::vector<GridPoint> GetNeighbors(const GridPoint& point);
    void Heuristic(GridPoint& point, const GridPoint& target);
    std::pair<int, int> MouseToGrid(int x, int y);
    
	tinyxml2::XMLDocument m_xmlDoc;
    IHero* m_hero;
    EnemyManager m_enemyManager;
    
    int m_textureWidth = -1, m_textureHeight = -1;
	SKTexture *m_levelTileSet;
    SKScene* m_scene;
	std::string m_tmxPath;
	double m_beatPause = 0;
	double m_beatBuffer = 0;
	int m_rows = -1, m_columns = -1;
	int m_tileHeight = 32, m_tileWidth = 32;

	std::vector<std::vector<GridItem*>> m_gridItems;
	std::string m_levelFilepath;
	AbilityInfo m_playerAttackInfo;
	std::vector<CellData> m_drawData;
    std::vector<SKShapeNode*> m_collisionDebugSprites;
    SKShapeNode* m_movementMarker;
    std::vector<std::vector<int>> m_collisionBodies;
    std::stack<std::pair<int, int>> m_movementPath;
    
};