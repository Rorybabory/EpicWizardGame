fire = {
  {
    componentName = "GraphicsComponent",
    folder = "./res/models/fire"
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
function fire_Hit(e,e2,hits)
  if (e2:getType() == "player") then
	
  else
	e2:setHP(e2:getHP()-hits)
	e2:write(e2:getHP())
	e2:setColorFlash(1.0,0.3,0.0)
  end
  
end

function fire_Update(e)
    e:setFloat("Count", e:getFloat("Count")+1)
	if (e:getFloat("Count") > 120) then
		e:kill()
	end
	if (e:getDistanceFromNearestNot("player") < 12) then
		e:damageNearest(3)
	end

end
function fire_Start(e)
    e:setHP(9999)
    e:lookAtPlayer()
    e:setScale(4.0)
    e:setCollisionBox(0.01,0.01,0.01)
    e:restartCollisionBox()
	e:setFloat("Count", 0)
    e:setAnimationTag("default",0)
	e:showHealth(false)
end

