//
//  Header.h
//  Material2D
//
//  Created by Zareh on 8/30/16.
//  Copyright © 2016 Zareh. All rights reserved.
//

#ifndef Header_h
#define Header_h

#include <map>
#include <string>
#include <memory>

enum MaterialType
{
    Fire,
    Ice,
    Glass,
    Sticky,
    Stone,
    Default,
};

class LuaObject;
class Entity;
class IMaterial
{
public:
    IMaterial()
    {
        m_type = Default;
    }
    
    IMaterial(MaterialType type);
    void OnCollide(std::shared_ptr<LuaObject> target);
    const MaterialType& GetType() const
    {
        return m_type;
    }
    const std::string& GetColor() const
    {
        return m_color;
    }
private:
    MaterialType m_type = Default;
    std::string m_color;
};

class MaterialManager
{
public:
    static void LoadAllMaterials();
    static std::shared_ptr<IMaterial> GetMaterialByType(const MaterialType& type);
private:
    MaterialManager() {};
    
    static std::map<MaterialType, std::shared_ptr<IMaterial>> m_materials;
};

#endif /* Header_h */
