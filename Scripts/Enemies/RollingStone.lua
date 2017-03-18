require "Scripts/Enemies/IEnemy"
require "Scripts/Levels/ILevel"
require "math"

RollingStone =
{
    _name = "RollingStone",
    _id = -1,
    _isAlive = true,
    _health = 3,
    _damage = 1,
    _attackPattern = {},
    _modifiers = {},
    _speed = 0,
    _defaultSpeed = 1,
    _originalX = 0,
    _originalY = 0,
    _startMoving = false
}
-- RollingStone.__index =  RollingStone

function RollingStone:Create(newID, origX, origY)
    local b = {}
    setmetatable(b,  RollingStone)
    setmetatable( RollingStone,  { __index = IEnemy})
    self.__index = self
    b._id = newID
    
    b:Init()
    
    b._originalX = origX
    b._originalY = origY
    
    return b
end

function RollingStone:Init()
    self._attackPattern[1] = {0, 0}
    self._animPath = "Images/Enemies/RollingStone/RollingStone_Anim.anim"
    self._currAnimName = "idle"
end

function RollingStone:GetSpeed()
    return self._speed;
end

function RollingStone:Trigger()
    self._startMoving = true
    print("Triggered stone")
end

function RollingStone:Update()

    if(not self._startMoving) then
        print("Waiting")
        return
    end

    self._speed = self._defaultSpeed

    if(self._parentLevel == "NULL") then
        print(" RollingStone.lua : NO PARENT")
        return
    end

    if(self._modifiers["Ice"]) then
        self._speed = self._defaultSpeed * 3
    end

    local cos = math.cos(self._direction);
    local sin = math.sin(self._direction);
    
    if(math.abs(cos) < 0.00001) then
        cos = 0;
    end
    if(math.abs(sin) < 0.00001) then
        sin = 0;
    end
    
    self:SetPosX(self._posX + cos * self._speed);
    self:SetPosY(self._posY + sin * self._speed);

    if(self._posX == self._originalX and self._posY == self._originalY) then
        self:SetDirection(self._direction - math.pi)
        self._startMoving = false
        print("At home");
    end

end

function RollingStone:ContactWithStatic(other)
    print("Stone hit")
    if(other == "NULL") then
        self:SetDirection(self._direction + math.pi)
    end
end