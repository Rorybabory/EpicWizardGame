necromancer = {
  {
    componentName = "GraphicsComponent",
    folder = "./res/models/necromancer"
  },
  {
    componentName = "CollisionComponent",
    scale = {
      x = 1.0,
      y = 2.0,
      z = 1.0
    }
  },
  {
    componentName = "ProjComponent",
    model = "res/models/projectiles/fireball.obj",
    colorR = 0.028,
    colorG = 0.027,
    colorB = 0.004,
    speed = 1.0,
    range = 100,
    height = 8.0,
    delay = 96
  }
}
function necromancer_Hit(e,e2,hits)
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

function necromancer_Update(e)
    if (e:isAnimationPlaying("raise") == false) then
        e:moveForward(0.05)
        e:lookAtPlayer()
    end
    e:setFloat("raiseCount", e:getFloat("raiseCount")+1)
	e:setFloat("teleportCount", e:getFloat("teleportCount")+1)
	if (e:getFloat("teleportCount") > 720) then
		--e:Emit(100, 0.4,0.3,0.8,0.5);
		e:setPos(e:random(-50, 50)+e:getX(),0.0,e:random(-50, 50)+e:getZ())
		e:setFloat("teleportCount", 0)
	end
    if (e:getFloat("raiseCount") > 240) then
        e:setFloat("raiseCount", 0)
        e:playAnimationTag("raise")
		if (e:getEntityCount() < 12) then
			e:spawnEntity("zombie", e:getX(), e:getZ())
		end
    end
end
function necromancer_Start(e)
    e:setHP(7)
    e:lookAtPlayer()
    e:setScale(3.0)
    e:setFloat("raiseCount", 0)
    e:setFloat("teleportCount", 0)
    e:setCollisionBox(1.0,1.0,2.5)
    e:restartCollisionBox()
    e:setAnimationTag("default",0)
    e:setAnimationTag("raise",1)
    e:setAnimationTag("damaged",0)
	e:setHPColor(0.2,0.5,0.2)
	e:setFloat("scoreInc", 3000)
end
