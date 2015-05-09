--[[

  dict hack is a dictionary hacking program.

]]--


dict_hack = {}


function dict_hack.hack(screen_id, user_file, terminal_instance_id)

  if(type(dict_hack['processes']) ~= "table") then
    dict_hack['processes'] = {}
  end
  
  dict_hack['processes'][screen_id] = {
    processing = true,
    timer = 0,
    file = user_file,
  }

end


function dict_hack.check_file(screen_id, user_file)

  terminal.echo(screen_id, "file is in correct format")

end


function dict_hack.help(screen_id)

  terminal.echo(screen_id, "Dictionary Hacking Tool")
  terminal.echo(screen_id, "-- by hekr021@freenet -- ")
  terminal.echo(screen_id, "dict_hack <stolen.user_db> - assumes db is encrypted in 982.a")
  terminal.echo(screen_id, "dict_hack.check <stolen.user_db> - check if file is correct format")

end
