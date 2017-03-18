require "Scripts/Enemies/IEnemy"

Hopper =
{
	_name = "Hopper",
	_id = -1,
	_health = 3,
	_damage = 1,
	_attackPattern = {},
    _speed = 0.6,
}
Hopper.__index = Hopper

function Hopper:Create(newID)
	local b = {}
    setmetatable(b, Hopper)
	setmetatable(Hopper,  { __index = IEnemy})
	self._id = newID
	self:Init()
	math.randomseed(os.time())
    return b
end

function Hopper:Init()
	self._attackPattern[1] = {0, 0}
	self._animPath = "Images/Enemies/Hopper/Hopper_Anim.anim"
	self._currAnimName = "idle"
end

function Hopper:GetSpeed()
    if(self._modifiers["Ice"]) then
        return self._speed * 1.5;
    end

    return self._speed;
end

function Hopper:Update()

    local _speed = self._speed;

    if(self._modifiers["Ice"]) then
        _speed = self._speed * 1.5;
        self._modifiers["Ice"] = false;
    end

    local cos = math.cos(self._direction);
    local sin = math.sin(self._direction);

    if(math.abs(cos) < 0.00001) then
        cos = 0;
    end
    if(math.abs(sin) < 0.00001) then
        sin = 0;
    end

    self._posX = self._posX + cos * _speed;
    self._posY = self._posY + sin * _speed;
    
end

function Hopper:ContactWithStatic(other)
	self._direction = self._direction + math.pi / 2
	if(self._direction >= math.pi * 2) then
		self._direction = self._direction - math.pi * 2;
	end
end