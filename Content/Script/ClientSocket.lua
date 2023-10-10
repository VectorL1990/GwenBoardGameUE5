local lua_socket = require "socket.core"

local socket = {}
local fd
local message

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

function socket.innerRead(ti)
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
			fd:settimeout(0)
			local p, status = fd:receive("*a")
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
			local p, status = fd:recv()
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
