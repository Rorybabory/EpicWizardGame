abilityManager = {
  {
    componentName = "GraphicsComponent",
    folder = "./res/models/WizardUI"
  },
  {
	componentName = "GUIComponent",
	font = "./res/fonts/CaslonAntique.ttf",
	color = {
		r = 0,
		g = 0,
		b = 1
	},
	size = 40
  }
}
function abilityManager_Hit(e,e2,hits)
  -- e2:setColor(0.0,0.0,0.0,1.0)
  e2:setHP(e2:getHP()-hits)
  -- if (e:getAnimation() == 2) then
  --   e:playAnimation(0)
  -- end
  if (e2:isPlayer() == true) then
    e2:Shake(5.0)
  end
  e2:write(e2:getHP())
end
function abilityManager_easeOutCubic(t)
    return 1 + (t-1) * t * t;
end

function abilityManager_drawAbility(e)
	
	e:setMouseCapture(false)
	e:setImageDraw("direction", true)
	e:setAnimationTag("default",e:getGlobalFloat("selectedAbility"))
	e:setGlobalBool("canPlayerMove", false)
	e:setDrawScene(false)
	
	--allow moving selection based on WASD
	if (e:getArrowDirY() ~= 0 or e:getMoveDirY() ~= 0) then
		e:setFloat("selectCounter", e:getFloat("selectCounter")+1)
		if (e:getFloat("selectCounter") > 15) then
			e:setFloat("selectCounter", 0)
			e:setGlobalFloat("selectedAbility", e:getGlobalFloat("selectedAbility")-e:getArrowDirY())
			e:setGlobalFloat("selectedAbility", e:getGlobalFloat("selectedAbility")-e:getMoveDirY())
			e:resetFrame()
			--push selection into bounds
			if (e:getGlobalFloat("selectedAbility") == e:getAbilityCount()) then
				e:setGlobalFloat("selectedAbility", e:getAbilityCount()-1)
			end
			if (e:getGlobalFloat("selectedAbility") < 0) then
				e:setGlobalFloat("selectedAbility", 0)
			end
			e:setFloat("TextX",0.0)
			e:setFloat("DescX",0.0)
			e:setBool("MovingDesc", true)
			--play sound effect
			e:playSound("./res/sounds/menu.wav")
		end
	end
	if (e:getBool("MovingDesc") == true) then
		e:setFloat("DescX",e:getFloat("DescX")+0.05)
	end
	
	if (e:getFloat("DescX") > -0.06 and e:getFloat("DescX") < 0.00) then
		e:setBool("MovingDesc", false)
	end
	if (e:getFloat("DescX")>1.5) then
		e:setFloat("DescX", -2.0)
		e:setString("abilityText", e:getAbilityDescription(e:getGlobalFloat("selectedAbility")))
	end
	if (e:getFloat("TextX") < 0.04) then
		e:setFloat("TextX",e:getFloat("TextX")+0.004)
	end
	value = 0
	while (value < e:getAbilityCount())
	do
		if (e:getGlobalFloat("selectedAbility") == value) then
			e:setTextColor(0.0,0.4,1.0,1.0)
			e:setText("ability"..value, e:getAbility(value), -0.85+e:getFloat("TextX"), 0.4-(value/5))
		else
			e:setTextColor(1.0,0.1,0.0,1.0)
			e:setText("ability"..value, e:getAbility(value), -0.9, 0.4-(value/5))
		end
		value=value+1
	end
	e:setTextColor(1.0,1.0,1.0,1.0-e:getFloat("DescX"))
	e:setText("abilityDescription", e:getString("abilityText"), -0.5, 0.0+e:getFloat("DescX"))
	e:setGlobalBool("drawPlayerUI", false)
	e:setDrawUI(true)
