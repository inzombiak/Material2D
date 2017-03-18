#include "GridItem.h"
#include "GameDefs.h"

GridItem::GridItem(int x, int y, int w, int h, SKSpriteNode *sprite) : m_sprite(sprite)
{
    [m_sprite setPosition:CGPointMake(x, y)];
}

GridItem::~GridItem()
{
    
}

bool GridItem::HasMaterial() const
{
    if(!m_material)
        return false;
    
    return m_material->GetType() != MaterialType::Default;
}

void GridItem::ApplyMaterial(std::shared_ptr<IMaterial> newMat)
{
    [m_sprite removeAllChildren];
    m_material = newMat;
    std::string color = m_material->GetColor();
    
    if(color.empty())
        return;
    
    SKShapeNode* mat = [SKShapeNode shapeNodeWithRectOfSize:m_sprite.size];
    //[mat setPosition:m_sprite ];
    if(color.compare("Red") == 0)
    {
        mat.fillColor = [SKColor redColor];
        mat.alpha = 0.6;
        mat.zPosition = 4;
        [m_sprite addChild:mat];
    }
    else if(color.compare("Blue") == 0)
    {
        mat.fillColor = [SKColor blueColor];
        mat.alpha = 0.6;
        mat.zPosition = 4;
        [m_sprite addChild:mat];
    }
    else if(color.compare("Green") == 0)
    {
        mat.fillColor = [SKColor greenColor];
        mat.alpha = 0.6;
        mat.zPosition = 4;
        [m_sprite addChild:mat];
    }
}

