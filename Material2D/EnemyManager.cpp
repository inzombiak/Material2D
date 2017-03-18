#include "EnemyManager.h"

#include "TinyXML2/tinyxml2.h"
#include "LuaWrapper/LUAWrapper.h"
#include "LuaWrapper/LUAObject.h"

#include <cmath>

EnemyManager::EnemyManager()
{
}


EnemyManager::~EnemyManager()
{
}

void EnemyManager::Init(SKScene* scene, const std::string& envName, const std::string& tableName, const std::string& globalName)
{
	std::vector<std::shared_ptr<LuaObject>> luaPtrs = LuaWrapper::GetInstance().TableToVector<std::shared_ptr<LuaObject>>(envName, tableName, globalName);

	if (luaPtrs.size() < 1)
		return;
	
	LoadSpritesheet(luaPtrs[0]);
	
	for (unsigned int i = 0; i < luaPtrs.size(); ++i)
	{
		m_enemies.push_back(Enemy(i, luaPtrs[i], m_enemySpriteSheet));
        m_enemies[i].m_tempOffsetX = m_tempOffsetX;
        m_enemies[i].m_tempOffsetY = m_tempOffsetY;
        m_enemies[i].Init(scene);
        m_collisionBodies[m_enemies[i].GetPos().second][m_enemies[i].GetPos().first] = i + 1;
	}
    
    LuaWrapper::GetInstance().RegisterMemberFunction<EnemyManager, void>("Level", this, "ILevel", "thisLevel", &EnemyManager::KillEnemyFromLua, "KillEnemyInGame");
}

void EnemyManager::SetLevelCollisionBodies(const std::vector<std::vector<int>>& collisionBodies)
{
    m_collisionBodies = collisionBodies;
}

void EnemyManager::Update()
{
    bool contact;
    std::pair<float, float> oldPos, pos, stepPos;
    float dir = 0;
    float speed = 0;
    int checkStartRow;
    int checkEndRow;
    int checkStartCol;
    int checkEndCol;
	for (unsigned int i = 0; i < m_enemies.size(); ++i)
	{
        contact = false;
        
		if (!m_enemies[i].IsAlive())
			continue;
        m_collisionBodies[m_enemies[i].GetPos().second][m_enemies[i].GetPos().first] = -1;
        
        stepPos = oldPos = m_enemies[i].GetPos();
        speed = m_enemies[i].GetSpeed();
        dir = m_enemies[i].GetDirection();
        
        pos.first = oldPos.first + speed * cos(dir);
        pos.second = oldPos.second + speed * sin(dir);
        
        if (pos.first < 0.0001)
        {
            contact = true;
            pos.first = 0;
        }
        else if (pos.first >= m_bounds.first)
        {
            contact = true;
            pos.first = m_bounds.first - 1;
        }
        if (pos.second < 0.0001)
        {
            contact = true;
            pos.second = 0;
        }
        else if(pos.second >= m_bounds.second)
        {
            contact = true;
            pos.second = m_bounds.second - 1;
        }
        
        checkStartCol  = std::min(floor(oldPos.first),  ceil(pos.first));
        checkEndCol    = std::max(floor(oldPos.first),  ceil(pos.first));
        checkStartRow  = std::min(floor(oldPos.second), ceil(pos.second));
        checkEndRow    = std::max(floor(oldPos.second), ceil(pos.second));
        
        unsigned int row = 0, col = 0;
        if(checkStartCol < 0 || checkStartRow <0 || checkEndRow >= m_bounds.second || checkEndCol >= m_bounds.first)
            continue;
        
        for (row = checkStartRow; row <= checkEndRow; row++)
        {
            for(col = checkStartCol; col <= checkEndCol; col++)
            {
                if(m_collisionBodies[row][col] != -1)
                {
                    contact = true;
                    break;
                }
            }

            if(contact)
                break;
        }
     
		m_enemies[i].Update();
        
        if (!m_enemies[i].IsAlive())
            continue;
        
        if(contact)
        {
            m_enemies[i].SetPos(round(col - cos(dir)), round(row - sin(dir)));
            if(m_collisionBodies[row][col] > 0 && m_collisionBodies[row][col] <= m_enemies.size())
            {
                m_enemies[i].ContactWithStatic(m_enemies[m_collisionBodies[row][col] - 1].GetLuaObj());
                m_enemies[m_collisionBodies[row][col] - 1].ContactWithStatic(m_enemies[i].GetLuaObj());
            }
            else
                m_enemies[i].ContactWithStatic(NULL);
        }
        
         m_collisionBodies[m_enemies[i].GetPos().second][m_enemies[i].GetPos().first] = i + 1;
        
	}
}

