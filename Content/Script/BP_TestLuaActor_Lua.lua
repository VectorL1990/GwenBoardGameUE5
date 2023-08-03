--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

local screen = require "Screen"

---@type BP_TestLuaActor_C
local M = UnLua.Class()

function M:Initialize(Initializer)
end

-- function M:UserConstructionScript()
-- end

function M:ReceiveBeginPlay()
  local msg = "test"
  screen.Print(msg)
end

-- function M:ReceiveEndPlay()
-- end

function M:ReceiveTick(DeltaSeconds)
  local msg = "test"
  screen.Print(msg)
end

-- function M:ReceiveAnyDamage(Damage, DamageType, InstigatedBy, DamageCauser)
-- end

-- function M:ReceiveActorBeginOverlap(OtherActor)
-- end

-- function M:ReceiveActorEndOverlap(OtherActor)
-- end

return M
