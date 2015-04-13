
terminal = {} --handle this inside the application?
term = require "terminald"
test_system = {}


require "GameCode/Terminal/systems/system_local"
require "GameCode/Terminal/systems/system_elevator"


function test_system.input(input)

	print("test_system")

	return true

end


-- Anything you want to load?
function terminal.load()

	terminal.systems = {}

	terminal.systems[0] = test_system
	terminal.systems[1] = elevator_sys
	terminal.systems[2] = local_sys

	print("terminal.loaded")

end


-- User entered in a cmd.
function terminal.input_string(input)

	print("terminal.input_string")
	print(input)

	local is_valid = true;

	-- if valid command
	if(is_valid) then
		
		terminal.systems[0].input(input)
		terminal.systems[1].input(input);
		terminal.systems[2].input(input);

	end

	-- bad request.
	print("bad request")

	return false;

end


-- User requested an auto complete.
function terminal.auto_complete(current_cmd_string)

end


-- Update tick.
function terminal.update()

	--print("terminal.update")

end


-- Last chance to save.
function terminal.quitting()

end


-- terminal.console.set_prompt("admin: ")
-- terminal.console.add_to_buffer("foop")
-- terminal.console.set_input(false)
