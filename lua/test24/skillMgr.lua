-- skillMgr.lua
-- 技能管理器

local class = require 'middleclass.middleclass'
local SkillMgr = class('SkillMgr')

local INSTANCE_ID = 0

-- count: 初始化技能数目
function SkillMgr:initialize(count)
    self.__count = 0
    self.__instances = {}
end

-- frame update
function SkillMgr:update()
    for id, skill in pairs(self.__instances) do
        skill:update()
    end
end

return SkillMgr