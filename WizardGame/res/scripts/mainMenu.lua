mainMenu = {
  {
	componentName = "GUIComponent",
	font = "./res/fonts/CaslonAntique.ttf",
	color = {
		r = 0,
		g = 0,
		b = 1
	},
	size = 60
  }
}
function mainMenu_Hit(e,e2,hits)
  
end

function mainMenu_Update(e)
	e:setFloat("selectedLast", e:getFloat("selected"))
	e:UpdateKeyPresses()
	e:setCanBeHit(false)
	--
	--Perform action when enter is pressed
	if (e:getKeyPressed() == "ENTER") then
		if (e:getFloat("selected") == 0) then
			e:setMapTarget("mainMap")
		elseif (e:getFloat("selected") == 1) then
			e:setMapTarget("newMap")
		elseif (e:getFloat("selected") == 2) then
			e:stopProgram()
		end
	end
	
	--Update Key presses
	if (e:getMoveDirY() ~= 0.0 or e:getArrowDirY() ~= 0.0) then
		e:setFloat("selectCount", e:getFloat("selectCount")+1)
		if (e:getFloat("selectCount") > 25) then
			e:setFloat("selectCount", 0)
			e:setFloat("selected", e:getFloat("selected")-e:getMoveDirY())
			e:setFloat("selected", e:getFloat("selected")-e:getArrowDirY())
			if (e:getFloat("selected") < 0) then
				e:setFloat("selected", 0)
			elseif (e:getFloat("selected") > 2) then
				e:setFloat("selected", 2)
			end
			e:playSound("./res/sounds/menu.wav")
		end
	else
		e:setFloat("selectCount", 25)
	end
	
	--Update selected item in menu
	if (e:getFloat("selected") ~= e:getFloat("selectedLast")) then
		e:setTextColor(1.0,1.0,1.0,1.0)
		e:setText("startGame", "Start Game", -0.43, -0.1)
		e:setText("playTutorial", "Play Tutorial", -0.45, -0.4)
		e:setText("exitGame", "Quit the Game", -0.53, -0.7)
		if (e:getFloat("selected") == 0.0) then
			e:setTextColor(1.0,0.0,0.0,1.0)
			e:setText("startGame", "Start Game", -0.43, -0.1)
		elseif (e:getFloat("selected") == 1.0) then
			e:setTextColor(1.0,0.0,0.0,1.0)
			e:setText("playTutorial", "Play Tutorial", -0.45, -0.4)
		elseif (e:getFloat("selected") == 2.0) then
			e:setTextColor(1.0,0.0,0.0,1.0)
			e:setText("exitGame", "Quit the Game", -0.53, -0.7)
		end
	end
	
end
function mainMenu_Start(e)
    e:setHP(9999)
	e:setPos(10000,10000,10000)
	e:setFloat("tutStage", 0)
	e:setFloat("fade", 1.0)
	e:showHealth(false)
	e:setString("fading", "")
	e:setTextColor(1.0,1.0,1.0,1.0)
	e:setFloat("selectCount", 0)
	e:setText("gameName", "Wizard Game!!", -0.54, 0.65)
	e:setTextColor(1.0,0.0,0.0,1.0)
	e:setText("startGame", "Start Game", -0.43, -0.1)
	e:setTextColor(1.0,1.0,1.0,1.0)
	e:setText("playTutorial", "Play Tutorial", -0.45, -0.4)
	e:setText("exitGame", "Quit the Game", -0.53, -0.7)
	e:setFloat("selected", 0.0)
	e:setFloat("selectedLast", 1.0)
	e:setImageTransform(0.0,-0.88,0.80,0.5)
	e:setImage("test", "./res/textures/menuDirections.png")
end

