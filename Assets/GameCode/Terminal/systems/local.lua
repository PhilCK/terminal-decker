--[[
	local.lua
	--
	This represents localhost.
	localhost is where you can use your hacking tools.
]]


localhost = {
	instances = {},
	process_id = 0,
}



function localhost.reset()

	localhost.instances = nil;

end


function localhost.connect()

	localhost.process_id = localhost.process_id + 1

	localhost.instances[localhost.process_id] = {}
	localhost.instances[localhost.process_id].systems = {}

	print(localhost.process_id)

	return localhost.process_id

end


function localhost.input(process_id, input_str, args)

	print("got localhost input")

	print(process_id)

	for i, v in pairs(localhost.instances[process_id].systems) do 

		local input_swallowed = v.input(input_str, args)

		if input_swallowed then
			return true
		end

	end
	
	-- No system wanted the desired input.
	return false

end