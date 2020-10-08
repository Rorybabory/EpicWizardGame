spawner = {
  {
	componentName = "GUIComponent",
	font = "./res/fonts/PolygonParty.ttf",
	color = {
		r = 0,
		g = 0,
		b = 1
	},
	size = 30
  }
}
function spawner_Hit(e,e2,hits)

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
	--print("wave is: " .. e:getGlobalFloat("wave"))
	e:setGlobalBool("isBetween", false)
	if (e:doesEntityExist("abilityManager") == true) then
		e:setFloat("hasAbilities", 4)
	else
		e:setFloat("hasAbilities", 3)
	end
	
	if (e:getEntityCount() < e:getFloat("hasAbilities") and e:getGlobalBool("isInMenu") == false and e:getGlobalBool("inPauseMenu") == true) then
		e:setGlobalBool("isBetween", true)
		if (e:getGlobalFloat("wave") ~= 0 ) then
			e:setTextColor(1.0,1.0,1.0,1.0)
			e:setText("betweenText", "Press TAB to go to next wave", -0.8, 0.0)
			e:setText("betweenText2", "Press ESCAPE to pause at any time", -0.95, -0.5)
			if (e:getKeyPressed() == "TAB") then
				e:setText("betweenText", "", -1.0, 0.0)
				e:setText("betweenText2", "", -1.0, 0.0)
				spawner_startWave(e)
			end
		else
			spawner_startWave(e)
		end
	else
		e:setTextColor(1.0,1.0,1.0,0.0)
		e:setText("betweenText", "", -1.0, 0.0)
		e:setText("betweenText2", "", -1.0, 0.0)

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
function spawner_spawnFunction(e, name)
	e:setPos(e:random(-260,260), 0.0 ,e:random(-260,260))
	while (e:getDistanceBetweenTwoPointsAPI(e:getPositionFromNearestX("player"),e:getPositionFromNearestY("player"), e:getX(), e:getZ()) < 100)
	do
		print("reset  position for spawned entity\n\n\n\n")
		e:setPos(e:random(-260,260), 0.0 ,e:random(-260,260))
	end
	e:spawnEntity(name, e:getX(), e:getZ())
end
function spawner_resolveSpawning(e)
	
	if (e:getFloat("spawnValue") < e:getFloat("maxValue") and e:getFloat("spawnValue") < 10) then
		e:setFloat("spawnValue", e:getFloat("spawnValue")+1)
		rand = e:randomInt(0, 16)
		if (e:getGlobalFloat("wave") < 5) then
			rand = 7
		elseif (e:getGlobalFloat("wave") < 10) then
			rand = e:randomInt(2, 13)
		elseif (e:getGlobalFloat("wave") > 13) then
			rand = e:randomInt(0, 7)
		end
		if (rand == 0) then
			spawner_spawnFunction(e, "necromancer")
		elseif (rand == 1) then
			spawner_spawnFunction(e, "stoneMonster")
		elseif (rand == 2) then
			spawner_spawnFunction(e, "test2")
		elseif (rand == 3 or rand == 4 or rand == 5) then
			spawner_spawnFunction(e, "skeleton")
		else
			spawner_spawnFunction(e, "zombie")
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
	if (e:getFloat("waveAlpha") < 0.1) then
		e:setFloat("waveAlpha", 0)
	end
	
end
function spawner_Update(e)
	e:setCanBeHit(false)
	if (e:getGlobalBool("inPauseMenu") == false) then
		e:setTextColor(1.0,1.0,1.0,0.0)
		e:setText("betweenText", "", -1.0, 0.0)
		e:setText("betweenText2", "", -1.0, 0.0)

		print ("not drawing tab")
	end
	if (e:getGlobalBool("enabled") == true and e:getGlobalBool("inPauseMenu") == true) then
		if (e:getGlobalFloat("wave")-1 ~= 0) then
			e:setTextColor(0.0,0.0,1.0,e:getFloat("waveAlpha"))
			e:setText("wave", "wave: " .. e:getGlobalFloat("wave")-1, -0.2, -0.5)
		end
		e:setText("pressStart", "", -0.3, 0.0)
		spawner_waveAlphaState(e)
		spawner_newSpawn(e)
		spawner_resolveSpawning(e)

		--if (e:getGlobalFloat("brightness") > -2) then
		--	e:setGlobalFloat("brightness", e:getGlobalFloat("brightness")-0.05)
		--else
		--end
		
		--e:setGlobalFloat("brightness", 0)
		--print("number of entities is: " .. e:getEntityCount())
	elseif (e:getGlobalBool("inPauseMenu") == true) then
		e:setTextColor(0.0,0.0,1.0,0.0)
		e:setText("wave", "wave: " .. e:getGlobalFloat("wave")-1, -0.3, -0.5)
		e:setTextColor(1.0,1.0,1.0,1.0)
		e:setText("pressStart", "Press SPACE to Start", -0.6, 0.0)
		
		if (e:getKeyPressed() == "SPACE") then
			e:setGlobalBool("enabled", true)
			--print("start round")
		end
	else
		e:setTextColor(0.0,0.0,0.0,0.0)
		e:setText("wave", "wave: " .. e:getGlobalFloat("wave")-1, -0.3, -0.5)
		e:setText("pressStart", "Press SPACE to Start", -0.6, 0.0)
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
	e:setGlobalBool("enabled", false)
	e:setGlobalBool("isBetween", false)
	e:setBool("isInMenu", false)
	e:setGlobalBool("drawPlayerUI", true)
	
	e:setRot(0.0,0.0,0.0)
	e:showHealth(false)
	e:setUICam(true)
	
end

