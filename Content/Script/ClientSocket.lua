local lua_socket = require "socket.core"
local screen = require "Screen"

local socket = {}
local fd
local message

local read_coroutine = coroutine.create(function()
	while true do
		screen.Print("[[[[[[[[[[[[[[")
		
		local ok, msg, n = pcall(string.unpack, ">s2", message)
		screen.Print("[[[[[[[[[[[[[[1111111111")
		if not ok then
			--[[
			local rd = lua_socket.select({fd}, 0) 
			screen.Print("[[[[[[[[[[[[[[222222222222")
			if not rd then
				screen.Print("pppppppppppp")
				return nil
			end
			if next(rd) == nil then
				screen.Print("pppppppppppp1111111")
				return nil
			end]]
			
			local p, status = fd:receive(1024)
			
			if not p then
				error(socket.error)
			end
			message = message .. p
		else
			message = message:sub(n)
			screen.Print("pppppppppppp2222222222")
			return msg
		end
		screen.Print("pppppppppppp3333333333")
		coroutine.yield()
	end
end)

socket.error = setmetatable({},{ __tostring = function() return "[socket error]" end })

function socket.connect(addr, port)
    assert(fd == nil)
    fd = lua_socket.connect(addr, port)
    if fd == nil then
        error(socket.error)
    end

    lua_socket.select(nil, {fd})
    --local ok, errmsg = fd:status()
    --if not ok then
    --    error(socket.error)
    --end

    message = ""
end

function socket.isconnect(ti)
    local rd, wt = lua_socket.select(nil, {fd}, ti)
    return next(wt) ~= nil
end

function socket.close()
    fd:close()
    fd = nil
    message = nil
end

function socket.trigger_read()
	local status, msg = coroutine.resume(read_coroutine)
	if not status then
		return nil
	else
		return msg
	end
end




function socket.read(ti)
	while true do
		local ok, msg, n = pcall(string.unpack, ">s2", message)
		if not ok then
			local rd = lua_socket.select({fd}, ti) 
			if not rd then
				return nil
			end
			if next(rd) == nil then
				return nil
			end
			local p, status = fd:receive("*l")
			if not p then
				error(socket.error)
			end
			message = message .. p
		else
			message = message:sub(n)
			return msg
		end
	end
end

function socket.write(msg)
	local pack = string.pack(">s2", msg)
	repeat
		local bytes = fd:send(pack)
		if not bytes then
			error(socket.error)
		end
		pack = pack:sub(bytes+1)
	until pack == ""
end

return socket
