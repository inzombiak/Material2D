require "Scripts/Enemies/IEnemy"
require "Scripts/Levels/ILevel"
require "math"

Follower =
{
	_name = "Follower",
	_id = -1,
    _isAlive = true,
	_health = 3,
	_damage = 1,
	_attackPattern = {},
    _modifiers = {},
    _speed = 0,
    _defaultSpeed = 0.3,
    _onFire = false,
    _originalX = 0,
    _originalY = 0,
    _targetX = 0,
    _targetY = 0,
    _wasFollowing = false,
}
--Follower.__index = Follower

function Follower:Create(newID, origX, origY)
	local b = {}
    setmetatable(b, Follower)
    setmetatable(Follower,  { __index = IEnemy})
    self.__index = self
	b._id = newID

	b:Init()

    b._originalX = origX
    b._originalY = origY
    b._targetX = origX
    b._targetY = origY
    
    return b
end

function Follower:Init()
	self._attackPattern[1] = {0, 0}
    self._animPath = "Images/Enemies/Follower/BouncingBomb_Anim.anim"
    self._currAnimName = "idle"
end

function Follower:GetSpeed()
    return self._speed;
end

function round(x)
    if x%2 ~= 0.5 then
        return math.floor(x+0.5)
    end
    return x-0.5
end

function Follower:Update()
    local speed = self._speed
    if(self._parentLevel == "NULL") then
        print("Follower.lua : NO PARENT")
        return
    end
    
    if(self._modifiers["Fire"]) then
        
        self._onFire = true
        self._speed = 2*self._defaultSpeed;
        
    elseif(not self._onFire) then
        local heroX = self._parentLevel:GetHeroX()
        local heroY = self._parentLevel:GetHeroY()
        
        if((self._posY <= heroY + 0.2 and self._posY >= heroY - 0.2 ) or
            (self._posX <= heroX + 0.2 and self._posX >= heroX - 0.2)) then
            self._targetX = round(heroX)
            self._targetY = round(heroY)
            if(not self._wasFollowing) then
                self._originalX = round(self._posX)
                self._originalY = round(self._posY)
                self._wasFollowing = true;
            end
            self._direction =  math.atan(heroY - self._posY, heroX - self._posX)
            self._speed = self._defaultSpeed;
            --print("Following")
        elseif((self._posX ~= self._originalX or self._posY ~= self._originalY) and self._wasFollowing) then
            self._targetX = self._originalX
            self._targetY = self._originalY
            self._wasFollowing = false
            self._speed = self._defaultSpeed;
           -- print("GoingHome")
        elseif (self._posX == self._targetX and self._posY == self._targetY) then
            self._speed = 0;
           -- print("At home")
            return
        end
    end
   
    if(not self._onFire) then
        self._direction = math.atan(self._targetY - self._posY, self._targetX - self._posX)
    end
    local clampedAngle = math.floor(self._direction / math.pi * 2)
    self._direction = clampedAngle * math.pi/2;
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

    if(math.abs(self._targetX - self._posX + self._targetY - self._posY) <= 0.3) then
        self._posX = self._targetX;
        self._posY = self._targetY;
    end
end

function Follower:ContactWithStatic(other)
    if(self._onFire) then
        self._onFire = false
        if(other ~= "NULL") then
            print(other:GetName())
            print(other:GetIsAlive())
            self._modifiers["Fire"] = false;
            other:ApplyModifier("Fire")
        end
    end
end