skeleton = {
  {
    componentName = "GraphicsComponent",
    folder = "./res/models/skeleton"
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
function skeleton_Hit(e,e2,hits)
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

function skeleton_Update(e)
    e:lookAtPlayer()
	if (e:getDistanceFromNearest("player") < 5 and e:isAnimationPlaying("attack") == false and e:getBool("backup") == false) then
		e:resetFrame()
		e:playAnimationTag("attack")
	end
	if (e:isAnimationPlaying("attack") == true) then
		if (e:getDistanceFromNearest("player") < 12 and e:getAnimFrame() == 55) then
			e:damageNearestEnt("player", 2)
			e:playSound("./res/sounds/hit.wav")
			e:setBool("backup", true)
		end
	end
	if (e:getBool("backup") == true) then
		e:moveBackward(0.7)
		e:setAnimationTag("default",1)
		e:setFloat("switchMode", e:getFloat("switchMode")+1)
		if (e:getFloat("switchMode") > 240) then
			e:setBool("backup", false)
			e:setFloat("switchMode", 0);
		end
	else
		if (e:getGlobalFloat("wave") > 12) then
			e:moveForward(0.9)
		else
			e:moveForward(0.6)
		end
		
		e:setAnimationTag("default",3)
	end
	
end
function skeleton_Start(e)
    e:setHP(6)
    e:lookAtPlayer()
    e:setScale(0.8)
    e:setCollisionBox(2.0,2.0,8)
    e:restartCollisionBox()
	e:setFloat("switchMode", 0.0)
	e:setBool("backup", false)
    e:setAnimationTag("default",3)
	e:setAnimationTag("attack",0)
	e:setAnimationTag("damaged",2)
	e:setAnimationTag("backward",1)
	e:setFloat("scoreInc", 600)
end

