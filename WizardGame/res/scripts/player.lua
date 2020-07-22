player = {
  {
    componentName = "CameraComponent",
    fov = 70.0,
    fPersonRef = true
  },
  {
    componentName = "ObjectComponent",
    folder = "./res/models/illusion.obj",
	color = {
		r = 0,
		g = 0.5,
		b = 0.5,
		a = 0.25
	}
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
	font = "./res/fonts/CaslonAntique.ttf",
	color = {
		r = 0,
		g = 1,
		b = 0
	},
	size = 35
  }
}


function player_Hit(e,e2,hits)
  if (e2:getCanBeHit() == true) then
  e2:setHP(e2:getHP()-hits)
  if (hits>1) then
	print("Bounced off wall")
  end
  e2:setColorFlash(e:random(0.0,0.3),e:random(0.6,1),e:random(0.6,1),1.0)
  -- e2:moveForwards(-30.0)
  -- e2:setFrozen(true)
  e:playSound("./res/sounds/explosion/"..e:randomInt(1,4)..".wav")
  if (e2:getHP() == 0) then
    e:Shake(4.0)
	if (e2:getFloat("scoreInc") ~= 0) then
		e:setFloat("Score", e:getFloat("Score")+(e2:getFloat("scoreInc")*hits));
	else
		e:setFloat("Score", e:getFloat("Score")+(100*hits));
	end
	e2:Emit(100, 0.5,1.0,1.0,0.8);
  else
	e:setFloat("Score", e:getFloat("Score")+(20*hits));
    e:Shake(2.5)
	e2:Emit(50, 0.5,1.0,1.0,0.8);
  end
  if (e2:hasAnimation("damaged") == true) then
	e2:resetFrame()
	e2:playAnimationTag("damaged")
  end
  e:setGlobalFloat("lastScore", e:getFloat("Score"));
  end
end
function player_RunAbility(e)
  if (e:getString("Ability") == "Rapid Fire") then
		e:setProjColor(0.8,0.15,0.0,0.8)
		e:setProjDelay(10)
		e:setProjRange(100)
  else
		e:setProjColor(0.5,1.0,1.0,1.0)
		e:setProjDelay(30)
		e:setProjRange(300)
  end
  if (e:getKeyPressed() == "LSHIFT") then
    --when pressed
    if (e:getString("Ability") == "Illusion") then
		e:setObjectPos(e:getX(), 0.0, e:getZ())
		e:setFloat("illusionCount",1.0)
		e:setObjectScale(1.2)
	end
	if (e:getString("Ability") == "Dash") then
		if (e:getBool("CanDash") == true) then
			e:moveForward(5.0*e:getMoveDirY())
			e:Strafe(e:getMoveDirX()*-5.0)
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
			e:setFloat("DashCount", e:getFloat("DashCount")-0.5)
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
      if (e:getFloat("AbilityCount") > 50) then
        e:moveForward(e:getMoveDirY()*200.0)
		e:Strafe(e:getMoveDirX()*-200.0)
        e:setFloat("AbilityCount", 0)
      end
    end
    if (e:getString("Ability") == "Speed") then
	  if (e:getFloat("AbilityCount") < 120) then
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
    --when unpressed
	if (e:getFloat("DashCount") > 1) then
		e:setFloat("DashCount", e:getFloat("DashCount")-0.5)
	end
    e:setFloat("Speed", e:getDefaultSpeed())
	e:setFOV(70);
	if (e:getFloat("FireCount") > 0) then
		e:setFloat("FireCount", e:getFloat("FireCount")-0.2);
	else
		e:setBool("hasDamagedDash", false)
	end
    if (e:getString("Ability") == "Speed") then
		if (e:getFloat("AbilityCount") > 0) then
			e:setFloat("AbilityCount", e:getFloat("AbilityCount")-1);
		end
	end
	if (e:getString("Ability") == "Teleport") then
		e:setFloat("AbilityCount", 0)
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
function player_updateIllusion(e)
	if (e:getString("Ability") == "Illusion") then
		e:setObjectColor(0.0,1.0,1.0,e:getFloat("illusionCount"))
		if (e:getFloat("illusionCount") > 0.0) then
			e:setFloat("illusionCount",e:getFloat("illusionCount")-0.002)
			e:setEnemyTarget(e:getObjectX(), e:getObjectZ())
		else
			e:setEnemyTarget(e:getX(), e:getZ())
		end
	else
		e:setEnemyTarget(e:getX(), e:getZ())
		e:setObjectColor(0.0,1.0,1.0,0.0)
	end
