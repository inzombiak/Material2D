require "Scripts/Enemies/IEnemy"
require "Scripts/Levels/ILevel"
require "math"

Trigger =
{
    _name = "Trigger",
    _id = -1,
    _triggered = false,
    _connectedObj = nil
}

function Trigger:Create(newID, origX, origY)

    local b = {}

    setmetatable(b, Trigger)
    setmetatable(Trigger,  { __index = IEnemy})
    self.__index = self

    b._id = newID
    b:Init()
    return b
end

function Trigger:ConnectTo(other)
    self._connectedObj = other
end

function Trigger:Init()
    
end

function Trigger:Update()

    local heroX = self._parentLevel:GetHeroX()
    local heroY = self._parentLevel:GetHeroY()
    if((self._posX == heroX and self._posY == heroY) and not self._triggered) then
        self._connectedObj:Trigger()
        self._triggered = true
    elseif (not(self._posX == heroX and self._posY == heroY)) then
        self._triggered = false
    end

end
