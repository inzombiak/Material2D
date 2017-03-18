#ifndef GRIDITEM_H
#define GRIDITEM_H

//#include "IHero.h"

#import <SpriteKit/SpriteKit.h>
#include <memory>

class IMaterial;
class GridItem
{
public:
	GridItem(int x, int y, int w, int h, SKSpriteNode *sprite);
	~GridItem();

	void Update()
	{
	//	if (!m_hero)
		//	m_cell.setFillColor(sf::Color::Transparent);
	}
    void AddMarker()
    {
        SKShapeNode* marker = [SKShapeNode shapeNodeWithCircleOfRadius:10];
        marker.fillColor = [SKColor purpleColor];
        marker.zPosition = 30;
        [m_sprite addChild:marker];
    }
    void ApplyMaterial(std::shared_ptr<IMaterial> newMat);
    std::shared_ptr<IMaterial> GetMaterial() const
    {
        return m_material;
    }
    bool HasMaterial() const;
	//void AddHero(IHero* hero)
	//{
		//m_hero = hero;
	//}
	//bool HasHero() const
	//{
	//	return m_hero != 0;
	//}
	//void RemoveHero()
	//{
	//	m_hero = 0;
	//}
private:
    SKSpriteNode *m_sprite;
    SKTexture* m_spriteSheet;
    std::shared_ptr<IMaterial> m_material;
	//IHero* m_hero = 0;
	//sf::RectangleShape m_cell;
};

#endif
