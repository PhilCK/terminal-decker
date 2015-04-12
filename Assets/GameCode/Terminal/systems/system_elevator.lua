--[[
	Elevator controls.
]]


elevator_sys = {}


function elevator_sys.input(input_str)

	if(input_str == "elevator.help") then
		elevator_sys.help_menu()
		return true
	end

	return false

end



function elevator_sys.help_menu()

	print("elevator help menu");

end