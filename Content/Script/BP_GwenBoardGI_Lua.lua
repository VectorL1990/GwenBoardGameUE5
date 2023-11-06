--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--
local screen = require "Screen"
local sproto = require "sproto"
--local proto = require "proto"
-- local socket = require "socket.core"

local socket = require "ClientSocket"
local message = require "ClientMessage"


---@type BP_GwenBoardGI_C
local game_gi = UnLua.Class()

local event = {}
local login_success = false
local host
local request
local fd
local session = 0
local registerInit = false

function game_gi.CallCppConnectServer()
    self:ConnectServer("192.168.19.57", 5678, 1)
end

function game_gi.RegisterInit()
    if not registerInit then
        local full_path = debug.getinfo(1,'S').source
        local path = full_path:match("(.*[/ \\])")
        message.register(path.."proto")
    end
end

function game_gi.ConnectToSkynetServer()
    --[[
    local full_path = debug.getinfo(1,'S').source
    local path = full_path:match("(.*[/ \\])")

    local s2c_file = assert(io.open(path .. "proto.s2c.sproto"))
	local s2c_content = s2c_file:read "a"
	s2c_file:close()
	host = sproto.parse(s2c_content):host "package"
	local c2s_file = assert(io.open(path .. "proto.c2s.sproto"))
	local c2s_content = c2s_file:read "a"
	c2s_file:close()
	request = host:attach(sproto.parse(c2s_content))
    fd = assert(socket.connect("192.168.19.57", 5678))
    ]]

    local full_path = debug.getinfo(1,'S').source
    local path = full_path:match("(.*[/ \\])")
    message.register(path.."proto")
    message.peer("192.168.19.57", 5678)
    message.connect()
    screen.Print("try to connect to skynet server")

    message.bind({}, event)
end

function game_gi.SendRequest(rpc_name, msg_table)
    --[[
    session = session + 1
    print("rpc name is: ", rpc_name)
    local str = request(rpc_name, msg_table, session)
    local package = string.pack(">s2", str)
    fd:send(package)
    ]]
    message.request(rpc_name, msg_table)
end

function game_gi.UpdateMessage()
    message.update()
end
  
function game_gi.ReceiveRoomMsg(msg)
    if msg == "" then
    end
end

function game_gi.SecondThreadCallLua()
    screen.Print("trigger game_gi.SecondThreadCallLua")
    --message.update()
end

function game_gi.TestCallLua()
    screen.Print("trigger TestCallLua")
end

function game_gi.play_card(card_info, grid_nb)
end

function event:signup(req, resp)
    screen.Print("111111111111")
    if resp.ok then
        screen.Print("aaaaaaaaaa")
    else
        screen.Print("bbbbbbbb")
    end
end


return game_gi