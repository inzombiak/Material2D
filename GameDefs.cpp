//
//  GameDefs.cpp
//  Material2D
//
//  Created by Zareh on 8/30/16.
//  Copyright © 2016 Zareh. All rights reserved.
//

#include <stdio.h>
#include "GameDefs.h"
#include "LuaWrapper/LuaWrapper.h"
#include "LuaWrapper/LUAObject.h"

static std::map<MaterialType, std::string> MATERIAL_TO_FILEPATH = {{MaterialType::Fire, "Scripts/Materials/Fire.lua"}, {MaterialType::Ice, "Scripts/Materials/Ice.lua"}, {MaterialType::Sticky, "Scripts/Materials/Sticky.lua"}};
static std::map<MaterialType, std::string> MATERIAL_TO_STD_STRING = {{MaterialType::Fire, "Mat_Fire"}, {MaterialType::Ice, "Mat_Ice"}, {MaterialType::Sticky, "Mat_Sticky"}};
static std::map<MaterialType, std::string> MATERIAL_TO_COLOR = {{MaterialType::Fire, "Red"}, {MaterialType::Ice, "Blue"}, {MaterialType::Ice, "Green"}};

IMaterial::IMaterial(MaterialType type)
{
    m_type = type;
    LuaWrapper::GetInstance().LoadScript(MATERIAL_TO_STD_STRING [type], MATERIAL_TO_FILEPATH[type]);
    auto it = MATERIAL_TO_COLOR.find(type);
    if(it == MATERIAL_TO_COLOR.end())
        m_color = "";
    else
        m_color = it->second;
}

void IMaterial::OnCollide(std::shared_ptr<LuaObject> target)
{
    LuaWrapper::GetInstance().RunFunction<void>(MATERIAL_TO_STD_STRING [m_type], FuncNameInfo("OnContact", "thisMat"), LuaCallDispatcher::LuaRefID(target->GetLuaRef()));
}

void MaterialManager::LoadAllMaterials()
{
    m_materials[MaterialType::Ice] = std::make_shared<IMaterial>(IMaterial(MaterialType::Ice));
    m_materials[MaterialType::Fire] = std::make_shared<IMaterial>(IMaterial(MaterialType::Fire));
}
std::shared_ptr<IMaterial> MaterialManager::GetMaterialByType(const MaterialType&  type)
{
    auto it = m_materials.find(type);
    if(it == m_materials.end())
        return std::shared_ptr<IMaterial>(NULL);
    else
        return it->second;
}

std::map<MaterialType, std::shared_ptr<IMaterial>> MaterialManager::m_materials;