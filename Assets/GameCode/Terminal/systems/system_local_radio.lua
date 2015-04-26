--[[
	Radio
	--
	Radio plays stuff in the background.
]]

--require "terminal"

local radio_sys = {}


function radio_sys.input(input_str)

	-- Stop the radio playing.
	if(input_str == "radio.stop") then

		return true

	end

	-- Turn the radio on.
	if(input_str == "radio.start") then

		return true

	end

	-- Change the volume
	if(input_str == "radio.volume") then

		return true

	end

	return false

end


function radio_sys.request_access()

	-- Hold input to this system only.

end