end
function player_drawText(e)
	e:setTextColor(0.0,1.0,0.0,1.0)
	e:setText("health", "health: " .. e:getHP(), 0.6, -0.9)
	if (e:getGlobalBool("enabled") == true) then
		e:setText("score", "score: " .. e:getFloat("ScoreDisplay"), -0.3, 0.9)
	else
		e:setText("score", "Last Score: " .. e:getGlobalFloat("lastScore"), -0.3, 0.9)
	end
end
function player_clearText(e)
	e:setTextColor(0.0,1.0,0.0,0.0)
	e:setText("health", "", 0.6, -0.9)
	e:setText("score", "", -0.3, 0.9)
	
end
function player_Update(e)
  player_updateIllusion(e)
  if (e:getGlobalBool("drawPlayerUI") == true) then
	player_drawText(e)
  else
	player_clearText(e)
  end
  --e:setTextColor(0.0,1.0,0.0,0.5)
  --e:setText("score2", "score: " .. e:getFloat("Score"), -0.3, 0.9)
  if (e:getFloat("Score") > e:getFloat("ScoreDisplay")) then
	e:setFloat("ScoreDisplay", e:getFloat("ScoreDisplay")+5);
  end
  e:setSaturation(0.06+e:getFloat("FireCount")/10.0)
  player_RunAbility(e)
  e:setValue(e:getFloat("AbilityCount")/240.0-0.05)
  if (e:getProjCount() == 54) then
	e:setPlayerTag("fire")
	--e:playSound("./res/sounds/shoot.wav")
  end
  if (e:getKeyPressed() == "SPACE") then
    e:Fire()
	if (e:getFloat("SpeedMod") > 0.3) then
		e:setFloat("SpeedMod",e:getFloat("SpeedMod")-0.01)
	end
  else
	e:setProjCount(52)
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
  if (e:getKeyPressed() ~= "ESCAPE") then
	e:setBool("EscapeReleased", true)
  end
  if (e:getBool("inCloseMenu") == true) then
	if (e:getKeyPressed() == "ESCAPE" and e:getBool("EscapeReleased") == true) then
		e:setBool("EscapeReleased", false)
		e:setBool("inCloseMenu", false)
	end
	e:setTextColor(1.0,0.0,0.0,0.0)
	e:setText("close", "", -0.5, 0.5)
  else
	if (e:getKeyPressed() == "ESCAPE" and e:getBool("EscapeReleased") == true) then
		e:setBool("EscapeReleased", false)
		e:setBool("inCloseMenu", true)
	end
	if (e:getKeyPressed() == "SPACE") then
		e:stopProgram()
	end
	e:setTextColor(1.0,0.0,0.0,1.0)
	e:setText("close", "Press SPACE to close\n           the game", -0.5, 0.5)
	
	

  end
  
  if (e:getBool("CanTime") == false and e:getPaused() == false) then
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
	e:setGlobalBool("canPlayerMove", true)
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
	e:setBool("inCloseMenu", true)
	e:setBool("EscapeReleased", true)
	e:setBool("hasIllusion", false)
	e:setFloat("illusionCount", 0.0)
	e:clearAbilities()
	e:addAbility("Fire", "Damage enemies \n within a radius")
	e:addAbility("Teleport", "Quickly teleport\n the direction \n you are moving")
	e:addAbility("Time", "Stop Time for \n a moment")
	e:addAbility("Speed", "Go very fast for\n about a second")
	e:addAbility("Dash", "Dash Forward a short\n distance and damage any\n enemies you hit")
	e:addAbility("Rapid Fire", "Be able to fire\n very fast, \nbut deal less damage")
	e:addAbility("Illusion", "Create Illusions\n that enemies \n will think is you")
	e:setString("Ability", e:getAbility(e:getGlobalFloat("selectedAbility")))
	--e:setScreenResolution(1920,1080)
end
