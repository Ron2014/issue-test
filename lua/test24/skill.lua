-- skill.lua
-- 技能实例

local class = require 'middleclass.middleclass'
local Skill = class("Skill")

local SKILL_STATE = {
    DEFAULT = 0,        -- CD状态
    CAST = 1,           -- 吟唱
    RELEASE = 2,        -- 施放
    FINAL = 3,          -- 收尾
}

function Skill:initialize(id, cid)
    self.__id = id
    self.__cid = cid
    self.__state = SKILL_STATE.DEFAULT
end

function Skill:id()
    return self.__id
end

function Skill:cid()
    return self.__cid
end

-- frame update
function Skill:update()
end

return Skill