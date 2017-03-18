IEnemy =
{
	_name = "NULL",
    _isAlive = true,
	_id = -1,
	_health = -1,
	_damage = -1,
	_posX = -1,
	_posY = -1,
	_attackPattern = {},
	_color = {},
	_currAnimName = "NULL",
	_animPath = "NULL",
    _modifiers = {},
    _speed = 0,
    _direction = 0,
    _parentLevel = "NULL"
}

IEnemy.__index = IEnemy

function IEnemy:Create(newID)
	local b = {}
    setmetatable(b, IEnemy)
	b.__index = b
    self._isAlive = true
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

function IEnemy:SetID(newId)
    self._id = newId
end

function IEnemy:GetID()
	return self._id
end

function IEnemy:GetHealth()
	return self._health
end

function IEnemy:SetIsAlive(val)
    self._isAlive = val
end

function IEnemy:Trigger()
    print("NO TRIGGER FUNCTION FOR " .. self:GetName())
end

function IEnemy:GetIsAlive()
    return self._isAlive
end

function IEnemy:SetHealth(val)
	self._health = val
end

function IEnemy:SetDirection(val)
    self._direction = val
end
function IEnemy:GetDamage()
	return self._damage
end

function IEnemy:SetPosX(newX)
    if(self._parentlevel ~= "NULL") then
        self._parentLevel:EnemyMoved(self._posX, self._posY, newX, self._posY, self._id)
    end
	self._posX = newX
end

function IEnemy:GetPosX()
	return self._posX
end

function IEnemy:SetPosY(newY)
    if(self._parentlevel ~= "NULL") then
        self._parentLevel:EnemyMoved(self._posX, self._posY, self._posX, newY, self._id)
    end
	self._posY = newY
end

function IEnemy:GetPosY()
	return self._posY
end

function IEnemy:SetSpeed(newSpeed)
    self._speed = newSpeed
end

function IEnemy:GetSpeed()
    return self._speed;
end

function IEnemy:SetDirection(newDir)
    self._direction = newDir;
end

function IEnemy:GetDirection()
    return self._direction
end

function IEnemy:GetAttackPattern()
	return self._attackPattern
end

function IEnemy:ContactWithStatic(other)
	print("NO CONTACT FUNCTION")
end

function IEnemy:Kill()
    self._isAlive = false
	print("Default Enemy Kill")
end

function IEnemy:GetCurrentAnimationName()
	return self._currAnimName
end

function IEnemy:GetAnimationPath()
	return self._animPath
end

function IEnemy:ApplyModifier(mod)
print(self:GetName() .. " has " .. mod)
    self._modifiers[mod] = true
end

function IEnemy:SetParentLevel(level)
    self._parentLevel = level;
end