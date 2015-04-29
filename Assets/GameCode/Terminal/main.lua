

require "Terminal/systems/system_local"
require "Terminal/systems/system_elevator"
require "Terminal/systems/system_net"
require "Terminal/systems/local"



-- Anything you want to load?
function terminal.load()

	terminal.systems = {}

	terminal.systems[1] = elevator_sys -- better way to insert?
	terminal.systems[2] = local_sys
	terminal.systems[3] = net_sys

	print("terminal.loaded")

end


-- User entered in a cmd.
function terminal.input_string(input)

	print("terminal.input_string")
	print(input)

	local is_valid = true;


	-- -- if valid command
	-- if is_valid then
		
	-- 	local found_command = false;

	-- 	for i, v in pairs(terminal.systems) do
	-- 		found_command = v.input(input)

	-- 		-- Command was consumed by a system so stop
	-- 		if found_command then
	-- 			return true
	-- 		end
	-- 	end

	-- end

	-- bad request.
	terminal.echo(" - bad request - ")

	return false;

end


-- User requested an auto complete.
function terminal.auto_complete(current_cmd_string)

end


-- Update tick.
function terminal.update()

end


-- Last chance to save.
function terminal.quitting()

end


-- terminal.console.set_prompt("admin: ")
-- terminal.console.add_to_buffer("foop")
-- terminal.console.set_input(false)
