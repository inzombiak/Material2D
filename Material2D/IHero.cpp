#define _USE_MATH_DEFINES
#include <cmath> 
#include <iostream>

#include "IHero.h"
#include "IAbility.h"

#include "LuaWrapper/LuaWrapper.h"
#include "TinyXML2/tinyxml2.h"


IHero::IHero(unsigned int id, std::string filepath, SKScene* scene)
{
	m_id = id;
	m_filePath = filepath;
	
	LuaWrapper::GetInstance().LoadScript(filepath + std::to_string(id), m_filePath);
	m_sglClickAb = new IAbility(1, LuaWrapper::GetInstance().RunFunction<std::string>(filepath + std::to_string(id), FuncNameInfo("GetSglClickPath", "hero")));
	m_dblClickAb = new IAbility(2, LuaWrapper::GetInstance().RunFunction<std::string>(filepath + std::to_string(id), FuncNameInfo("GetDblClickPath", "hero")));

	LoadSpriteSheet();
	LoadAnimationFile();

    m_sprite = [SKSpriteNode spriteNodeWithColor:[UIColor blueColor] size:CGSizeMake(m_tileWidth, m_tileHeight)];
	UpdateAnimation();
    
//    auto ssW = m_spriteSheet.size.width;
//    auto ssH = m_spriteSheet.size.height;
//    double margin = 0.3;
//    
//    NSString *tspNS = [NSString stringWithCString:"Images/Entities/Hero/Hero.png"
//                                         encoding:[NSString defaultCStringEncoding]];
//    UIImage *img = [UIImage imageWithContentsOfFile:tspNS];
//    auto priteSheet = [SKTexture textureWithImage:img];
//    double marginX = (double)32/96;
//    double marginY = 0.25;
    //auto texRect = CGRectMake(0, marginY, marginX, marginY)


    //m_texRect = CGRectMake(0, 2*margin, margin, margin);
   // m_sprite = [SKSpriteNode spriteNodeWithTexture: m_spriteSheet];//[SKTexture textureWithRect:m_texRect inTexture:m_spriteSheet]];
    //m_sprite = [SKSpriteNode spriteNodeWithImageNamed:@"Spaceship"];
    m_sprite.zPosition = 2;
    m_sprite.xScale = 1;
    m_sprite.yScale = 1;
    [scene addChild:m_sprite];
}


IHero::~IHero()
{
	if (m_sglClickAb)
	{
		delete m_sglClickAb;
		m_sglClickAb = 0;
	}

	if (m_dblClickAb)
	{
		delete m_dblClickAb;
		m_dblClickAb = 0;
	}
	
}

std::pair<int, int> IHero::GetCooldowns() const
{
	return std::pair<int, int>(m_sglClickAb->GetCooldown(), m_dblClickAb->GetCooldown());
}

std::pair<int, int> IHero::GetBeatsPassed() const
{
	return std::pair<int, int>(m_sglClickAb->GetBeats(), m_dblClickAb->GetBeats());
}

void IHero::Update()
{
	m_sglClickAb->Update();
	m_dblClickAb->Update();
}

void IHero::UpdateRender()
{
	m_counter++;
	if (m_counter >= m_maxCounter)
	{
		m_currentAnimation.NextFrame();

        Animation::Frame currentFrame = m_currentAnimation.frames[m_currentAnimation.GetCurrentFrameIndex()];
        Animation::IntRect currentBounds = m_spriteDefinitions[currentFrame.sprites[0].spriteDir].spriteBounds[currentFrame.sprites[0].spriteName];
        UpdateSKSprite(currentBounds);
		m_counter = 0;
	}
}

AbilityInfo IHero::Tap(double angle, bool useOwnDirection)
{
    angle = m_direction;
    
	AbilityInfo result;

	m_sglClickAb->Use(result.pattern);

	double angleSin = std::sin(angle);
	double angleCos = std::cos(angle);

	if (std::abs(angleCos) < 0.001)
		angleCos = 0;
	if (std::abs(angleSin) < 0.001)
		angleSin = 0;

	for (unsigned int i = 0; i < result.pattern.size(); ++i)
	{
		int posX = int(result.pattern[i].first * angleCos - result.pattern[i].second * angleSin);
		int posY = int(result.pattern[i].first * angleSin + result.pattern[i].second * angleCos);
		result.pattern[i] = std::pair<int, int>(posX, posY);
	}

	result.damage = m_sglClickAb->GetDamage();

	return result;
}

AbilityInfo IHero::DoubleTap(double angle, bool useOwnDirection)
{
    if(useOwnDirection)
        angle = m_direction;
    
	AbilityInfo result;

	m_dblClickAb->Use(result.pattern);

	double angleSin = std::sin(angle);
	double angleCos = std::cos(angle);

	if (std::abs(angleCos) < 0.001)
		angleCos = 0;
	if (std::abs(angleSin) < 0.001)
		angleSin = 0;

	for (unsigned int i = 0; i < result.pattern.size(); ++i)
	{
		int posX = int(result.pattern[i].first * angleCos - result.pattern[i].second * angleSin);
		int posY = int(result.pattern[i].first * angleSin + result.pattern[i].second * angleCos);
		result.pattern[i] = std::pair<int, int>(posX, posY);
	}

	result.damage = m_dblClickAb->GetDamage();

	return result;
}

