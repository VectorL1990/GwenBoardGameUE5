local screen = require "Screen"
local sproto = require "sproto"
local proto = require "proto"
-- local socket = require "socket.core"

local socket = require "ClientSocket"
local message = require "ClientMessage"

local skynet_connector = UnLua.Class()

local event = {}
local login_success = false

function skynet_connector:ConnectToSkynetServer()
  local full_path = debug.getinfo(1,'S').source
  local path = full_path:match("(.*[/ \\])")
  message.register(path.."proto")
  message.peer("127.0.0.1", 5678)
  message.connect()

  message.bind({}, event)
end

function skynet_connector:SendRequest(rpc_name, msg_table)
  message.request(rpc_name, msg_table)
end

function skynet_connector:ReceiveRoomMsg(msg)
  
end

return skynet_connector
