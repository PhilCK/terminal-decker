--[[
	laptop.lua
	--
	This is the hackers laptop.
	The laptop can have x connections to other systems (including localhost).
]]


require "GameCode/Terminal/systems/local"


laptop = {
	
	max_processes = 4

	processes = {

	}
}


-- Make 'n' network connections to the localhost
function laptop.reset_connections()

	--terminal.set_number_of_buffers(max_processes)

	for i = 0, max_processes, 1 do
		local localhost = localhost.connect()
		table.insert(laptop.processes, localhost)
	end

end



function laptop.input_str(input_str)

	-- Check is process switch command
	if(input_str == "task")
		-- terminal.switch_buffer()

		return true
	end

	

end
