require "Scripts/Materials/IMaterial"
require "Scripts/Enemies/IEnemy"

Fire =
{
    _name = "Fire"
}

Fire.__index = Fire;

function Fire:Create()
    local b = {}
    setmetatable(b, Fire)
    setmetatable(Fire,  { __index = IMaterial})
    return b
end

function Fire:OnContact(otherBody)
    otherBody:ApplyModifier("Fire");
end

thisMat = Fire:Create()