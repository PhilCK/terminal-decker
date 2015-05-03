--[[
	local.lua
	--
	This represents localhost.
	localhost is where you can use your hacking tools.
]]

require "Terminal/programs/dict_hack"

localhost = {

  history = {}

}



function localhost.reset()

end


function localhost.get_name()

  return "localhost"

end



function localhost.connect(screen_id)

  terminal.echo(screen_id, "Welcome to localhost")
  return localhost;

end



function localhost.disconnect(screen_id)

  terminal.echo(screen_id, "disconnected from localhost")

end



function localhost.on_think(screen_id, delta_time)

end


-- localhost input handler

function localhost.input(screen_id, input_str, args)


  if(input_str == "help") then
    terminal.echo(screen_id, "System Helper")
    return true
  end


  if(input_str == "clear") then
    terminal.clear(screen_id)
    return true
  end


  if(input_str == "net.connect") then
    terminal.echo(screen_id, "net.connect")
    return true
  end


  if(input_str == "net.connect.history") then
    terminal.echo(screen_id, "sys.connect.history")
    return true
  end


  if(input_str == "dict_hack.help") then
    dict_hack.help(screen_id)
    return true
  end


  -- unix things

  if(input_str == "whoami") then
    terminal.echo(screen_id, "nobody")
    return true
  end


  if(input_str == "pwd") then
    terminal.echo(screen_id, "/")
    return true
  end


  return false

end