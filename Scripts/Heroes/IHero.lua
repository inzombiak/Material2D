IHero = 
{
	_name = "NULL",
	_hp = -1,
	_sglClickAblPath = "NULL",
	_dblClickAblPath = "NULL",
	_spritesheetPath = "NULL",
	_animPath = "NULL",
	_animMsgTable = {},
}

function IHero:GetName()
	return self._name
end

function IHero:GetHP()
	return self._hp
end

function IHero:GetSglClickPath()
	return self._sglClickAblPath
end

function IHero:GetDblClickPath()
	return self._dblClickAblPath
end

function IHero:GetSpritesheetPath()
	return self._spritesheetPath
end

function IHero:GetAnimationPath()
	return self._animPath
end

function IHero:GetAnimationNameByMsg(message)
	if (self._animMsgTable[message] == nil) then
		print("INVALID MESSAGE, NO ANIMATION")
	else
		return self._animMsgTable[message]
	end
end