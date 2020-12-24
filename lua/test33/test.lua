local function swap(t, i, j)
    t[i], t[j] = t[j], t[i]
end

local function printt(t)
    local str = {}
    for k, v in ipairs(t) do
        str[#str + 1] = tostring(v)
    end
    print(table.concat(str))
end

local function full_permutation(t, i, len)
    if i == len then
        printt(t)
        return
    end
    for j = i, len do
        swap(t, i, j)
        full_permutation(t, i+1, len)
        swap(t, i, j)
    end
end

local t = {1, 2, 3}
full_permutation(t, 1, 3)