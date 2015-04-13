--[[
	Local system
	This is the local machine.	
]]

--require "terminal"

local_sys = {}


function local_sys.input(input_str)

	if(input_str == "sys.help") then
		local_sys.help_menu()
		terminald.clear()
		return true
	end

	return false

end


function local_sys.help_menu()

	print("local sys help")

end