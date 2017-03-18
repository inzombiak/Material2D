IMaterial =
{
	_name = "NULL",
	
}

function IMaterial:Create()
	local b = {}
    setmetatable(b, self)
	b.__index = self
    return b
end

function IMaterial:GetName()
    print("hi")
	return self._name
end

function IMaterial:OnContact(otherBody)
    return "NO CONTACT FUNCTION FOR " + self._name
end
