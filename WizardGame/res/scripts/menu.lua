menu = {
  {
	componentName = "GUIComponent",
	font = "./res/Avara.ttf",
	color = {
		r = 0,
		g = 1,
		b = 0
	},
	size = 50
  }
}
function menu_Hit(e,e2,hits)
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
function menu_Update(e)
	e:setTextColor(1.0,1.0,1.0,1.0)
	e:UpdateKeyPresses()
	e:setText("pressSpace", "Press SPACE to start", -0.87, 0.0)
	if (e:getKeyPressed() == "SPACE") then
		e:spawnEntity("spawner", e:getX(), e:getZ())
		e:kill()
	end
end
function menu_Start(e)
    e:setHP(9999)
	e:setPos(10000,10000,10000)
	e:showHealth(false)
end

