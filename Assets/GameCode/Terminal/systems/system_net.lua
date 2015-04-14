--[[
	Net
	--
	This handles the connection to remote computer systems.
]]

--require "terminal"

net_sys = {}


function net_sys.input(input_str)

	-- TODO: Split up the input string so as to be able to get the 'input' and the 'args'
	local input_cmd = "echo"
	local input_args = {"doop"}

	if(input_str == "net") then

		terminal.echo("no net connection :/")

		return true

	end

	if(input_str == "net.help") then

		terminal.echo("Net Help")
		terminal.echo("--------")
		terminal.echo("net.connect <ip/hostname> to connect to remote system")
		terminal.echo("net.disconnect to disconnect from a system")

		return true

	end

	-- Found nothing.
	return false

end
