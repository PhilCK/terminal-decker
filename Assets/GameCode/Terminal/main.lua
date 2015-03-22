terminal = {}

-- Anything you want to load?
function terminal.load()

	print("terminal.loaded")

end


function terminal.command(command, arguments)

	print("terminal.command")
	print(command)

end

-- Update tick.
function terminal.update()

	--print("terminal.update")

end


-- terminal.console.set_prompt("admin: ")
-- terminal.console.add_to_buffer("foop")
-- terminal.console.set_input(false)
