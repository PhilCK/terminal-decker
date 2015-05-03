
require "Terminal/laptop"


-- Anything you want to load?
function terminal.on_load()

  local screen_id = terminal.get_current_screen_id()

  laptop.boot_up(screen_id)

end


-- User entered in a cmd.
function terminal.on_input_string(input)

  local screen_id = terminal.get_current_screen_id()

  -- quit application
  if(input == "app.quit") then
    terminal.quit()
    return
  end
  
  -- pass through to game
  laptop.input_str(screen_id, input)

end


-- User requested an auto complete.
function terminal.on_auto_complete(current_cmd_string)



end


-- Update tick.
function terminal.on_think(delta_time)

  local screen_id = terminal.get_current_screen_id()
  
  laptop.on_think(screen_id, delta_time)

end


-- Last chance to save.
function terminal.on_quit()



end
