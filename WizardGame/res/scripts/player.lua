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
    delay = 55
  },
  {
	componentName = "GUIComponent",
	font = "./res/Avara.ttf",
	color = {
		r = 0,
		g = 1,
		b = 0
	},
	size = 25
  }
}


function player_Hit(e,e2,hits)
  e2:setHP(e2:getHP()-hits)
  e2:setColorFlash(e:random(0.0,0.3),e:random(0.6,1),e:random(0.6,1),1.0)
  -- e2:moveForwards(-30.0)
  -- e2:setFrozen(true)
  if (e2:getHP() == 0) then
    e:Shake(4.0)
	if (e2:getFloat("scoreInc") ~= 0) then
		e:setFloat("Score", e:getFloat("Score")+e2:getFloat("scoreInc"));
	else
		e:setFloat("Score", e:getFloat("Score")+100);
	end
	e2:Emit(100, 0.5,1.0,1.0,0.8);
  else
	e:setFloat("Score", e:getFloat("Score")+10);
    e:Shake(2.5)
	e2:Emit(50, 0.5,1.0,1.0,0.8);
  end
  if (e2:hasAnimation("damaged") == true) then
	e2:playAnimationTag("damaged")
  end
end
function player_RunAbility(e)
  print(e:getFloat("DashCount"))
  if (e:getKeyPressed() == "LSHIFT") then
	if (e:getString("Ability") == "Dash") then
		if (e:getBool("CanDash") == true) then
			e:moveForward(5.0)
			if (e:getDistanceFromNearestEnt() < 15) then
				if (e:getBool("hasDamagedDash") == false) then
					e:damageNearest(4)
					e:Shake(7)
					e:setBool("hasDamagedDash", true)
				end
			end
			e:setFloat("DashCount", e:getFloat("DashCount")+1)
			if (e:getFloat("DashCount") > 20) then
				e:setBool("CanDash", false)
			end
		else
			e:setFloat("DashCount", e:getFloat("DashCount")-1)
			if (e:getFloat("DashCount") < 1) then
				e:setBool("CanDash", true)
			end
		end
	end
	if (e:getString("Ability") == "Fire") then
		if (e:getFloat("FireCount") <= 0) then
			if (e:getDistanceFromNearestEnt() < 35) then
				e:damageNearest(4)
			end
			e:Emit(100,1.0,0.5,0.3,0.8)
			e:setFloat("FireCount", 10)
		end
	end
    if (e:getString("Ability") == "Teleport") then
	e:setFloat("AbilityCount", e:getFloat("AbilityCount")+1)
      if (e:getFloat("AbilityCount") > 10) then
        e:moveForward(100.0)
        e:setFloat("AbilityCount", 0)
      end
    end
    if (e:getString("Ability") == "Speed") then
	  if (e:getFloat("AbilityCount") < 15) then
        e:setFloat("Speed", e:getDefaultSpeed()*4.0)
		e:setFloat("AbilityCount", e:getFloat("AbilityCount")+1)
		e:setFOV(70.15);
      else
		e:setFloat("Speed", e:getDefaultSpeed())
		e:setFOV(70);
	  end
    end
    if (e:getString("Ability") == "Time" and e:getBool("CanTime") == true) then
      e:setGlobalFrozen(true)
      e:setInverted(1)
      e:setFloat("AbilityCount", e:getFloat("AbilityCount")+1)
      if (e:getFloat("AbilityCount")>=e:getFloat("TimeLength")) then
        e:setBool("CanTime", false);
      end
    end
  else
	e:setFloat("DashCount", 0)
    e:setFloat("Speed", e:getDefaultSpeed())
	e:setFOV(70);
	if (e:getFloat("FireCount") > 0) then
		e:setFloat("FireCount", e:getFloat("FireCount")-0.2);
	else
		e:setBool("hasDamagedDash", false)
	end
    if (e:getString("Ability") == "Speed") then
		if (e:getFloat("AbilityCount") > 0) then
			e:setFloat("AbilityCount", e:getFloat("AbilityCount")-0.1);
		end
	end
	
    if (e:getString("Ability") == "Time") then
      e:setInverted(0)
      e:setGlobalFrozen(false)
      e:setBool("CanTime",true);
	  if (e:getFloat("AbilityCount")>0) then
        e:setFloat("AbilityCount",e:getFloat("AbilityCount")-2)
	  end
    end
  end
end

function player_resetHP(e)
	e:setHP(e:getFloat("MaxHP"))
end

function player_Update(e)
  e:setTextColor(0.0,1.0,0.0,1.0)
  e:setText("health", "health: " .. e:getHP(), 0.6, -0.9)
  e:setText("score", "score: " .. e:getFloat("ScoreDisplay"), -0.3, 0.9)
  e:setTextColor(0.0,1.0,0.0,0.5)
  e:setText("score2", "score: " .. e:getFloat("Score"), -0.3, 0.9)
  if (e:getFloat("Score") > e:getFloat("ScoreDisplay")) then
	e:setFloat("ScoreDisplay", e:getFloat("ScoreDisplay")+5);
  end
  e:setSaturation(0.06+e:getFloat("FireCount")/10.0)
  player_RunAbility(e)
  e:setValue(e:getFloat("AbilityCount")/240.0-0.05)
  if (e:getProjCount() >= 40) then
	e:setPlayerTag("fire")
	--e:playSound("./res/sounds/shoot.wav")
  end

  if (e:getKeyPressed() == "SPACE") then
    e:Fire()
	if (e:getFloat("SpeedMod") > 0.3) then
		e:setFloat("SpeedMod",e:getFloat("SpeedMod")-0.01)
	end
  else
	e:setProjCount(30)
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
  if (e:getPaused() == false and e:getGlobalBool("canPlayerMove") == true) then
    e:FPSControllerUpdate(e:getFloat("Speed")*e:getFloat("SpeedMod"))
  end
  e:UpdateKeyPresses();
  
  e:setUIText(e:getString("Ability") .. ": " .. e:getFloat("TimeCount"))
  e:setString("Ability", e:getAbility(e:getGlobalFloat("selectedAbility")))
end
function player_Start(e)
	e:setParticleSpread(1.5)
	e:setParticleModel("./res/models/fire.obj");
	e:setFloat("SpeedMod", 0)
    e:setFloat("TimeLength", 120)
    e:setHP(10)
    e:setFloat("ShieldCount", 0)
	e:setFloat("DashCount", 0)
	e:setBool("CanDash", true)
    -- e:TopDown_Start()
    e:setFloat("TimeCount", 0)
    
	e:setBool("hasDamagedDash", false)
	e:setFloat("Score", 0)
	e:setFloat("ScoreDisplay", 0)
	e:setFloat("MeleeCount", 0)
    e:setFloat("Speed", 0.5)
	e:setFloat("MaxHP", 10)
    e:setBool("justTeleported", false)
    e:setFloat("AbilityCount", 0)
    e:setFloat("FireCount", 0)
    e:setBool("CanTime", true)
	
	e:addAbility("Time")
	e:addAbility("Speed")
	e:addAbility("Teleport")
	e:addAbility("Fire")
	e:addAbility("Dash")
	e:setString("Ability", e:getAbility(e:getGlobalFloat("selectedAbility")))
end
