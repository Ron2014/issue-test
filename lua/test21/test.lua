if -1 then
    print("-1 is true")
else
    print("-1 is false")
end

local a = 1
local b = 2
local c = ( (a > b) and a ) or b
print(string.format("%d", c))
