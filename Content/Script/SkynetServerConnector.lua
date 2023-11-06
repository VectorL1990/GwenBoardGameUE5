local screen = require "Screen"
local sproto = require "sproto"
local proto = require "proto"
-- local socket = require "socket.core"

local socket = require "socket.core"
local message = require "ClientMessage"

local skynet_connector = UnLua.Class()

local event = {}
local login_success = false
local host
local request
local fd

function skynet_connector:RegisterProto()
  local full_path = debug.getinfo(1,'S').source
  local path = full_path:match("(.*[/ \\])")

  local s2c_file = assert(io.open(path .. ".s2c.sproto"))
	local s2c_content = s2c_file:read "a"
	s2c_file:close()
	host = sproto.parse(s2c_content):host "package"
	local c2s_file = assert(io.open(path .. ".c2s.sproto"))
	local c2s_content = c2s_file:read "a"
	c2s_file:close()
	request = host:attach(sproto.parse(c2s_content))
end

function skynet_connector:ConnectToSkynetServer()
  --[[
  local full_path = debug.getinfo(1,'S').source
  local path = full_path:match("(.*[/ \\])")
  message.register(path.."proto")
  message.peer("127.0.0.1", 5678)
  message.connect()

  message.bind({}, event)
  ]]
  fd = assert(socket.connect("192.168.19.57", 5678))
end

function skynet_connector:SendRequest(rpc_name, msg_table)
  local package = string.pack(">s2", msg_table)
  fd:send(package)
end

function skynet_connector:ReceiveRoomMsg(msg)
  
end

return skynet_connector
