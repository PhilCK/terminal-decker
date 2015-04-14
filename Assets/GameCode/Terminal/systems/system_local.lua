--[[
	Local system
	--
	These are basic bits and bobs for the local machine.	
]]

--require "terminal"

local_sys = {}


function local_sys.input(input_str)

	-- TODO: Split up the input string so as to be able to get the 'input' and the 'args'
	local input_cmd = ""
	local input_args = {"doop"}

	-- Shutdown
	if(input_str == "sys.quit") then
		terminal.quit()
		return true
	end

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

	-- Echo string.
	if(input_cmd == "echo") then
		terminal.echo(input_args[1])
		return true
	end

	-- Echo Help.
	if(input_str == "echo.help") then
		local_sys.echo_help_menu()
		return true
	end

	-- ls.
	if(input_str == "ls") then
		terminal.echo("Nothing to show")
		return true
	end

	-- ls Help.
	if(input_str == "ls.help") then
		terminal.echo("Useage: ls")
		return true
	end

	-- Found nothing.
	return false

end

-- Output the help menu.
function local_sys.help_menu()

	terminal.echo("System Help")
	terminal.echo("-----------")
	terminal.echo("Type 'sys.quit' to quit the application.")
	terminal.echo("Type 'clear' to clear the screen.")
	terminal.echo("Type 'ls' to list files and folders.")
	terminal.echo("Type 'echo' to echo an output.")

end


function local_sys.echo_help_menu()

	terminal.echo("Useage: echo <string you wish to print>")

end