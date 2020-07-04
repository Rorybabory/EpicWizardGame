slime = {
  {
    componentName = "GraphicsComponent",
    folder = "./res/models/slime"
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
function slime_Hit(e,e2,hits)
  -- e2:setColor(0.0,0.0,0.0,1.0)
  e2:setHP(e2:getHP()-1)
  -- if (e:getAnimation() == 2) then
  --   e:playAnimation(0)
  -- end
  if (e2:isPlayer() == true) then
    e2:Shake(5.0)
  end
  e2:write(e2:getHP())
end

function slime_Update(e)
    e:moveForward(0.4)
    e:lookAtPlayer()
	if (e:getAnimation() == 1 and e:getAnimFrame() == 55) then
		e:playAnimationTag("fire")
	end
	--print(e:getAnimFrame())
end
function slime_Start(e)
    e:setHP(5)
    e:lookAtPlayer()
    e:setScale(2.8+e:random(0,0.2))
    e:setCollisionBox(1.5,1.5,1.5)
    e:restartCollisionBox()
    e:setAnimationTag("default",1)
    e:setAnimationTag("damage",1)
	e:setAnimationTag("fire",2)
	--e:playAnimationTag("spawn")
	e:setBool("hasStarted", false)
end

