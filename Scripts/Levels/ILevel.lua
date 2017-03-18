ILevel =
{
	_name = "NULL",
	_tmxPath = "NULL",
	_rows = -1,
	_columns = -1,
	_beatPause = -1,
	_beatBuffer = -1,
	_heroX = -1,
	_heroY = -1,
	_enemies = {},
    _levelMatrix = {},
    _triggers = {}
}

function ILevel:Create()
	local b = {}
    setmetatable(b, self)
	b.__index = self
    return b
end

function ILevel:GetName()
	return self._name
end

function ILevel:GetTmxPath()
	return self._tmxPath
end

function ILevel:GetRows()
	return self._rows
end

function ILevel:GetColumns()
	return self._columns
end

function ILevel:GetBeatPause()
	return self._beatPause
end

function ILevel:GetBeatBuffer()
	return self._beatBuffer
end

function ILevel:SetHeroX(newX)
    self._heroX = newX
end

function ILevel:SetHeroY(newY)
    self._heroY = newY
end

function ILevel:GetHeroX()
	return self._heroX
end

function ILevel:GetHeroY()
	return self._heroY
end

function ILevel:GetEnemies()
	return self._enemies
end

function ILevel:UpdateTriggers()
    for key,value in pairs(self._triggers) do --actualcode
        self._triggers[key]:Update()
    end
end

function ILevel:GetEnemy(i)
	print(self._enemies[i]:GetName())
	return self._enemies[i]
end

function round(x)
    if x%2 ~= 0.5 then
        return math.floor(x+0.5)
    end
    return x-0.5
end

function ILevel:EnemyMoved(oldX, oldY, newX, newY, id)
    oldX = round(oldX)
    oldY = round(oldY)
    newX = round(newX)
    newY = round(newY)
    if(oldX < 1 or oldY < 1 or oldX > self._columns or oldY > self._rows or
       newX < 1 or newY < 1 or newX > self._columns or newY > self._rows) then
        return
    end

    self._levelMatrix[oldY][oldX] = 0
    self._levelMatrix[newY][newX] = id
end

function ILevel:KillEnemy(i)
    if(i > #self._enemies or i == -1) then
        print("INVALID ENEMY TO REMOVE, INDEX: ", i)
    end
    self:KillEnemyAtPos(self._enemies[i]:GetPosX(), self._enemies[i]:GetPosY())
end

function ILevel:KillEnemyAtPos(x, y)
    if(x < 1 or y < 1 or x > self._columns or y > self._rows) then
        return
    end
    local id = self._levelMatrix[y][x]
    if(id > #self._enemies or id == -1) then
        print("INVALID ENEMY TO REMOVE, INDEX: ", i)
    end
    
    if(self._enemies[id] == 0 or self._enemies[id] == nil) then
        return;
    end
    
    print(id)
    print("Killing " .. self._enemies[id]._name);

    if(self._enemies[id]:GetIsAlive()) then
        self._enemies[id]:Kill()
    end
    
    self._levelMatrix[y][x] = -1
    table.remove(self._enemies, id);
    KillEnemyInGame(id)
    self._enemies[id] = 0
end