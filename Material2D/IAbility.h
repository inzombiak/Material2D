#pragma once

#include <vector>
#include <string>

class IAbility
{
public:
	IAbility(const unsigned int id, const std::string& filePath);

	int GetDamage();
	void Update();
	bool Use(std::vector<std::pair<int, int>>& result);
	int GetCooldown() const;
	int GetBeats() const;

protected:
	std::string m_filePath;
	unsigned int m_id;
	int m_beatsSinceUse;
	int m_coolDown;
};

