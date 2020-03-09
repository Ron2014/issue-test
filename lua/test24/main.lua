local TIME_UNIT = 10 -- millisecond, interval for frame logic
local FLAG_EXIT = false

local BattleMgr = require 'battleMgr'
local battleMgr = BattleMgr(100)

-- logic finished in ONE frame
function loopFrame()
    battleMgr:update()
end

function main()
    local last_time = 0
    local current_time = 0

    while true do
        current_time = os.clock()
        while current_time - last_time > TIME_UNIT do
            loopFrame()
            last_time = last_time + TIME_UNIT
        end
        if FLAG_EXIT then
            break
        end
    end
end