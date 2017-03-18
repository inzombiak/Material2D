require "Scripts/Enemies/IEnemy"

BouncingBomb =
{
	_name = "BouncingBomb",
	_id = -1,
	_health = 3,
	_damage = 1,
	_attackPattern = {},
	_direction = 0,
	
	_move = 0
}
BouncingBomb.__index = BouncingBomb

function BouncingBomb:Create(newID)
	local b = {}
    setmetatable(b, BouncingBomb)
	setmetatable(BouncingBomb,  { __index = IEnemy})
	self._id = newID
	self:Init()
	math.randomseed(os.time())
    return b
end

function BouncingBomb:Init()
	self:SetPattern()
	self._animPath = "Images/Enemies/BouncingBomb/BouncingBomb_Anim.anim"
	self._currAnimName = "idle"
end

function BouncingBomb:SetPattern()
	self._attackPattern[1] = {-1, -1}
	self._attackPattern[2] = {-1, 1}
	self._attackPattern[3] = {1, -1}
	self._attackPattern[4] = {1, 1}
end

function BouncingBomb:RemovePattern()
	self._attackPattern[1] = nil
	self._attackPattern[2] = nil
	self._attackPattern[3] = nil
	self._attackPattern[4] = nil
end

function BouncingBomb:Update()
	local dice = math.random(10)
	if(dice >= 9) then
		self._direction = self._direction + math.pi/ 4
	end
	if(self._direction >= math.pi * 2) then
		self._direction = self._direction - math.pi * 2;
	end
	if(self._move == 1) then
		self:RemovePattern()
		self._posX = self._posX + math.ceil(2*math.cos(self._direction))
		self._posY = self._posY + math.ceil(2*math.sin(self._direction))
		self._move = 0;
		self._currAnimName = "idle"
	else
		self:SetPattern()
		self._move = 1;
		self._currAnimName = "explode"
	end
end

function BouncingBomb:ContactWithStatic()
	self._direction = self._direction + math.pi / 4
	if(self._direction >= math.pi * 2) then
		self._direction = self._direction - math.pi * 2;
	end
end