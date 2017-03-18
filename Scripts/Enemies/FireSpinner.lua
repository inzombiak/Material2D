require "Scripts/Enemies/IEnemy"

FireSpinner =
{
	_name = "FireSpinner",
	_id = -1,
	_health = 3,
	_damage = 1,
	_attackPattern = {},
	
	_direction = 0
}

FireSpinner.__index = FireSpinner

function FireSpinner:Create(newID)
	local b = {}
    setmetatable(b, FireSpinner)
	setmetatable(FireSpinner,  { __index = IEnemy})
	self._id = newID
	self:Init()
	print("Firespinner ready")
    return b
end

function FireSpinner:Init()
	self._attackPattern[1] = {1, 0}
	self._attackPattern[2] = {2, 0}
	
	self._animPath = "Images/Enemies/FireSpinner/FireSpinner_Anim.anim"
	self._currAnimName = "right"
end

function FireSpinner:GetAttackPattern()
    
	local result = {}
	
	local dirSin = math.sin(self._direction)
	local dirCos = math.cos(self._direction)
	
	if(math.abs(dirSin) < 0.00001) then
		dirSin = 0
	end
	if(math.abs(dirCos) < 0.00001) then
		dirCos = 0
	end
    
	dirSin = dirSin * -1
	for i = 1, #self._attackPattern do
		local apX = self._attackPattern[i][1] * dirCos - self._attackPattern[i][2] * dirSin
		local apY = self._attackPattern[i][1] * dirSin + self._attackPattern[i][2] * dirCos
		result[i] = {apX, apY}
	end
	
	return result
	
end

function FireSpinner:Update()
	
	self._direction = self._direction + math.pi/2

	if(self._direction >= 2*math.pi) then
		self._direction = self._direction - 2*math.pi
	end
	
	if(self._direction == 0) then
		self._currAnimName = "right"
	elseif (self._direction == math.pi/2) then
		self._currAnimName = "up"
	elseif(self._direction == math.pi) then
		self._currAnimName = "left"
	elseif(self._direction == 1.5 * math.pi) then
		self._currAnimName = "down"
	else 
		self._currAnimName = "down"
	end
	
end
