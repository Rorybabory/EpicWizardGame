zombie = {
  {
    componentName = "GraphicsComponent",
    folder = "./res/models/basicZombie"
  },
  {
    componentName = "CollisionComponent",
    scale = {
      x = 0.3,
      y = 0.6,
      z = 0.3
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
function zombie_Hit(e,e2,hits)
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

function zombie_Update(e)
    e:lookAtPlayer()
	if (e:getDistanceFromNearest("player") < 5 and e:isAnimationPlaying("attack") == false) then
		e:resetFrame()
		e:playAnimationTag("attack")
		print("attack")
	end
	if (e:isAnimationPlaying("attack") == true) then
		if (e:getDistanceFromNearest("player") < 16 and e:getAnimFrame() == 55) then
			e:damageNearestEnt("player", 3)
		end
	end
	e:moveForward(0.45)
	e:setColor(1.0,1.0,1.0,1.0)

end
function zombie_Start(e)
    e:setHP(4)
    e:lookAtPlayer()
    e:setScale(1.2+e:random(0,0.2))
    e:setFloat("raiseCount", 0)
    e:setCollisionBox(1.5,1.5,4.2)
    e:restartCollisionBox()
    e:setAnimationTag("default",2)
	e:setAnimationTag("attack",0)
    e:setAnimationTag("damaged",1)
	e:setFloat("scoreInc", 100)
	e:setHPColor(0.2,0.5,0.2)
end

