player = {
  {
    componentName = "CameraComponent",
    fov = 70.0,
    fPersonRef = true
  },
  {
    componentName = "CollisionComponent",
    scale = {
      x = 1.5,
      y = 6.0,
      z = 3.0
    }
  },
  {
    componentName = "ProjComponent",
    model = "res/models/projectiles/fireball.obj",
    colorR = 0.5,
    colorG = 1.0,
    colorB = 1.0,
    speed = 5.0,
    range = 300,
    height = 8.0,
    delay = 50
  }
}
function player_Hit(e,e2,hits)
  e2:setHP(e2:getHP()-1)
  e2:setColor(e:random(0.0,0.3),e:random(0.6,1),e:random(0.6,1),1.0)
  -- e2:moveForwards(-30.0)
  -- e2:setFrozen(true)
  if (e2:getHP() == 0) then
    e:Shake(4.0)
	e2:Emit(200, 0.5,1.0,1.0,0.3);
  else
    e:Shake(2.5)
	e2:Emit(50, 0.5,1.0,1.0,0.3);

  end
  if (e:random(0.0,1.0) > 0.5) then
	e2:playAnimationTag("damaged")
  end
  
end
function player_RunAbility(e)
  if (e:getKeyPressed() == "LSHIFT") then
    e:setFloat("AbilityCount", e:getFloat("AbilityCount")+1)
    if (e:getString("Ability") == "Teleport") then
      if (e:getFloat("AbilityCount") > 30) then
        e:moveForward(100.0)
        e:setFloat("AbilityCount", 0)
      end
    end
    if (e:getString("Ability") == "Speed") then
      e:setFloat("Speed", e:getDefaultSpeed()*4.0)
    end
    if (e:getString("Ability") == "Time" and e:getBool("CanTime") == true) then
      e:setGlobalFrozen(true)
      e:setInverted(1)
      e:setFloat("TimeCount", e:getFloat("TimeCount")+1)
      if (e:getFloat("TimeCount")>=e:getFloat("TimeLength")) then
        e:setBool("CanTime", false);
      end
    end
  else
    e:setFloat("Speed", e:getDefaultSpeed())
    
    if (e:getFloat("TimeCount")>0) then
        e:setFloat("TimeCount",e:getFloat("TimeCount")-2)
    end

    if (e:getString("Ability") == "Time") then
      e:setInverted(0)
      e:setGlobalFrozen(false)
      e:setBool("CanTime",true);
    end
  end
end
function player_Update(e)
  player_RunAbility(e)
  e:setBrightness(1+e:getProjCount()/180.0)
  if (e:getProjCount() >= 40) then
	e:setPlayerTag("fire")
  end

  if (e:getKeyPressed() == "SPACE") then
    e:Fire()
	if (e:getFloat("SpeedMod") > 0.3) then
		e:setFloat("SpeedMod",e:getFloat("SpeedMod")-0.01)
	end
  else
	e:setProjCount(0)
	e:setFloat("SpeedMod",1.0)
	if (e:getFloat("SpeedMod") < 1.0) then
		e:setFloat("SpeedMod",e:getFloat("SpeedMod")+0.01)
	end
  end
  if (e:getKeyPressed() == "SPACE") then
    if (e:getDistanceFromNearestEnt() < 10.0) then
		if (e:getFloat("MeleeCount") >= 20) then
			e:damageNearest(2);
			e:setFloat("MeleeCount", 0)
		end
		e:setFloat("MeleeCount",e:getFloat("MeleeCount")+1)
	end
  end
  if (e:getKeyPressed() == "ESCAPE") then
    e:kill()
  end
  if (e:getBool("CanTime") == false) then
    e:setInverted(0)
    e:setGlobalFrozen(false)
  end
  -- if (e:doesEntityExist("test2") == true) then
  --   print(e:getPo
  if (e:getPaused() == false) then
    e:FPSControllerUpdate(e:getFloat("Speed")*e:getFloat("SpeedMod"))
  end
  e:pushInsideLevel();
  e:UpdateKeyPresses();
  
  e:setUIText(e:getString("Ability") .. ": " .. e:getFloat("TimeCount"))
end
function player_Start(e)
	e:setFloat("SpeedMod", 0)
    e:setFloat("TimeLength", 120)
    e:setHP(10)
    e:setFloat("ShieldCount", 0)
    -- e:TopDown_Start()
    e:setFloat("TimeCount", 0)
    e:setString("Ability", "Time")
	e:setFloat("MeleeCount", 0)
    e:setFloat("Speed", 0.5)
    e:setBool("justTeleported", false)
    e:setFloat("AbilityCount", 0)
    e:setBool("CanTime", true)
end
