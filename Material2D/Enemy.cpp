#include "Enemy.h"

#include "TinyXML2/tinyxml2.h"
#include "LuaWrapper/LuaWrapper.h"
#include "LuaWrapper/LUAObject.h"

#include "glm/glm.hpp"

Enemy::Enemy(unsigned int id, std::shared_ptr<LuaObject> obj, SKTexture* tex) : m_spriteSheet(tex)
{
    m_luaObj = obj;
    m_health = GetHealth();
    m_id = id;

}

Enemy::~Enemy()
{

}


void Enemy::Init(SKScene* scene)
{
	m_isAlive = true;


	LoadSpriteSheet();
	LoadAnimationFile();
    
    m_sprites.reserve(m_maxSprites);
    SKSpriteNode* newSpr;
    
    for(unsigned int i = 0; i < m_maxSprites; ++i)
    {
        newSpr = [SKSpriteNode spriteNodeWithColor:[UIColor blueColor] size:CGSizeMake(m_tileWidth, m_tileHeight)];
        newSpr.zPosition = 20;
        m_sprites.push_back(newSpr);
        [scene addChild:newSpr];
    }
    
    UpdateRender();

}


void Enemy::Update()
{
	if (!m_isAlive)
		return;

	m_luaObj->CallFunction<void>("Update");

}

void Enemy::UpdateRender()
{
	std::string currentAnim = m_luaObj->CallFunction<std::string>("GetCurrentAnimationName");
	if (m_currentAnimation.name.compare(currentAnim) != 0)
		m_currentAnimation = m_animations[currentAnim];

	m_counter++;
	if (m_counter >= m_maxCounter)
	{
		m_currentAnimation.NextFrame();

		Animation::Frame currentFrame = m_currentAnimation.frames[m_currentAnimation.GetCurrentFrameIndex()];
		for (unsigned int i = 0; i < currentFrame.sprites.size(); ++i)
		{
			Animation::IntRect currentBounds = m_spriteDefinitions[currentFrame.sprites[i].spriteDir].spriteBounds[currentFrame.sprites[i].spriteName];
			auto pos = GetPos();
            glm::vec2 worldPos((float)pos.first * 32, (float)pos.second * 32);
            worldPos += glm::vec2((float)currentFrame.sprites[i].x, (float)currentFrame.sprites[i].y);
            [m_sprites[i] setPosition:CGPointMake(m_tempOffsetX + worldPos.x, m_tempOffsetY - worldPos.y)];
            m_sprites[i].hidden = false;
            UpdateSKSpriteTexture(m_sprites[i], currentBounds);
			m_counter = 0;
		}
        
        if(currentFrame.sprites.size() < m_sprites.size())
        {
            for (unsigned int i = currentFrame.sprites.size(); i < m_sprites.size(); ++i)
            {
                m_sprites[i].hidden = true;
            }
        }
	}
}

void Enemy::Kill()
{
	m_isAlive = false;
	m_luaObj = nullptr;
	m_health = 0;
    
    for (unsigned int i = 0; i < m_sprites.size(); ++i)
    {
        m_sprites[i].hidden = true;
    }
}

