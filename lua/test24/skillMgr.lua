-- skillMgr.lua
-- 技能管理器

local Skill = require "skill"

local class = require 'middleclass.middleclass'
local SkillMgr = class('SkillMgr')

local INSTANCE_ID = 0

-- count: 初始化技能数目
function SkillMgr:initialize(count)
    self.__count = 0
    self.__instances = {}

    for i in 1,count do
        self:createOne()
    end
end

-- frame update
function SkillMgr:update()
    for id, battle in pairs(self.__instances) do
        battle:update()
    end
end

function SkillMgr:createOne()
    local id = INSTANCE_ID
    local battle = Battle:new(id)
    self.__instances[id] = battle

    INSTANCE_ID = id + 1
    self.__count = self.__count + 1

    return battle
end

function SkillMgr:destroy(id)
    local battle = self.__instances[id]
    if not battle then return end
    battle:destroy()
    self.__instances[id] = nil
    self.__count = self.__count - 1
end

return SkillMgr