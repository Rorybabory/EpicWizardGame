spawner = {}
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

function spawner_Update(e)
	e:setFloat("timer", e:getFloat("timer")+1)
	print(e:getFloat("entitiesSpawned"))
	
	if (e:getFloat("timer") > 300) then
		rand = e:randomInt(0, 15)
		if (rand == 0) then
			e:spawnEntity("necromancer", e:random(-260,260), e:random(-260,260))
		end
		if (rand == 1 or rand == 2) then
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
function spawner_Start(e)
    e:setHP(9999)
	e:setPos(10000,10000,10000)
	e:setFloat("timer", 0.0)
	e:setFloat("entitiesSpawned", 0)
	e:setFloat("timerLength", 2)
	e:showHealth(false)
end

