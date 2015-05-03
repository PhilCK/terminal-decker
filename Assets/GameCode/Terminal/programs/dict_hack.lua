--[[

  dict hack is a dictionary hacking program.

]]--


dict_hack = {}


function dict_hack.hack(screen_id, user_file, terminal_instance_id)

  -- stop user input.
  -- do spinny thing.
  -- jump up and down with joy.
  -- reveal usr and pass word.

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
