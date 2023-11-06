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

local full_path = debug.getinfo(1,'S').source
local path = full_path:match("(.*[/ \\])")
print("test path is: ", path)
local file, err = io.open(path .. "proto.c2s.sproto", "r")
--local file = io.open("proto.s2c", "r")
local t = file:read("*a")
file:close()
local host = sproto.parse(t):host "package"
local file, err = io.open(path .. "proto.c2s.sproto", "r")
local t = file:read "a"
file:close()
local request = host:attach(sproto.parse(t))


--host = sproto.new(proto.s2c):host "package"
--request = host:attach(sproto.new(proto.c2s))
fd = assert(socket.connect("192.168.19.57", 5678))

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
