#include "IAbility.h"

#include "LuaWrapper/LuaWrapper.h"

IAbility::IAbility(const unsigned int id, const std::string& filepath)
{
	m_filePath = filepath;
	m_id = id;
	LuaWrapper::GetInstance().LoadScript(m_filePath + std::to_string(m_id), m_filePath);
	
	m_coolDown = 1;//LuaWrapper::GetInstance().GetGlobalValue<int>(m_filePath + std::to_string(m_id), "cooldown");
	m_beatsSinceUse = m_coolDown;
}

int IAbility::GetDamage()
{
	return LuaWrapper::GetInstance().GetGlobalValue<int>(m_filePath + std::to_string(m_id), "damage");
}

void IAbility::Update()
{
	m_beatsSinceUse++;
	
	if (m_beatsSinceUse > m_coolDown)
		m_beatsSinceUse = m_coolDown;
}

bool IAbility::Use(std::vector<std::pair<int, int>>& result)
{
	if (m_beatsSinceUse < m_coolDown)
	{
		result.resize(0);
		return false;
	}
	LuaWrapper::GetInstance().CleanStack();

	result = LuaWrapper::GetInstance().TableToVector<int, int>(m_filePath + std::to_string(m_id), "path");
    
	if (result.size() == 0)
		return false;
    
    return true;
}

int IAbility::GetCooldown() const
{
	return LuaWrapper::GetInstance().GetGlobalValue<int>(m_filePath + std::to_string(m_id), "cooldown");
}
int IAbility::GetBeats() const
{
	return m_beatsSinceUse;
}