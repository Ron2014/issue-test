-- battleMgr.lua
-- 关卡管理器

local Battle = require "battle"

local class = require 'middleclass.middleclass'
local BattleMgr = class('BattleMgr')

local INSTANCE_ID = 0

-- count: 初始化关卡数目
function BattleMgr:initialize(count)
    self.__count = 0
    self.__instances = {}

    for cid in 1,count do
        self:createOne(cid)
    end
end

-- frame update
function BattleMgr:update()
    for id, battle in pairs(self.__instances) do
        battle:update()
    end
end

function BattleMgr:createOne(cid)
    local id = INSTANCE_ID
    local battle = Battle:new(id, cid)
    self.__instances[id] = battle

    INSTANCE_ID = id + 1
    self.__count = self.__count + 1

    return battle
end

function BattleMgr:destroy(id)
    local battle = self.__instances[id]
    if not battle then return end
    battle:destroy()
    self.__instances[id] = nil
    self.__count = self.__count - 1
end

return BattleMgr