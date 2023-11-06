--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

local screen = require "Screen"

---@type BP_CoreCardGameManager_C
local core_game_manager = UnLua.Class()

function core_game_manager:Initialize(Initializer)
end

-- function core_game_manager:UserConstructionScript()
-- end

function core_game_manager:ReceiveBeginPlay()
end

-- function core_game_manager:ReceiveEndPlay()
-- end

function core_game_manager:ReceiveTick(DeltaSeconds)
end

function core_game_manager:ConnectToSkynetServer()
end

-- function core_game_manager:ReceiveAnyDamage(Damage, DamageType, InstigatedBy, DamageCauser)
-- end

-- function core_game_manager:ReceiveActorBeginOverlap(OtherActor)
-- end

-- function core_game_manager:ReceiveActorEndOverlap(OtherActor)
-- end

return core_game_manager
