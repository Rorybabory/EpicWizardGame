target = {
  {
    componentName = "GraphicsComponent",
    folder = "./res/models/target"
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
function target_Hit(e,e2,hits)
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

function target_Update(e)
    e:lookAtPlayer()
	e:moveForward(0.1)

end
function target_Start(e)
    e:setHP(10)
    e:lookAtPlayer()
    e:setScale(5.2)
    e:setCollisionBox(0.6,0.6,1.6)
    e:restartCollisionBox()
    e:setAnimationTag("default", 0)
	e:setHPColor(1.0,1.0,1.0)
end

