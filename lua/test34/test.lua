local function isarray(t)
    if type(t) ~= "table" then return false end
    
    local max_index = 0
    for k, v in pairs(t) do
        if type(k) ~= "number" then return false end    -- 数组下标
        if k ~= math.floor(k) then return false end     -- 数组下标是整数
        if k <= 0 then return false end                 -- 数组下标从1开始
        if k > max_index then max_index = k end
    end
    
    -- for case 3
    -- {[1]=1, [2]=2, [4]=4, [8]=8, [16]=16}
    for i=1, max_index do
        if t[i] == nil then return false end            -- 数组必须是连续非nil值
    end

    return true
end

print(isarray(1))
print(isarray("abc"))
print(isarray({[1]=1, [2]=2, [4]=4, [8]=8, [16]=16}))       -- 非常特殊
print(isarray({1,2, a='a', b='b'}))
print(isarray({1,2,3,4,5}))
print(isarray({'a', 'b', 'c', 'd', 'e'}))