require "Scripts/Materials/IMaterial"
require "Scripts/Enemies/IEnemy"

Sticky =
{
    _name = "Sticky"
}

function Sticky:Create()
    local b = {}
    setmetatable(b, Sticky)
    setmetatable(Sticky,  { __index = IMaterial})
    return b
end

Sticky.__index = Sticky;

function Fire:OnContact(otherBody)
    otherBody:ApplyModifier("Sticky");
end

thisMat = Sticky:Create()