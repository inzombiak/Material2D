require "Scripts/Enemies/IEnemy"
require "Scripts/Levels/ILevel"
require "math"

Explosive =
{
	_name = "Explosive",
	_health = 3,
	_damage = 10,
	_attackPattern = {},
    _speed = 0,
    _modifiers = {},
}

setmetatable(Explosive,  {__index = IEnemy})
Explosive.__index = Explosive

function Explosive:Create(newID, origX, origY)
    
	local b = {}
    setmetatable(b, Explosive)

    b.__index = b
    b._id = newID
	b:Init()
    print(self)
    return b
    
end

function Explosive:Init()
	self._attackPattern[1] = {0, 0}
    self._animPath = "Images/Enemies/Explosive/FireSpinner_Anim.anim"
    self._currAnimName = "idle"
    self._isAlive = true
end

function Explosive:GetSpeed()
    return self._speed;
end

function Explosive:Update()
    if(self._modifiers["Fire"] == true) then
        print("I'm on fire")
    self._parentLevel:KillEnemyAtPos(self:GetPosX(), self:GetPosY());
    end
    
end

function Explosive:Kill()
    if(self._parentLevel == "NULL") then
        print("Explosive.lua : NO PARENT")
        return
    end

    if (not self:GetIsAlive()) then
print(self._name .. " " .. self:GetID() .. " is already dead")
        return
    end
    
    print(self:GetID() .. ", " .. "exploded")
    self:SetIsAlive(false)
    self._parentLevel:KillEnemyAtPos(self._posX,     self._posY)
    self._parentLevel:KillEnemyAtPos(self._posX,     self._posY - 1)
    self._parentLevel:KillEnemyAtPos(self._posX - 1, self._posY)
    self._parentLevel:KillEnemyAtPos(self._posX + 1, self._posY)
    self._parentLevel:KillEnemyAtPos(self._posX,     self._posY + 1)
    self._modifiers["Fire"] = false
    
end

function Explosive:ContactWithStatic(other)
    if(self._onFire) then
        self._onFire = false
    end
end