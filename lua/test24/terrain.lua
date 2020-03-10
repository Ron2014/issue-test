-- terrain.lua
-- 地形数据

local class = require 'middleclass.middleclass'

local GridInfo = class("GridInfo")

function GridInfo:initialize(point, type)
    self.__point = point
    self.__type = type
end

function GridInfo:type()
    return self.__type
end

function GridInfo:x()
    return self.__point:x()
end

function GridInfo:y()
    return self.__point:y()
end

local Terrain = class("Terrain")

local GRID_TYPE = {
    Walkable = 0,           -- 可行走
    Walkable_Non = 1,       -- 隔离
    StartPoint = 2,         -- 出生点
    Flyable = 3,            -- 飞行
    EndPoint = 4,           -- 终点
    HeroPoint = 5,          -- 英雄放置点
}

local LINE_WIDTH = 1
local LINE_LENGTH = 1

function Terrain:initialize(cid)
    self.__gridInfo = {}
    self:loadGridInfo(cid)
end

function Terrain:loadGridInfo(cid)
    self.__cid = cid
end

function Terrain:getType(point)
    local i = math.floor(point:x() / LINE_WIDTH)
    local j = math.floor(point:y() / LINE_LENGTH)

    if self.__gridInfo[i] and self.__gridInfo[i][j] then
        return self.__gridInfo[i][j]:type()
    end
end