end
function abilityManager_eraseAbility(e)
	e:setMouseCapture(true)
	e:setImageDraw("direction", false)
	e:setDrawUI(false)
	e:setDrawScene(true)
	e:setGlobalBool("drawPlayerUI", true)
	--e:setDrawScene(true)
	e:setGlobalBool("canPlayerMove", true)
	e:setTextColor(1.0,1.0,1.0,0.0)
	e:setText("menuCloseTip", "", -0.75, 0.8)
	value = 0
	
	while (value < e:getAbilityCount())
	do
		e:setText("ability"..value, "", -0.75, 0.5-(value/5))
		value=value+1
	end
	e:setTextColor(1.0,1.0,1.0,0.0)
	e:setText("abilityDescription", "", 0.1, 0.0)
end
function abilityManager_Update(e)
	
	--setting inverted from anim frame on Time ability
	if (e:getAnimFrame() > 37 and e:getAnimFrame() < 85 and e:getGlobalFloat("selectedAbility") == 2 and e:getGlobalBool("isInMenu") == true) then
		e:setInverted(1)
	else
		e:setInverted(0)
	end
	--print("wave alpha is:" .. e:getFloat("waveAlpha"))
	if (e:doesEntityExist("spawner") == true) then
		e:setBool("isBetween", e:getGlobalBool("isBetween"))
	else
		e:setBool("isBetween", true)
	end
	if (e:getBool("isBetween") == true and e:getGlobalBool("isInMenu") == false and e:getGlobalBool("inPauseMenu") == true) then
		e:setTextColor(1.0,1.0,1.0,1.0)
		e:setText("menuTip", "Press Q to Open Ability Menu", -0.75, -0.4)
		if (e:getKeyPressed() == "Q") then
			e:setGlobalBool("isInMenu", true)
			abilityManager_drawAbility(e)
			
		end
	else
		e:setTextColor(1.0,1.0,1.0,0.0)
		e:setText("menuTip", "", -0.75, -0.4)
	end
	
	if (e:getGlobalBool("isInMenu") == true and e:getGlobalBool("inPauseMenu") == true) then
		if (e:getKeyPressed() == "ENTER") then
			e:setGlobalBool("isInMenu", false)
			e:setInverted(0)
		end
		abilityManager_drawAbility(e)
	else
		abilityManager_eraseAbility(e)
		if (e:getGlobalBool("inPauseMenu") == false) then
			e:setDrawScene(true)
		end
	end
	
	e:setCanBeHit(false)
	e:UpdateKeyPresses()
	e:setBool("lastPause", e:getGlobalBool("inPauseMenu"))
end
function abilityManager_Start(e)
    e:setHP(9999)
	e:setPos(10000,10000,10000)
	e:setFloat("secondsBetween", 10)
	e:setFloat("timer", 0.0)
	e:setGlobalFloat("wave", 0.0)
	e:setFloat("incTest", 0.0)
	e:setFloat("betweenCount", 0.0)
	e:setFloat("betweenDisplay", 0.0)
	e:setFloat("waveAlpha", 0.0)
	e:setFloat("spawnValue", 0.0)
	e:setFloat("maxValue", 1.0)
	e:setFloat("entitiesSpawned", 0)
	e:setFloat("timerLength", 2)
	e:setGlobalFloat("selectedAbility", 0)
	e:setFloat("selectCounter", 0)
	e:setBool("betweenState", false)
	e:setGlobalBool("enabled", false)
	e:setBool("isBetween", false)
	e:setGlobalBool("isInMenu", false)
	e:setGlobalBool("drawPlayerUI", true)
	e:setBool("lastPause", false)
	e:setAnimationTag("default",6)
	e:setAnimationTag("fire",0)
	e:setAnimationTag("teleport",1)
	e:setAnimationTag("time",2)
	e:setAnimationTag("speed",3)
	e:setAnimationTag("dash",4)
	e:setAnimationTag("rapid fire",5)
	e:setScale(10)
	e:setRot(0.0,0.0,0.0)
	e:showHealth(false)
	e:setUICam(true)
	e:setImageTransform(-0.4,0.7,0.55,0.33)
	e:setImage("direction", "./res/textures/menuDirections.png")
	e:setFloat("TextX",0.0)
	e:setFloat("DescX",0.0)
	e:setBool("MovingDesc", true)
	e:setString("abilityText", "")
	abilityManager_eraseAbility(e)
end

