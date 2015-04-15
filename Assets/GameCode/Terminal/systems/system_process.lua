--[[
	Net
	--
	This handles the connection to remote computer systems.
]]

--require "terminal"

local process_sys = {}
process_sys.__index = process_sys


function process_sys.new(process_id)

	local self setmetatable({}, process_sys)

	self.id = process_id

	return self

end



function process_sys:input(input_str)

	

end