std::pair<int, int> IHero::Move(double direction)
{
	double clampedAngle = std::round(direction / M_PI_2) * M_PI_2;
	if (clampedAngle >= 2*M_PI)
		clampedAngle -= 2*M_PI;

	int cos = int (std::cos(clampedAngle));
	int sin = int (std::sin(clampedAngle));

	m_pos.first += cos;
	m_pos.second += sin;

	m_direction = clampedAngle;
    UpdateSKSpritePos();
	UpdateAnimation();

	return m_pos;
}

std::pair<int, int> IHero::MoveToPoint(const std::pair<int, int>& newPos)
{
    m_pos.first = newPos.first;
    m_pos.second = newPos.second;
    
    UpdateSKSpritePos();
    UpdateAnimation();
    
    return m_pos;
}

void IHero::Rotate(double angle)
{
	m_direction = std::round((m_direction + angle) / M_PI_2) * M_PI_2;
	if (m_direction >= 2 * M_PI)
		m_direction -= 2 * M_PI;
    if (m_direction < 0)
        m_direction += 2 * M_PI;
	UpdateAnimation();
}

void IHero::LoadSpriteSheet()
{

	std::string filename = LuaWrapper::GetInstance().RunFunction<std::string>(m_filePath + std::to_string(m_id), FuncNameInfo("GetSpritesheetPath", "hero"));

	tinyxml2::XMLDocument spriteFile;
	if (spriteFile.LoadFile(filename.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
		std::cout << "ERROR LOADING" << filename << std::endl;

	tinyxml2::XMLElement* pImg = spriteFile.FirstChildElement("img");
	std::string textureName = pImg->Attribute("name");
    textureName = "Images/Entities/Hero/" + textureName;
    NSString *tspNS = [NSString stringWithCString:textureName.c_str()
                                         encoding:[NSString defaultCStringEncoding]];
    UIImage *img = [UIImage imageWithContentsOfFile:tspNS];
    m_spriteSheet = [SKTexture textureWithImage:img];
    
	tinyxml2::XMLElement* pDefs = pImg->FirstChildElement("definitions");
	tinyxml2::XMLElement* pBaseDir = pDefs->FirstChildElement("dir");
	tinyxml2::XMLElement* pDir = pBaseDir->FirstChildElement("dir");
	tinyxml2::XMLElement* pSprite;
	Animation::IntRect spriteBounds;
	Animation::SpriteDefinition newSpriteDir;

	while (pDir)
	{
		newSpriteDir.spriteBounds.clear();
		if (!pDir->Attribute("name"))
			std::cout << "ERROR PROCESSING" << filename << "NO ATTRIBUTE name" << std::endl;
		
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
		pDir = pDir->NextSiblingElement("dir");
	}
}

void IHero::LoadAnimationFile()
{
	std::string filename = LuaWrapper::GetInstance().RunFunction<std::string>(m_filePath + std::to_string(m_id), FuncNameInfo("GetAnimationPath", "hero"));

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
			}
			newAnimation.frames.push_back(newFrame);
			pCell = pCell->NextSiblingElement("cell");
		}
		m_animations[newAnimation.name] = newAnimation;
		pAnim = pAnim->NextSiblingElement("anim");
	}
}

void IHero::UpdateAnimation()
{
	std::string animName;

	if (m_direction == 0)
		animName = LuaWrapper::GetInstance().RunFunction<std::string>(m_filePath + std::to_string(m_id), FuncNameInfo("GetAnimationNameByMsg", "hero"), "MoveRight");
	else if (m_direction == M_PI_2)
		animName = LuaWrapper::GetInstance().RunFunction<std::string>(m_filePath + std::to_string(m_id), FuncNameInfo("GetAnimationNameByMsg", "hero"), "MoveDown");
	else if (m_direction == M_PI)
		animName = LuaWrapper::GetInstance().RunFunction<std::string>(m_filePath + std::to_string(m_id), FuncNameInfo("GetAnimationNameByMsg", "hero"), "MoveLeft");
	else if (m_direction == M_PI_2 * 3)
		animName = LuaWrapper::GetInstance().RunFunction<std::string>(m_filePath + std::to_string(m_id), FuncNameInfo("GetAnimationNameByMsg", "hero"), "MoveUp");
	else
		animName = LuaWrapper::GetInstance().RunFunction<std::string>(m_filePath + std::to_string(m_id), FuncNameInfo("GetAnimationNameByMsg", "hero"), "MoveRight");

	if (animName.empty())
		return;

	m_currentAnimation = m_animations[animName];

	Animation::Frame currentFrame = m_currentAnimation.frames[m_currentAnimation.GetCurrentFrameIndex()];
	Animation::IntRect currentBounds = m_spriteDefinitions[currentFrame.sprites[0].spriteDir].spriteBounds[currentFrame.sprites[0].spriteName];
    UpdateSKSprite(currentBounds);
}

void IHero::UpdateSKSprite(Animation::IntRect currentBounds)
{
    double tx = currentBounds.left / m_spriteSheet.size.width;
    double ty = 1 - (currentBounds.top + currentBounds.height) / m_spriteSheet.size.height;
    double tw = currentBounds.width / m_spriteSheet.size.width;
    double th = currentBounds.height / m_spriteSheet.size.height;
    
    m_texRect = CGRectMake(tx, ty, tw, th);
   // m_texRect = CGRectMake(currentBounds.left/m_tileWidth * m_textureMarginX, (m_spriteSheet.size.height - currentBounds.top)/m_tileHeight * m_textureMarginY - m_textureMarginY, currentBounds.width/m_tileWidth * m_textureMarginX, currentBounds.height/m_tileHeight * m_textureMarginY);
    m_sprite.texture = [SKTexture textureWithRect:m_texRect inTexture:m_spriteSheet];

}