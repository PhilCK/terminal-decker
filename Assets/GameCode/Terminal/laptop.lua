--[[
	laptop.lua
	--
	This is the hackers laptop.
	The laptop can have x connections to other systems (including localhost).
]]


require "Terminal/systems/local"


laptop = {

}


-- Make 'n' network connections to the localhost
function laptop.reset_connections()

  laptop['max_processes'] = terminal.get_number_of_screens()
  laptop['processes']     = {}
  laptop['net_history']   = {}
  
	for i = 1, laptop['max_processes'] do

    table.insert(laptop['processes'], localhost.connect(i))

  end


end


function laptop.input_str(screen_id, input_str)

  captured = laptop['processes'][screen_id].input(screen_id, input_str, "1")

  if captured then return true end
  
  if(input_str == "task.help") then
  
    terminal.echo(screen_id, "Tasks Help")
    terminal.echo(screen_id, "----------")
    terminal.echo(screen_id, "'task.next' will switch to the next task.")
    terminal.echo(screen_id, "'task.prev' will switch to the previous task.")
    terminal.echo(screen_id, "'task.list' will display running tasks.")
    

  end

  if(input_str == "task.next") then
  
    next_screen_id = screen_id + 1;
    terminal.set_active_screen(next_screen_id);
    
    return
  
  end
  
  if(input_str == "task.prev") then
    
    next_screen_id = screen_id - 1;
    terminal.set_active_screen(next_screen_id);
    
    return
    
  end

  if(input_str == "task.list") then

    terminal.echo(screen_id, "Tasks Running")
    terminal.echo(screen_id, "-------------")
    
    for i = 1, terminal.get_number_of_screens() do
      if(i == screen_id) then
        terminal.echo(screen_id, "task " .. i .. ". *current*")
      else
        terminal.echo(screen_id, "task " .. i .. ".")
      end
    end
    
    return

  end

  -- found nothing, bad request.
  terminal.echo(screen_id, "-- Bad Request --")

end


function laptop.on_think(screen_id, delta_time)

  -- bootup info.
  if(laptop['boot_up'] == true) then

    laptop['boot_up_counter'] = laptop['boot_up_counter'] + delta_time

    if(laptop['boot_up_counter'] > 10 and laptop['boot_up_stage'] == 8) then
      laptop['boot_up'] = false
      laptop['boot_up_stage'] = laptop['boot_up_stage'] + 1
      laptop.reset_connections()
    end

    if(laptop['boot_up_counter'] > 7 and laptop['boot_up_stage'] == 7) then
      terminal.echo(screen_id, "Encrypting Connection [4] ... [DONE]")
      laptop['boot_up_stage'] = laptop['boot_up_stage'] + 1
      return
    end

    if(laptop['boot_up_counter'] > 6 and laptop['boot_up_stage'] == 6) then
      terminal.echo(screen_id, "Encrypting Connection [3] ... [DONE]")
      laptop['boot_up_stage'] = laptop['boot_up_stage'] + 1
      return
    end

    if(laptop['boot_up_counter'] > 5 and laptop['boot_up_stage'] == 5) then
      terminal.echo(screen_id, "Encrypting Connection [2] ... [DONE]")
      laptop['boot_up_stage'] = laptop['boot_up_stage'] + 1
      return
    end

    if(laptop['boot_up_counter'] > 4 and laptop['boot_up_stage'] == 4) then
      terminal.echo(screen_id, "Encrypting Connection [1] ... [DONE]")
      laptop['boot_up_stage'] = laptop['boot_up_stage'] + 1
      return
    end

    if(laptop['boot_up_counter'] > 3 and laptop['boot_up_stage'] == 3) then
      terminal.echo(screen_id, "Checking Sandbox Integrity ... [DONE]")
      laptop['boot_up_stage'] = laptop['boot_up_stage'] + 1
      return
    end

    if(laptop['boot_up_counter'] > 3 and laptop['boot_up_stage'] == 2) then
      terminal.echo(screen_id, "Mounting Net Drive v0.2 ... [DONE]")
      laptop['boot_up_stage'] = laptop['boot_up_stage'] + 1
      return
    end

    if(laptop['boot_up_counter'] > 2 and laptop['boot_up_stage'] == 1) then
      terminal.echo(screen_id, "Connecting to Net ... [DONE]")
      laptop['boot_up_stage'] = laptop['boot_up_stage'] + 1
      return
    end
  
  
    if(laptop['boot_up_counter'] > 0 and laptop['boot_up_stage'] == 0) then
      terminal.echo(screen_id, "Booting Ono-Sendai Prototype Net Deck - 0.0001v")
      terminal.echo(screen_id, "This Deck is for internal use only!")
      
      laptop['boot_up_stage'] = laptop['boot_up_stage'] + 1
      return
    end
    
    return
  end

  -- If we are not booting up then send think to the processes.
  for i = 1, terminal.get_number_of_screens() do
    laptop['processes'][i].on_think(screen_id, delta_time)
  end

end


function laptop.boot_up(screen_id)

  laptop['boot_up'] = true
  laptop['boot_up_counter'] = 0
  laptop['boot_up_stage'] = 0

end

