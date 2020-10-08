tutorial = {
  {
	componentName = "GUIComponent",
	font = "./res/fonts/PolygonParty.ttf",
	color = {
		r = 0,
		g = 0,
		b = 1
	},
	size = 30
  }
}
function tutorial_Hit(e,e2,hits)
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

function tutorial_Update(e)
	e:setGlobalBool("canPlayerMove", true)
	e:setTextColor(1.0,1.0,1.0,e:getFloat("fade"))
	--print("text fade is: " .. e:getFloat("fade") .. " and fade state is: " .. e:getString("fade"))
	if (e:getString("lastTutText") ~= e:getString("tutText")) then
		e:setString("fading", "out")
		e:setString("fadeOutText", e:getString("lastTutText"))
	end
	if (e:getString("fading") == "in") then
		e:setFloat("fade", e:getFloat("fade")+0.01)
		e:setText("tutorial", e:getString("tutText"), -0.85, 0.1)
		if (e:getFloat("fade") > 1.0) then
			e:setString("fading", "")
		end
	elseif (e:getString("fading") == "out") then
		e:setFloat("fade", e:getFloat("fade")-0.01)
		e:setText("tutorial", e:getString("fadeOutText"), -0.85, 0.1)

		if (e:getFloat("fade") < 0.01) then
			e:setText("tutorial", e:getString("tutText"), -0.85, 0.1)
			e:setString("fading", "in")
		end
	end
	e:setString("lastTutText", e:getString("tutText"))
	if (e:getFloat("tutStage") == 0) then
		if (e:getKeyPressed() == "ENTER") then
			e:setFloat("tutStage", 1)
			e:setString("tutText", "Great! Now hold down \nSPACE to shoot!")
		end
	elseif (e:getFloat("tutStage") == 1) then
		if (e:getKeyPressed() == "SPACE") then
			e:setFloat("tutStage", 2)
			e:setString("tutText", "You can now hold SHIFT \nto use your ability\nThe default ability \ndamages any enemies \nnearby. You can change it later.")
		end
	elseif (e:getFloat("tutStage") == 2) then
		if (e:getKeyPressed() == "LSHIFT") then
			e:setFloat("tutStage", 3)
			e:setString("tutText", "Now fight this target")
			e:spawnEntity("target", e:getX(), e:getZ())
		end
	elseif (e:getFloat("tutStage") == 3) then
		if (e:getEntityCount() < 3) then
			e:setString("tutText", "You finished the tutorial!\nNow press TAB to start \na game!")
			if (e:getKeyPressed() == "TAB") then
				e:setMapTarget("mainMap")
			end
		end
	end
	e:UpdateKeyPresses();
	e:setCanBeHit(false)

end
function tutorial_Start(e)
    e:setHP(9999)
	e:setPos(10000,10000,10000)
	e:setFloat("tutStage", 0)
	e:setFloat("fade", 1.0)
	e:setString("lastTutText", "Hello! Use WASD to move\nMouse to look around\npress ENTER to continue")
	e:setString("tutText", "Hello! Use WASD to move\nMouse to look around\npress ENTER to continue")
	e:setString("fadeOutText", "")
	e:showHealth(false)
	e:setString("fading", "")
	e:setTextColor(1.0,1.0,1.0,1.0)
	e:setText("tutorial", e:getString("tutText"), -0.85, 0.1)
	e:setDrawScene(true)
end

