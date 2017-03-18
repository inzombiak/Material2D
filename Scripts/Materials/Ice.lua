require "Scripts/Materials/IMaterial"
require "Scripts/Enemies/IEnemy"

Ice =
{
	_name = "Ice"
}

Ice.__index = Ice;

function Ice:Create()
    local b = {}
    setmetatable(b, Ice)
    setmetatable(Ice,  { __index = IMaterial})
    return b
end

function Ice:OnContact(otherBody)
   otherBody:ApplyModifier("Ice");
end

thisMat = Ice:Create()