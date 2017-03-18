require "Scripts/Enemies/IEnemy"
require "Scripts/Levels/ILevel"
require "math"

Breakable =
{
    _name = "Breakable",
    _id = -1,
    _health = 3,
    _damage = 10,
    _attackPattern = {},
    _breakingSpeed = 2,
    _modifiers = {},
}

setmetatable(Breakable,  { __index = IEnemy})
Breakable.__index = Breakable

function Breakable:Create(newID, origX, origY)
    
    local b = {}
    
    setmetatable(b, Breakable)

    b.__index = b
    
    b._id = newID;
    b:Init()
    return b
    
end

function Breakable:Init()
    self._attackPattern[1] = {0, 0}
    self._animPath = "Images/Enemies/Breakable/Breakable_Anim.anim"
    self._currAnimName = "idle"
    self._isAlive = true
end

function Breakable:Update()
end

function Breakable:Kill()
    print(self:GetID())
    if(self._parentLevel == "NULL") then
        print("Breakable.lua : NO PARENT")
        return
    end
    
    if (not self:GetIsAlive()) then
        print(self._name .. " " .. self._id .. " is already dead")
        return
    end
    
    print(self:GetID() .. ", " .. "exploded")
    self._parentLevel:KillEnemyAtPos(self._posX,  self._posY)
    self:SetIsAlive(false)

end

function Breakable:ContactWithStatic(other)
    print("Might break")
    if(other:GetSpeed() > self._breakingSpeed) then
        self:Kill()
        print("Broke")
    else
        other:ContactWithStatic("NULL")
        print("Didnt break")
    end
end