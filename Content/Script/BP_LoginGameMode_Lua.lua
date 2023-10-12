--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--
local screen = require "Screen"
local game_gi = require "BP_GwenBoardGI_Lua"

---@type BP_LoginGameMode_C
local login_gamemode = UnLua.Class()

function login_gamemode:Initialize(Initializer)

end

-- function M:UserConstructionScript()
-- end

function login_gamemode:ReceiveBeginPlay()
  --game_gi.ConnectToSkynetServer()
  --game_gi.SendRequest("signup", {userid = "Alice"})
  --self:AfterBeginPlay()
  --game_gi.CallCppConnectServer()
end

-- function M:ReceiveEndPlay()
-- end

function login_gamemode:ReceiveTick(DeltaSeconds)
  game_gi.UpdateMessage()
end

function login_gamemode:CallLuaBeginPlay()
end

function login_gamemode:CallLuaTick(dT)
  --game_gi.UpdateMessage()
end

-- function M:ReceiveAnyDamage(Damage, DamageType, InstigatedBy, DamageCauser)
-- end

-- function M:ReceiveActorBeginOverlap(OtherActor)
-- end

-- function M:ReceiveActorEndOverlap(OtherActor)
-- end

return login_gamemode
