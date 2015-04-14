--[[
	Local system
	This is the local machine.	
]]

--require "terminal"

local_sys = {}


function local_sys.input(input_str)

	-- Display help.
	if(input_str == "sys.help") then
		local_sys.help_menu()
		return true
	end

	-- Clear the screen.
	if(input_str == "clear") then
		terminal.clear()
		return true
	end

	-- Found nothing.
	return false

end

-- Output the help menu.
function local_sys.help_menu()

	terminal.echo("System Help")
	terminal.echo("-----------")
	terminal.echo("Type 'clear' to clear the screen.")
	terminal.echo("Type 'ls' to list files and folders.")
	terminal.echo("Type 'echo' to echo an output.")

end