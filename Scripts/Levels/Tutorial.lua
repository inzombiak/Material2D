require "Scripts/Levels/ILevel"
require "Scripts/Enemies/FireSpinner"
require "Scripts/Enemies/Hopper"
require "Scripts/Enemies/BouncingBomb"
require "Scripts/Enemies/Follower"
require "Scripts/Enemies/Explosive"
require "Scripts/Enemies/RollingStone"
require "Scripts/Enemies/Breakable"
require "Scripts/Enemies/Trigger"


Tutorial = 
{
	_name = "Tutorial",
	_tmxPath = "Levels/Tutorial.tmx",
	_rows = 15,
	_columns = 35,
	_heroX = 6,
	_heroY = 6,
	_enemies = {},
    _triggers = {},
	_beatPause = 0.1,
	_beatBuffer = 0.2,
}

Tutorial.__index = Tutorial

function Tutorial:Create()
	local b = {}
    setmetatable(b, Tutorial)
	setmetatable(Tutorial,  { __index = ILevel})
    
    for i=1,b._rows do
        b._levelMatrix[i] = {}     -- create a new row
        for j=1,b._columns do
            b._levelMatrix[i][j] = 0
        end
    end
    return b
end

function Tutorial:SetHeroX(newX)
    self._levelMatrix[self._heroY][self._heroX] = 0
    self._heroX = newX
    self._levelMatrix[self._heroY][self._heroX] = -1
end

function Tutorial:SetHeroY(newY)
    self._levelMatrix[self._heroY][self._heroX] = 0
    self._heroY = newY
    self._levelMatrix[self._heroY][self._heroX] = -1
end

function Tutorial:Init()

    self._enemies[1] =  Follower:Create(1, 2, 2)
    self._enemies[1]:SetParentLevel(thisLevel)
    self._enemies[1]:SetPosX(2)
    self._enemies[1]:SetPosY(2)
    self._levelMatrix[2][2] = 1

    self._enemies[2] = Explosive:Create(2, 12, 6)
    self._enemies[2]:SetParentLevel(thisLevel)
    self._enemies[2]:SetPosX(12)
    self._enemies[2]:SetPosY(6)
    self._levelMatrix[6][12] = 2

    self._enemies[3] = Explosive:Create(3, 12, 7)
    self._enemies[3]:SetParentLevel(thisLevel)
    self._enemies[3]:SetPosX(12)
    self._enemies[3]:SetPosY(7)
    self._levelMatrix[7][12] = 3

    self._enemies[4] = Explosive:Create(4, 11, 7)
    self._enemies[4]:SetParentLevel(thisLevel)
    self._enemies[4]:SetPosX(11)
    self._enemies[4]:SetPosY(7)
    self._levelMatrix[7][11] = 4
    
    self._enemies[5] = RollingStone:Create(5, 13, 2)
    self._enemies[5]:SetParentLevel(thisLevel)
    self._enemies[5]:SetPosX(13)
    self._enemies[5]:SetPosY(2)
    self._enemies[5]:SetDirection(0)
    self._levelMatrix[2][13] = 5
    
    self._enemies[6] = Breakable:Create(6, 24, 2)
    self._enemies[6]:SetParentLevel(thisLevel)
    self._enemies[6]:SetPosX(24)
    self._enemies[6]:SetPosY(2)
    self._levelMatrix[2][24] = 6
    
    self._enemies[7] = Breakable:Create(7, 25, 2)
    self._enemies[7]:SetParentLevel(thisLevel)
    self._enemies[7]:SetPosX(25)
    self._enemies[7]:SetPosY(2)
    self._levelMatrix[2][25] = 7
    
    self._triggers[1] = Trigger:Create(1, 13, 3)
    self._triggers[1]:SetParentLevel(thisLevel)
    self._triggers[1]:SetPosX(13)
    self._triggers[1]:SetPosY(3)
    self._triggers[1]:ConnectTo(self._enemies[5])

    --[[self._enemies[5] =  Follower:Create(5, 8, 4)
    self._enemies[5]:SetParentLevel(thisLevel)
    self._enemies[5]:SetPosX(8)
    self._enemies[5]:SetPosY(4)
    self._levelMatrix[4][8] = 5]]--

    self._levelMatrix[self._heroY][self._heroX] = -1
    
end

function Tutorial:KillEnemy(i)
    if(i > #self._enemies or i  < 1) then
        print("INVALID ENEMY TO REMOVE, INDEX: ", i)
    end
    self:KillEnemyAtPos(self._enemies[i]:GetPosX(), self._enemies[i]:GetPosY())
end

function Tutorial:KillEnemyAtPos(x, y)
    if(x < 1 or y < 1 or x > self._columns or y > self._rows) then
        return
    end
    local id = self._levelMatrix[y][x]
    print(x .. ", " .. y .. ", " .. id)
    if(id > #self._enemies or id < 1) then
        print("INVALID ENEMY TO REMOVE, INDEX: ", id)
        return
    end
    
    if(self._enemies[id] == 0 or self._enemies[id] == nil) then
        return;
    end

    if(self._enemies[id]:GetIsAlive()) then
        print(self._enemies[id]:GetName() .. " " .. id .. " is being killed")
        self._levelMatrix[y][x] = 0
        self._enemies[id]:Kill()

        KillEnemyInGame(id)
        self._enemies[id] = 0
    else
        print(self._enemies[id]:GetName() .. " " .. id .. " is already dead")
        return
    end
end

thisLevel = Tutorial:Create()
thisLevel:Init()