void Enemy::LoadSpriteSheet()
{
	std::string objName = m_name = m_luaObj->CallFunction<std::string>("GetName");
	std::string filename = m_luaObj->CallFunction<std::string>("GetAnimationPath");
	std::string ssFP;

	{
		tinyxml2::XMLDocument animFile;
		if (animFile.LoadFile(filename.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
			std::cout << "ERROR LOADING" << filename << std::endl;

		tinyxml2::XMLElement* pAnimations = animFile.FirstChildElement("animations");
		ssFP = pAnimations->Attribute("spriteSheet");

		if (ssFP.empty())
			return;

	}

	tinyxml2::XMLDocument spriteFile;
	ssFP = ConvertRelativePathToStatic(filename, ssFP);
	if (spriteFile.LoadFile(ssFP.c_str()) != tinyxml2::XMLError::XML_SUCCESS)	
		std::cout << "ERROR LOADING" << ssFP << std::endl;

	tinyxml2::XMLElement* pImg = spriteFile.FirstChildElement("img");
	tinyxml2::XMLElement* pDefs = pImg->FirstChildElement("definitions");
	tinyxml2::XMLElement* pBaseDir = pDefs->FirstChildElement("dir");
	tinyxml2::XMLElement* pDir = pBaseDir->FirstChildElement("dir");

	while (pDir && std::strcmp(pDir->Attribute("name"), objName.c_str()) != 0)
        pDir = pDir->NextSiblingElement("dir");
		

	if (!pDir)
	{
		printf("INCORRECT OBJECT NAME: %s not found in spritesheet \n", objName.c_str());
        
		return;
	}

	tinyxml2::XMLElement* pSprite;
	Animation::IntRect spriteBounds;
	Animation::SpriteDefinition newSpriteDir;;
	newSpriteDir.dirName = pDir->Attribute("name");

	pSprite = pDir->FirstChildElement("spr");
	std::string name;
	while (pSprite)
	{
		
		name = pSprite->Attribute("name");
		if (pSprite->QueryIntAttribute("x", &spriteBounds.left) != tinyxml2::XMLError::XML_SUCCESS)
			std::cout << "ERROR PROCESSING" << filename << "NO ATTRIBUTE x" << std::endl;
		if (pSprite->QueryIntAttribute("y", &spriteBounds.top) != tinyxml2::XMLError::XML_SUCCESS)
			std::cout << "ERROR PROCESSING" << filename << "NO ATTRIBUTE y" << std::endl;
		if (pSprite->QueryIntAttribute("w", &spriteBounds.width) != tinyxml2::XMLError::XML_SUCCESS)
			std::cout << "ERROR PROCESSING" << filename << "NO ATTRIBUTE w" << std::endl;
		if (pSprite->QueryIntAttribute("h", &spriteBounds.height) != tinyxml2::XMLError::XML_SUCCESS)
			std::cout << "ERROR PROCESSING" << filename << "NO ATTRIBUTE h" << std::endl;
		newSpriteDir.spriteBounds[name] = spriteBounds;
		pSprite = pSprite->NextSiblingElement("spr");
	}

	m_spriteDefinitions[newSpriteDir.dirName] = newSpriteDir;
}

void Enemy::LoadAnimationFile()
{

	std::string filename = m_luaObj->CallFunction<std::string>("GetAnimationPath");

	tinyxml2::XMLDocument animationFile;
	if (animationFile.LoadFile(filename.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
		std::cout << "ERROR LOADING" << filename << std::endl;

	tinyxml2::XMLElement* pAnimations = animationFile.FirstChildElement("animations");
	tinyxml2::XMLElement* pAnim = pAnimations->FirstChildElement("anim");
	tinyxml2::XMLElement* pCell;
	tinyxml2::XMLElement* pSprite;
	Animation::Animation newAnimation;
	Animation::Frame newFrame;
	Animation::Sprite newSprite;
	std::string trash;
	std::vector<std::string> trashVec;
    int currMax = 0;
	while (pAnim)
	{
		newAnimation.frames.clear();
		if (!pAnim->Attribute("name"))
			std::cout << "ERROR PROCESSING" << filename << "NO ATTRIBUTE name" << std::endl;
		newAnimation.name = pAnim->Attribute("name");
		if (pAnim->QueryIntAttribute("loops", &newAnimation.loops) != tinyxml2::XMLError::XML_SUCCESS)
			std::cout << "ERROR PROCESSING" << filename << "NO ATTRIBUTE loops" << std::endl;
		pCell = pAnim->FirstChildElement("cell");
		while (pCell)
		{
            currMax = 0;
			newFrame.sprites.clear();
			if (pCell->QueryIntAttribute("delay", &newFrame.maxDelay) != tinyxml2::XMLError::XML_SUCCESS)
				std::cout << "ERROR PROCESSING" << filename << "NO ATTRIBUTE delay" << std::endl;

			pSprite = pCell->FirstChildElement("spr");
			while (pSprite)
			{
				if (pSprite->QueryIntAttribute("x", &newSprite.x) != tinyxml2::XMLError::XML_SUCCESS)
					std::cout << "ERROR PROCESSING" << filename << "NO ATTRIBUTE x" << std::endl;
				if (pSprite->QueryIntAttribute("y", &newSprite.y) != tinyxml2::XMLError::XML_SUCCESS)
					std::cout << "ERROR PROCESSING" << filename << "NO ATTRIBUTE y" << std::endl;
				if (pSprite->QueryIntAttribute("z", &newSprite.z) != tinyxml2::XMLError::XML_SUCCESS)
					std::cout << "ERROR PROCESSING" << filename << "NO ATTRIBUTE z" << std::endl;
				if (!pSprite->Attribute("name"))
					std::cout << "ERROR PROCESSING" << filename << "NO ATTRIBUTE name" << std::endl;
				trash = pSprite->Attribute("name");

				//Seperate directory from index
				trash.erase(trash.begin(), trash.begin() + 1);
				SplitString(trash, trashVec, '/');
				newSprite.spriteDir = trashVec[0];
				newSprite.spriteName = trashVec[1].c_str();

				newFrame.sprites.push_back(newSprite);
				pSprite = pSprite->NextSiblingElement("spr");
                currMax++;
                
			}
			newAnimation.frames.push_back(newFrame);
			pCell = pCell->NextSiblingElement("cell");
            if(currMax > m_maxSprites)
                m_maxSprites = currMax;
		}
		m_animations[newAnimation.name] = newAnimation;
		pAnim = pAnim->NextSiblingElement("anim");
	}
}

std::pair<float, float> Enemy::GetPos() const
{
	float x = m_luaObj->CallFunction<float>("GetPosX");
	float y = m_luaObj->CallFunction<float>("GetPosY");

	return std::pair<float, float>(x, y);
}
void Enemy::SetPos(float newPosX, float newPosY)
{
	m_luaObj->CallFunction<void>("SetPosX", newPosX);
	m_luaObj->CallFunction<void>("SetPosY", newPosY);
}

float Enemy::GetDirection() const
{
    return m_luaObj->CallFunction<float>("GetDirection");
}

float Enemy::GetSpeed() const
{
    return m_luaObj->CallFunction<float>("GetSpeed");
}

int Enemy::GetHealth() const
{
	return m_luaObj->CallFunction<int>("GetHealth");
}
void Enemy::SetHealth(int amount)
{
	m_luaObj->CallFunction<void>("SetHealth", amount);
}
int Enemy::GetDamage() const
{
	return m_luaObj->CallFunction<int>("GetDamage");
}
const std::string& Enemy::GetName() const
{
    return m_name;
}
std::vector<std::pair<int, int>> Enemy::GetPattern() const
{
	return m_luaObj->CallFunctionVec<int, int>("GetAttackPattern");
}

std::shared_ptr<LuaObject> Enemy::GetLuaObj() const
{
    return m_luaObj;
}

void Enemy::ContactWithStatic(std::shared_ptr<LuaObject> other) const
{
    if(!other)
        m_luaObj->CallFunction<void>("ContactWithStatic", "NULL");
    else
        m_luaObj->CallFunction<void>("ContactWithStatic", LuaCallDispatcher::LuaRefID(other->GetLuaRef()));
}

void Enemy::UpdateSKSpriteTexture(SKSpriteNode* spr, Animation::IntRect currentBounds)
{
    double tx = currentBounds.left / m_spriteSheet.size.width;
    double ty = 1 - (currentBounds.top + currentBounds.height) / m_spriteSheet.size.height;
    double tw = currentBounds.width / m_spriteSheet.size.width;
    double th = currentBounds.height / m_spriteSheet.size.height;
    
    m_texRect = CGRectMake(tx, ty, tw, th);
    [spr setSize:CGSizeMake(currentBounds.width, currentBounds.height)];
    spr.texture = [SKTexture textureWithRect:m_texRect inTexture:m_spriteSheet];
}
