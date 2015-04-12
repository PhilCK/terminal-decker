--[[
	Local system
	This is the local machine.	
]]

local_sys = {}


function local_sys.input(input_str)

	if(input_str == "sys.help") then
		local_sys.help_menu()
		return true
	end

	return false

end


function local_sys.help_menu()

	print("local sys help")

end