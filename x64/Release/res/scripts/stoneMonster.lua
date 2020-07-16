stoneMonster = {
  {
    componentName = "GraphicsComponent",
    folder = "./res/models/stoneMonster"
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
function stoneMonster_Hit(e,e2,hits)
  -- e2:setColor(0.0,0.0,0.0,1.0)
  e2:setHP(e2:getHP()-hits)
  -- if (e:getAnimation() == 2) then
  --   e:playAnimation(0)
  -- end
  if (e2:isPlayer() == true) then
    e2:Shake(20.0)
  end
  e2:write(e2:getHP())
end

function stoneMonster_Update(e)
	if (e:getString("state") == "walk") then
		e:moveForward(0.2)
		e:lookAtPlayer()
		e:setAnimationTag("default",e:getFloat("walk"))
		e:setFloat("walkToJump", e:getFloat("walkToJump")+1)
		if (e:getFloat("walkToJump") > 238) then
			e:resetFrame()
			e:setString("state", "jump")
			e:setFloat("walkToJump", 0)
		end
		e:setCanBeHit(true)
	end
	if (e:getString("state") == "bwalk") then
		e:moveBackward(0.6)
		e:lookAtPlayer()
		e:setAnimationTag("default",e:getFloat("bwalk"))
	end
	if (e:getString("state") == "jump") then
		e:setAnimationTag("default",e:getFloat("jump"))
		if (e:getAnimFrame() > 40 and e:getAnimFrame() < 145) then
			e:moveForward(e:getDistanceFromNearest("player")/40)
			--e:setCollide(false)
			e:setCanBeHit(false)
			if (e:getDistanceFromNearest("player") < 40 and e:getAnimFrame() == 144) then
				e:damageNearestEnt("player", 4)
				
			end
		else
			e:lookAtPlayer()
			--e:setCollide(true)
			e:setCanBeHit(true)
		end
		e:setFloat("jumpToWalk", e:getFloat("jumpToWalk")+1)
		if (e:getFloat("jumpToWalk") > 159) then
			e:setString("state", "walk")
			e:setFloat("jumpToWalk", 0)
		end
	end
	--e:setAnimationTag("damaged",e:getAnimationID("default"))
end
function stoneMonster_Start(e)
    e:setHP(15)
    e:lookAtPlayer()
    e:setScale(2.8)
    e:setCollisionBox(3,3,6.0)
    e:restartCollisionBox()
	e:setString("state","walk");
	e:setFloat("bwalk", 0)
	e:setFloat("idle", 1)
	e:setFloat("jump", 2)
	e:setFloat("walk", 3)
    e:setAnimationTag("default",0)
	e:setFloat("walkToJump", 0)
	e:setFloat("jumpToWalk", 0)
	e:setFloat("scoreInc", 1200)

end

