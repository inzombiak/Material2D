require "Scripts/Heroes/IHero"

WarriorHero =
{
	_name = "Warrior",
	_hp = 10,
	_sglClickAblPath = "Scripts/Heroes/Warrior Hero/MeleeSweep.lua",
	_dblClickAblPath = "Scripts/Heroes/Warrior Hero/ThrownDagger.lua",
	_spritesheetPath = "Images/Entities/Hero/Hero_SS.sprites",
	_animPath = "Images/Entities/Hero/Hero_Walk.anim",
	_animMsgTable = {}
}

WarriorHero.__index = WarriorHero

function WarriorHero:Create()
	local b = {}
    setmetatable(b, WarriorHero)
	setmetatable(WarriorHero,  { __index = IHero})
	b:Init()
    return b
end

function WarriorHero:Init()
	self._animMsgTable["MoveDown"] = "move_down"
	self._animMsgTable["MoveUp"] = "move_up"
	self._animMsgTable["MoveLeft"] = "move_left"
	self._animMsgTable["MoveRight"] = "move_right"
end

hero = WarriorHero:Create()