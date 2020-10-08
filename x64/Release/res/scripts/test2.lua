test2 = {
  {
    componentName = "GraphicsComponent",
    folder = "./res/models/spider"
  },
  {
    componentName = "CollisionComponent",
    scale = {
      x = 4.0,
      y = 2.0,
      z = 2.0
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
test2_distance = 0.0
function test2_Hit(e,e2,hits)
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

function test2_Update(e)
  if (e:getFloat("SpawnCount") >= 100) then
    --e:spawnEntity("smallSpider", e:getX(), e:getZ())
    e:setFloat("SpawnCount",0)
  end
  e:setFloat("SpawnCount", e:getFloat("SpawnCount")+1)

  if (e:isAnimationPlaying("damaged") == false and e:isAnimationPlaying("attack") == false) then
    e:moveForward(0.8)
    e:lookAtPlayer()
  end
  if (e:getDistanceFromNearest("player") < 20 and e:isAnimationPlaying("attack") == false and e:getAnimFrame() > 25) then
    e:playAnimationTag("attack")
    test2_Hit(e,e:getNearestEntWithName("player"),2)
	e:playSound("./res/sounds/hit.wav")
  end
end
function test2_Start(e)
  e:setFloat("SpawnCount", 0)
  e:setHP(6)
  e:lookAtPlayer()
  e:setScale(3.0)
  e:write("RAN START")
  e:setColorFlash(0.0,0.0,0.0,0.0)
  e:setCollisionBox(5.0,2.0,2.0)
  e:restartCollisionBox()
  e:setDamageAnimation(1)
  e:setAnimationTag("default",3)
  e:setAnimationTag("damaged",1)
  e:setAnimationTag("attack",0)
  e:setFloat("scoreInc", 1000)
  e:setBool("isCounting", false)
end
