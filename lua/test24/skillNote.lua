-- skillNote.lua
-- 技能效果节点

local class = require 'middleclass.middleclass'
local SkillNote = class("SkillNote")

function SkillNote:initialize(cid, args)
    self.__cid = cid
    self.__args = args
end

function SkillNote:args()
    return self.__args
end

function SkillNote:cid()
    return self.__cid
end

function SkillNote:update()
end