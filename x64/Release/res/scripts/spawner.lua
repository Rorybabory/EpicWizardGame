spawner = {
  {
	componentName = "GUIComponent",
	font = "./res/Avara.ttf",
	color = {
		r = 0,
		g = 1,
		b = 0
	},
	size = 25
  },
  {
	componentName = "GUIComponent",
	font = "./res/Avara.ttf",
	color = {
		r = 0,
		g = 0,
		b = 1
	},
	size = 35
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
		rand = e:randomInt(0, 15)
		if (rand == 0) then
			e:spawnEntity("necromancer", e:random(-260,260), e:random(-260,260))
		end
		if (rand == 1) then
			e:spawnEntity("stoneMonster", e:random(-260,260), e:random(-260,260))
		end
		if (rand == 2) then
			e:spawnEntity("test2", e:random(-260,260), e:random(-260,260))
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
	if (e:getEntityCount() < 3) then
		e:setFloat("maxValue", e:getGlobalFloat("wave"))
		e:setGlobalFloat("wave", e:getGlobalFloat("wave")+1)
		e:setFloat("waveAlphaState", 1)

		e:setFloat("spawnValue", 0.0)
		--print("setting max value to:" .. e:getFloat("maxValue"))
		if (e:getFloat("maxValue") > 10) then
			e:setFloat("maxValue", 10)
		end
	end
end
function spawner_resolveSpawning(e)
	if (e:getFloat("spawnValue") < e:getFloat("maxValue") and e:getFloat("spawnValue") < 10) then
		e:setFloat("spawnValue", e:getFloat("spawnValue")+1)
		rand = e:randomInt(0, 16)
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
function spawner_Update(e)
	e:setTextColor(0.0,0.0,1.0,e:getFloat("waveAlpha"))
	e:setText("wave", "wave: " .. e:getGlobalFloat("wave")-1, -0.3, -0.5)
	spawner_waveAlphaState(e)
	e:setFloat("timer", e:getFloat("timer")+1)
	if (e:getFloat("timer")>20) then
		spawner_newSpawn(e)
		spawner_resolveSpawning(e)
		e:setFloat("timer", 0)
	end
	print("number of entities is: " .. e:getEntityCount())
end
function spawner_Start(e)
    e:setHP(9999)
	e:setPos(10000,10000,10000)
	e:setFloat("timer", 0.0)
	e:setGlobalFloat("wave", 0.0)
	e:setFloat("waveAlpha", 0.0)
	e:setFloat("spawnValue", 0.0)
	e:setFloat("maxValue", 1.0)
	e:setFloat("entitiesSpawned", 0)
	e:setFloat("timerLength", 2)
	e:setFloat("waveAlphaState", 0)
	e:setFloat("waveTimer", 0)
	e:setFloat("betweenWaveTimer", 0)
	e:setBool("betweenState", false)
	e:showHealth(false)
end

