-- skillNoteMgr.lua
-- 技能效果管理器

local class = require 'middleclass.middleclass'
local SkillNoteMgr = class("SkillNoteMgr")

function SkillNoteMgr:initialize(...)
    self.__count = 0
    self.__instances = {}
end

function SkillNoteMgr:addBuff(node)
    self.__instances[node:id()] = node
    self.__count = self.__count + 1
end

function SkillNoteMgr:removeBuff(id)
    local node = self.__instances[id]
    if not node then return end
    self.__instances[id] = nil
    self.__count = self.__count - 1
end

function SkillNoteMgr:update()
    for id, node in pairs(self.__instances) do
        node:update()
    end
end