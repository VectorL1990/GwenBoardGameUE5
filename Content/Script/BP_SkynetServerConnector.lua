--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--

local screen = require "Screen"
local sproto = require "sproto"
local proto = require "proto"
local socket = require "socket.core"

--host = sproto.new(proto.s2c):host "package"
--request = host:attach(sproto.new(proto.c2s))
fd = assert(socket.connect("192.168.19.57", 5678))

---@type BP_SkynetServerConnector_C
local M = UnLua.Class()

-- function M:Initialize(Initializer)
-- end

-- function M:UserConstructionScript()
-- end

function M:ReceiveBeginPlay()
end

-- function M:ReceiveEndPlay()
-- end

function M:ReceiveTick(DeltaSeconds)
end

-- function M:ReceiveAnyDamage(Damage, DamageType, InstigatedBy, DamageCauser)
-- end

-- function M:ReceiveActorBeginOverlap(OtherActor)
-- end

-- function M:ReceiveActorEndOverlap(OtherActor)
-- end

function M:ConnectToSkynetServer()
  local full_path = debug.getinfo(1,'S').source
  local path = full_path:match("(.*[/ \\])")
  local file, err = io.open(path .. "proto.c2s.sproto", "r")
  local t = file:read("*a")
  file:close()
  local host = sproto.parse(t):host "package"
  local file, err = io.open(path .. "proto.c2s.sproto", "r")
  local t = file:read "a"
  file:close()
  local request = host:attach(sproto.parse(t))
end

return M
