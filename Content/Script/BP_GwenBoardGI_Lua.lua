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
-- local socket = require "socket.core"

local socket = require "ClientSocket"
local message = require "ClientMessage"


---@type BP_GwenBoardGI_C
local game_gi = UnLua.Class()

local event = {}
local login_success = false

function game_gi:ConnectToSkynetServer()
    local full_path = debug.getinfo(1,'S').source
    local path = full_path:match("(.*[/ \\])")
    message.register(path.."proto")
    message.peer("127.0.0.1", 5678)
    message.connect()
  
    message.bind({}, event)
end
  
function game_gi:SendRequest(rpc_name, msg_table)
    message.request(rpc_name, msg_table)
end
  
function game_gi:ReceiveRoomMsg(msg)
    if msg == "" then
    end
end

function game_gi:try_connect_server(id, passwd)
    SendRequest("login", {user_id = id, user_pass = passwd})
end

function game_gi:play_card(card_info, grid_nb)
end

return game_gi