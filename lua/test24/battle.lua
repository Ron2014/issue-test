-- battle.lua
-- 关卡/战场

local SkillMgr = require "skillMgr"
local SkillNodeMgr = require "skillNodeMgr"
local Terrain = require "terrain"

local class = require 'middleclass.middleclass'
local Battle = class("Battle")

function Battle:initialize(id, cid)
    self.__id = id
    self.__cid = cid
    self.__entity = {}
    self.__terrain = Terrain:new()
    self.__skillMgr = SkillMgr:new()
    self.__skillNodeMgr = SkillNodeMgr:new()

    self:initTerrain()
end

function Battle:id()
    return self.__id
end

function Battle:cid()
    return self.__cid
end

-- frame update
function Battle:update()
    self.__skillMgr:update()
    self.__skillNodeMgr:update()
end

function Battle:addEntity(entity)
    self.__entity[entity:id()] = entity
    -- 根据 entity 携带的技能更新至 self.__skillMgr
end

function Battle:removeEntity(id)
    local entity = self.__entity[id]
    if not entity then return end

    -- 从 self.__skillMgr 移除与 entity 相关的技能
    
    self.__entity[id] = nil
end

function Battle:destroy()
end

return Battle