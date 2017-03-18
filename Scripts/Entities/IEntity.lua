IEnemy =
{
_name = "NULL",
_id = -1,
_health = -1,
_damage = -1,
_posX = -1,
_posY = -1,
_attackPattern = {},
_color = {},
_currAnimName = "NULL",
_animPath = "NULL"

}

function IEnemy:Create(newID)
local b = {}
setmetatable(b, self)
b.__index = self
return b
end

function IEnemy:Init()
print("INVALID ENEMY CANNOT INIT")
end

function IEnemy:Update()
print("INVALID ENEMY CANNOT UPDATE");
end

function IEnemy:GetName()
return self._name
end

function IEnemy:GetID()
return self._id
end

function IEnemy:GetHealth()
return self._health
end

function IEnemy:SetHealth(val)
self._health = val
end

function IEnemy:GetDamage()
return self._damage
end

function IEnemy:SetPosX(newX)
self._posX = newX
end

function IEnemy:GetPosX()
return self._posX
end

function IEnemy:SetPosY(newY)
self._posY = newY
end

function IEnemy:GetPosY()
return self._posY
end

function IEnemy:GetAttackPattern()
return self._attackPattern
end

function IEnemy:ContactWithStatic()
print("NO CONTACT FUNCTION")
end

function IEnemy:ContactWithStatic()
print("NO CONTACT FUNCTION")
end

function IEnemy:Kill()
print("EMPTY ENEMY")
end

function IEnemy:GetCurrentAnimationName()
return self._currAnimName
end

function IEnemy:GetAnimationPath()
return self._animPath
end