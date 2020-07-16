hand = {
  {
    componentName = "GraphicsComponent",
    folder = "./res/models/hand"
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
function hand_Hit(e,e2,hits)
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

function hand_Update(e)
	e:setPos(-1, -1, 1);
	e:setRot(-1.57, 0, 0);
	if (e:getPlayerTag() == "fire") then
		e:playAnimationTag("fire");
		e:setPlayerTag("");
	end
	e:setCanBeHit(false)

end
function hand_Start(e)
    e:setHP(5)
    e:setScale(0.6)
    e:setFloat("raiseCount", 0)
    e:setCollisionBox(3.0,3.0,7.5)
    e:restartCollisionBox()
    e:setAnimationTag("default",1)
    e:setAnimationTag("fire",0)
	e:setColor(0.0,0.3,1.0,1.0)
end

