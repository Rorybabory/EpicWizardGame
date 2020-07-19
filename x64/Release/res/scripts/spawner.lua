spawner = {

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
function spawner_Hit(e,e2,hits)
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
function spawner_easeOutCubic(t)
    return 1 + (t-1) * t * t;
end
function spawner_oldSpawn(e)
	e:setFloat("timer", e:getFloat("timer")+1)
	if (e:getFloat("timer") > 600 and e:getEntityCount() < 10) then
		if (rand == 0) then
			e:spawnEntity("necromancer", e:random(-260,260), e:random(-260,260))
		end
		if (rand == 1) then
			e:spawnEntity("stoneMonster", e:random(-260,260), e:random(-260,260))
		end
		if (rand == 2) then
			if (e:getGlobalFloat("wave") < 5) then
				e:spawnEntity("zombie", e:random(-260,260), e:random(-260,260))
			else
				e:spawnEntity("test2", e:random(-260,260), e:random(-260,260))
			end
			
		end
		if (rand == 3 or rand == 4 or rand == 5) then
			e:spawnEntity("skeleton", e:random(-260,260), e:random(-260,260))
		end
		if (rand > 5) then
			e:spawnEntity("zombie", e:random(-260,260), e:random(-260,260))
		end
		e:setFloat("timer",0)
		e:setFloat("entitiesSpawned", e:getFloat("entitiesSpawned")+1)
	end
end
function spawner_newSpawn(e)
	print("wave is: " .. e:getGlobalFloat("wave"))
	e:setBool("isBetween", false)
	if (e:getEntityCount() < 3 and e:getBool("isInMenu") == false) then
		e:setBool("isBetween", true)
		if (e:getGlobalFloat("wave") ~= 0) then
			e:setTextColor(1.0,1.0,1.0,1.0)
			e:setText("betweenText", "Press TAB to go to next wave.", -0.75, 0.0)
			if (e:getKeyPressed() == "TAB") then
				e:setText("betweenText", "", -1.0, 0.0)
				spawner_startWave(e)
			end
		else
			spawner_startWave(e)
		end
	else
		e:setText("betweenText", "", -1.0, 0.0)
	end
end
function spawner_startWave(e)
	e:setFloat("maxValue", e:getGlobalFloat("wave"))
	e:setGlobalFloat("wave", e:getGlobalFloat("wave")+1)
	e:setFloat("waveAlphaState", 1)
	e:setFloat("spawnValue", 0.0)
	--print("setting max value to:" .. e:getFloat("maxValue"))
	if (e:getFloat("maxValue") > 10) then
		e:setFloat("maxValue", 10)
	end
end
function spawner_resolveSpawning(e)
	if (e:getFloat("spawnValue") < e:getFloat("maxValue") and e:getFloat("spawnValue") < 10) then
		e:setFloat("spawnValue", e:getFloat("spawnValue")+1)
		rand = e:randomInt(0, 16)
		if (e:getGlobalFloat("wave") < 3) then
			rand = 7
		elseif (e:getGlobalFloat("wave") > 7 and rand > 11) then
			rand = e:randomInt(0, 6)
		end
		if (rand == 0) then
			e:spawnEntity("necromancer", e:random(-260,260), e:random(-260,260))
		elseif (rand == 1) then
			e:spawnEntity("stoneMonster", e:random(-260,260), e:random(-260,260))
		elseif (rand == 2) then
			e:spawnEntity("test2", e:random(-260,260), e:random(-260,260))
		elseif (rand == 3 or rand == 4 or rand == 5) then
			e:spawnEntity("skeleton", e:random(-260,260), e:random(-260,260))
		else
			e:spawnEntity("zombie", e:random(-260,260), e:random(-260,260))
		end
	end
end
function spawner_waveAlphaState(e)
	if (e:getFloat("waveAlphaState") == 1) then
		e:setFloat("waveAlpha", spawner_easeOutCubic(e:getFloat("waveAlpha")+0.02))
		if (e:getFloat("waveAlpha") > 0.98) then
			e:setFloat("waveAlphaState", 2)
		end
	end
	if (e:getFloat("waveAlphaState") == 2) then
		e:setFloat("waveTimer", e:getFloat("waveTimer")+1)
		if (e:getFloat("waveTimer") > 50) then
			e:setFloat("waveTimer", 0)
			e:setFloat("waveAlphaState", 3)
		end
	end
	if (e:getFloat("waveAlphaState") == 3) then
		e:setFloat("waveAlpha", e:getFloat("waveAlpha")-0.02)
		if (e:getFloat("waveAlpha") < 0.02) then
			e:setFloat("waveAlphaState", 0)
		end
	end
end
function spawner_drawAbility(e)
	print(e:getAbilityCount() .. " is the ability count")
	e:setTextColor(1.0,1.0,1.0,1.0)
	e:setText("menuCloseTip", "Press LCONTROL to close", -0.75, 0.8)
	e:setGlobalBool("canPlayerMove", false)
	e:setDrawScene(false)
	--allow moving selection based on WASD
	if (e:getMoveDirY() ~= 0) then
		e:setFloat("selectCounter", e:getFloat("selectCounter")+1)
		if (e:getFloat("selectCounter") > 15) then
			e:setFloat("selectCounter", 0)
			e:setGlobalFloat("selectedAbility", e:getGlobalFloat("selectedAbility")-e:getMoveDirY())
			--push selection into bounds
			if (e:getGlobalFloat("selectedAbility") == e:getAbilityCount()) then
				e:setGlobalFloat("selectedAbility", e:getAbilityCount()-1)
			end
			if (e:getGlobalFloat("selectedAbility") < 0) then
				e:setGlobalFloat("selectedAbility", 0)
			end
			--play sound effect
			e:playSound("./res/sounds/menu.wav")
		end
	end
	value = 0
	while (value < e:getAbilityCount())
	do
		if (e:getGlobalFloat("selectedAbility") == value) then
			e:setTextColor(0.0,0.4,1.0,1.0)
		else
			e:setTextColor(1.0,0.1,0.0,1.0)
		end
		
		e:setText("ability"..value, e:getAbility(value), -0.75, 0.4-(value/5))
		value=value+1
	end
end
function spawner_eraseAbility(e)
	e:setDrawScene(true)
	e:setGlobalBool("canPlayerMove", true)
	e:setTextColor(1.0,1.0,1.0,0.0)
	e:setText("menuCloseTip", "", -0.75, 0.8)
	value = 0
	
	while (value < 5)
	do
		e:setText("ability"..value, "", -0.75, 0.5-(value/5))
		value=value+1
	end
end
function spawner_Update(e)
	if (e:getBool("isBetween") == true and e:getBool("isInMenu") == false) then
		e:setTextColor(1.0,1.0,1.0,1.0)
		e:setText("menuTip", "Press Q to Open Ability Menu", -0.75, -0.4)
		if (e:getKeyPressed() == "Q") then
			e:setBool("isInMenu", true)
		end
	else
		e:setTextColor(1.0,1.0,1.0,0.0)
		e:setText("menuTip", "", -0.75, -0.4)
	end
	
	if (e:getBool("isInMenu") == true) then
		spawner_drawAbility(e)
		if (e:getKeyPressed() == "LCTRL") then
			e:setBool("isInMenu", false)
		end
	else
		spawner_eraseAbility(e)
		
	end
	e:setCanBeHit(false)
	if (e:getBool("enabled") == true) then
		e:setTextColor(0.0,0.0,1.0,e:getFloat("waveAlpha"))
		e:setText("wave", "wave: " .. e:getGlobalFloat("wave")-1, -0.3, -0.5)
		e:setText("pressStart", "", -0.3, 0.0)
		spawner_waveAlphaState(e)
		spawner_newSpawn(e)
		spawner_resolveSpawning(e)

		print("number of entities is: " .. e:getEntityCount())
	else
		e:setTextColor(0.0,0.0,1.0,0.0)
		e:setText("wave", "wave: " .. e:getGlobalFloat("wave")-1, -0.3, -0.5)
		e:setTextColor(1.0,1.0,1.0,1.0)
		e:setText("pressStart", "Press SPACE to Start", -0.6, 0.0)
		
		if (e:getKeyPressed() == "SPACE") then
			e:setBool("enabled", true)
			print("start round")
		end
	end
	e:UpdateKeyPresses()
end
function spawner_Start(e)
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
	e:setFloat("waveAlphaState", 0)
	e:setFloat("waveTimer", 0)
	e:setFloat("betweenWaveTimer", 0)
	e:setGlobalFloat("selectedAbility", 0)
	e:setFloat("selectCounter", 0)
	e:setBool("betweenState", false)
	e:setBool("enabled", false)
	e:setBool("isBetween", false)
	
	e:setBool("isInMenu", false)
	
	e:showHealth(false)
end