void EnemyManager::UpdateRender()
{
	for (unsigned int i = 0; i < m_enemies.size(); ++i)
        if(m_enemies[i].IsAlive())
            m_enemies[i].UpdateRender();
}

std::vector<EnemyData> EnemyManager::GetEnemyData()
{
	std::vector<EnemyData> result;
	result.resize(m_enemies.size());

	for (unsigned int i = 0; i < result.size(); ++i)
	{
		if (!m_enemies[i].IsAlive())
			continue;

		result[i].pos = m_enemies[i].GetPos();
        result[i].pos.first = round(result[i].pos.first);
        result[i].pos.second = round(result[i].pos.second);
        result[i].direction = m_enemies[i].GetDirection();
        result[i].speed = m_enemies[i].GetSpeed();
		result[i].pattern = m_enemies[i].GetPattern();
		result[i].hp = m_enemies[i].GetHealth();
		result[i].dmg = m_enemies[i].GetDamage();
		result[i].index = i;
	}

	return result;
}

void EnemyManager::SetEnemyHP(unsigned int index, int amount)
{
	if (index > m_enemies.size())
		return;

	if (amount <= 0)
		KillEnemy(index);
	else
		m_enemies[index].SetHealth(amount);
}

void EnemyManager::KillEnemy(unsigned int index)
{
	m_unusedEnemyIndicies.push(index);
	LuaWrapper::GetInstance().RunFunction<void>("Level", FuncNameInfo("KillEnemy", "thisLevel"), index);
	LuaWrapper::GetInstance().DeleteLuaObject("Level", m_enemies[index-1].GetName());
    
    m_collisionBodies[m_enemies[index - 1].GetPos().second][m_enemies[index - 1].GetPos().first] = -1;
	m_enemies[index - 1].Kill();
}

void EnemyManager::KillEnemyFromLua(unsigned int index)
{
    std::cout << "Lua kill command for enemy wiht index: " << index << std::endl;
    m_unusedEnemyIndicies.push(index);
    
    m_collisionBodies[m_enemies[index - 1].GetPos().second][m_enemies[index - 1].GetPos().first] = -1;
    m_enemies[index - 1].Kill();
    
    LuaWrapper::GetInstance().DeleteLuaObject("Level", m_enemies[index-1].GetName());
}

void EnemyManager::LoadSpritesheet(std::shared_ptr<LuaObject> enemyPtr)
{
	std::string filename = enemyPtr->CallFunction<std::string>("GetAnimationPath");
	std::string ssFP;

	{
		tinyxml2::XMLDocument animFile;
		if (animFile.LoadFile(filename.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
            std::cout<< "ERROR Loading %s" << filename << std::endl;

		tinyxml2::XMLElement* pAnimations = animFile.FirstChildElement("animations");
		ssFP = pAnimations->Attribute("spriteSheet");

		if (ssFP.empty())
			return;

	}

	tinyxml2::XMLDocument spriteFile;
	ssFP = ConvertRelativePathToStatic(filename, ssFP);
	if (spriteFile.LoadFile(ssFP.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
		std::cout << "ERROR LOADING: " << ssFP << std::endl;

	std::string animFP = enemyPtr->CallFunction<std::string>("GetAnimationPath");
	tinyxml2::XMLDocument animationFile;
	if (animationFile.LoadFile(animFP.c_str()) != tinyxml2::XMLError::XML_SUCCESS)
		std::cout<< "ERROR Loading %s" << animFP << std::endl;

	tinyxml2::XMLElement* pImg = spriteFile.FirstChildElement("img");

	std::string ssFPRaw = pImg->Attribute("name");
	ssFPRaw = ConvertRelativePathToStatic(ssFP, ssFPRaw);
    
    NSString *tspNS = [NSString stringWithCString:ssFPRaw.c_str()
                                         encoding:[NSString defaultCStringEncoding]];
    UIImage *img = [UIImage imageWithContentsOfFile:tspNS];
    m_enemySpriteSheet = [SKTexture textureWithImage:img];
    
	//m_enemySpriteSheet.loadFromFile("Images/DungeonCrawl.png");
}

const Enemy& EnemyManager::GetEnemyByIndex(int id)
{
    return m_enemies[id];
}
