require "Scripts/Levels/ILevel"
require "Scripts/Enemies/FireSpinner"
require "Scripts/Enemies/Hopper"
require "Scripts/Enemies/BouncingBomb"

Tutorial = 
{
	_name = "Tutorial",
	_tmxPath = "Levels/Tutorial.tmx",
	_rows = 10,
	_columns =10,
	_heroX = 5,
	_heroY = 5,
	_enemies = {},
	_beatPause = 0.5,
	_beatBuffer = 0.1,
}

Tutorial.__index = Tutorial

function Tutorial:Create()
	local b = {}
    setmetatable(b, Tutorial)
	setmetatable(Tutorial,  { __index = ILevel})
	self:Init()
    return b
end

function Tutorial:Init()
	self._enemies[1] = FireSpinner:Create(1)
	self._enemies[1]:SetPosX(5)
	self._enemies[1]:SetPosY(3)
	
	self._enemies[2] = Hopper:Create(2)
	self._enemies[2]:SetPosX(3)
	self._enemies[2]:SetPosY(3)
	
	self._enemies[3] = BouncingBomb:Create(3)
	self._enemies[3]:SetPosX(8)
	self._enemies[3]:SetPosY(8)
end

thisLevel = Tutorial:Create()