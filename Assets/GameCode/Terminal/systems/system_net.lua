--[[
	Net
	--
	This handles the connection to remote computer systems.
]]

--require "terminal"

net_sys = {

	local_sys = {}

}


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


	if(input_str == "net.local") then

		if(#net_sys.local_sys == 0) then
			terminal.echo("Nothing found on the local system.")
		else
			terminal.echo("Stuff on local system.")

			for i, v in pairs(net_sys.local_sys) do
				terminal.echo("Found en")
			end
		end

		return true

	end

	-- Found nothing.
	return false

end


function net_sys.connect_to_local_sys(local_system)



end
