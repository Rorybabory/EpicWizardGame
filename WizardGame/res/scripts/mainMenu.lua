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
function mainMenu_interp(e,t)
	t2 = t * t;
	return t2 * t2 * e:sin( t * 3.14 * 4.5 );
end
function mainMenu_Hit(e,e2,hits)
  
end
function mainMenu_drawStart(e)
	e:setTextColor(1.0,1.0,1.0,e:getFloat("fadeValue"))
	e:setText("startGame", "Start Game", -0.43+e:getFloat("textOffset")+e:getFloat("shakeOffset"), 0.1+e:getFloat("textVerticalOffset"))
	e:setText("playTutorial", "Play Tutorial", -0.45+e:getFloat("textOffset")+e:getFloat("shakeOffset"), -0.2+e:getFloat("textVerticalOffset"))
	e:setText("exitGame", "Quit the Game", -0.53+e:getFloat("textOffset")+e:getFloat("shakeOffset"), -0.5+e:getFloat("textVerticalOffset"))
	e:setText("options", "Open Settings Menu", -0.7+e:getFloat("textOffset")+e:getFloat("shakeOffset"), -0.8+e:getFloat("textVerticalOffset"))
	e:setTextColor(1.0,1.0,1.0,1.0)
	if (e:getFloat("selected") == 0.0) then
		e:setTextColor(1.0,0.0,0.0,e:getFloat("fadeValue"))
		e:setText("startGame", "Start Game", -0.43+e:getFloat("textOffset")+e:getFloat("shakeOffset"), 0.1+e:getFloat("textVerticalOffset"))
	elseif (e:getFloat("selected") == 1.0) then
		e:setTextColor(1.0,0.0,0.0,e:getFloat("fadeValue"))
		e:setText("playTutorial", "Play Tutorial", -0.45+e:getFloat("textOffset")+e:getFloat("shakeOffset"), -0.2+e:getFloat("textVerticalOffset"))
	elseif (e:getFloat("selected") == 2.0) then
		e:setTextColor(1.0,0.0,0.0,e:getFloat("fadeValue"))
		e:setText("exitGame", "Quit the Game", -0.53+e:getFloat("textOffset")+e:getFloat("shakeOffset"), -0.5+e:getFloat("textVerticalOffset"))
	elseif (e:getFloat("selected") == 3.0) then
		e:setTextColor(1.0,0.0,0.0,e:getFloat("fadeValue"))
		e:setText("options", "Open Settings Menu", -0.7+e:getFloat("textOffset")+e:getFloat("shakeOffset"), -0.8+e:getFloat("textVerticalOffset"))
	end
end
function mainMenu_eraseStart(e)
	e:setTextColor(1.0,1.0,1.0,0.0)
	e:setText("startGame", "Start Game", -0.43, 0.1)
	e:setText("playTutorial", "Play Tutorial", -0.45, -0.2)
	e:setText("exitGame", "Quit the Game", -0.53, -0.5)
	e:setText("options", "Open Settings Menu", -0.7, -0.8)
end
function mainMenu_UpdateCursor(e)
	if (e:getMoveDirY() ~= 0.0 or e:getArrowDirY() ~= 0.0) then
		e:setFloat("selectCount", e:getFloat("selectCount")+1)
		if (e:getFloat("selectCount") > 25) then
			e:setFloat("selectCount", 0)
			e:setFloat("selected", e:getFloat("selected")-e:getMoveDirY())
			e:setFloat("selected", e:getFloat("selected")-e:getArrowDirY())
			if (e:getFloat("selected") < 0) then
				e:setFloat("selected", 0)
			elseif (e:getFloat("selected") > 3) then
				e:setFloat("selected", 3)
			end
			e:playSound("./res/sounds/menu.wav")
		end
	else
		e:setFloat("selectCount", 25)
	end
	
end
function mainMenu_checkSelect(e, value)
	if (e:getFloat("selected") == value) then
		e:setTextColor(1.0,0.0,0.0,e:getFloat("fadeValue"))
	else
		e:setTextColor(1.0,1.0,1.0,e:getFloat("fadeValue"))
	end
end
function mainMenu_DrawOptions(e)
	mainMenu_checkSelect(e, 0)
	mainMenu_DrawResolutionText(e)
	mainMenu_checkSelect(e, 1)
	if (e:getBool("shakeScreen") == true) then
		e:setText("ScreenShake", "Screen Shake: ON", -0.9, -0.1)
	else
		e:setText("ScreenShake", "Screen Shake: OFF", -0.9, -0.1)
	end
	
	mainMenu_checkSelect(e, 2)
	e:setText("Apply", "Apply", -0.9, -0.5)
	mainMenu_checkSelect(e, 3)
	e:setText("Back", "Back", -0.9, -0.8)
end

function mainMenu_ClearOptions(e)
	e:setTextColor(1.0,1.0,1.0,0.0)
	e:setText("Resolution", "Resolution: 800 by 600", -0.9, 0.2)
	e:setText("ScreenShake", "Screen Shake: ON", -0.9, -0.1)
	e:setText("Apply", "Apply", -0.9, -0.5)
	e:setText("Back", "Back", -0.9, -0.8)
end
function mainMenu_UpdateStart(e)
	--Perform action when enter is pressed in start menu
	if (e:getKeyPressed() == "ENTER" and e:getBool("canEnterPress") == true) then
		if (e:getFloat("selected") == 0) then
			e:setMapTarget("mainMap")
		elseif (e:getFloat("selected") == 1) then
			e:setMapTarget("newMap")
		elseif (e:getFloat("selected") == 2) then
			e:stopProgram()
		elseif (e:getFloat("selected") == 3) then
			e:setBool("fadeText", true)
			e:playSound("./res/sounds/menu.wav")
		end
		e:setBool("canEnterPress", false)
	end
end
function mainMenu_DrawResolutionText(e)
	--0 is 800x600
	--1 is 1024x768
	--2 is 1280x1024
	--3 is 1600x900
	--4 is 1680x1050
	--5 is 1920x1080
	if (e:getFloat("ResolutionSetting") == 0) then
		e:setFloat("WindowWidth", 800)
		e:setFloat("WindowHeight", 600)
		e:setText("Resolution", "Resolution: 800 by 600", -0.9, 0.2)
	elseif (e:getFloat("ResolutionSetting") == 1) then
		e:setFloat("WindowWidth", 1024)
		e:setFloat("WindowHeight", 768)
		e:setText("Resolution", "Resolution: 1024 by 768", -0.9, 0.2)
	elseif (e:getFloat("ResolutionSetting") == 2) then
		e:setFloat("WindowWidth", 1280)
		e:setFloat("WindowHeight", 1024)
		e:setText("Resolution", "Resolution: 1280 by 1024", -0.9, 0.2)
	elseif (e:getFloat("ResolutionSetting") == 3) then
		e:setFloat("WindowWidth", 1600)
		e:setFloat("WindowHeight", 900)
		e:setText("Resolution", "Resolution: 1600 by 900", -0.9, 0.2)
	elseif (e:getFloat("ResolutionSetting") == 4) then
		e:setFloat("WindowWidth", 1680)
		e:setFloat("WindowHeight", 1050)
		e:setText("Resolution", "Resolution: 1680 by 1050", -0.9, 0.2)
	elseif (e:getFloat("ResolutionSetting") == 5) then
		e:setFloat("WindowWidth", 1920)
		e:setFloat("WindowHeight", 1080)
		e:setText("Resolution", "Resolution: 1920 by 1080", -0.9, 0.2)
	end
	
end
function mainMenu_UpdateOptions(e)
	--Perform action when enter is pressed in options menu
	if (e:getKeyPressed() == "ENTER" and e:getBool("canEnterPress") == true) then
		if (e:getFloat("selected") == 0) then
			e:setFloat("ResolutionSetting", e:getFloat("ResolutionSetting")+1)
			if (e:getFloat("ResolutionSetting") > 5) then
				e:setFloat("ResolutionSetting", 0)
			end
			e:playSound("./res/sounds/menu.wav")
		elseif (e:getFloat("selected") == 1) then
			e:playSound("./res/sounds/menu.wav")
			if (e:getBool("shakeScreen") == true) then
				e:setBool("shakeScreen", false)
			else
				e:setBool("shakeScreen", true)
			end
		elseif (e:getFloat("selected") == 2) then
			e:setScreenResolution(e:getFloat("WindowWidth"),e:getFloat("WindowHeight"))
			e:setScreenShake(e:getBool("shakeScreen"))
		elseif (e:getFloat("selected") == 3) then
			e:setBool("fadeText", true)
			e:playSound("./res/sounds/menu.wav")
			
		end
		e:setBool("canEnterPress", false)
	end
end
function mainMenu_Update(e)
	if (e:getBool("canWiggle") == true) then
		e:setFloat("shakeCount",e:getFloat("shakeCount")+0.2)
		e:setFloat("wiggleCount",e:getFloat("wiggleCount")+0.02)
	end
	e:setFloat("shakeOffset", ((e:sin(e:getFloat("shakeCount"))/10.0)*(e:getFloat("shake"))))
	e:setImageTransform(e:getFloat("textOffset")+e:getFloat("shakeOffset"),0.5+(e:sin(e:getFloat("wiggleCount"))/100.0),1.0,0.7)
	e:setImage("title", "./res/textures/Title.png")
	e:setImageTransform(e:getFloat("textOffset")+e:getFloat("shakeOffset"),-0.9+(e:sin(e:getFloat("wiggleCount")+124)/150.0),0.55,0.36)
	e:setImage("test", "./res/textures/menuDirections.png")

	if (e:getFloat("textOffset") > 0) then
		e:setFloat("textOffset", e:getFloat("textOffset")-0.03)
	elseif ((e:getFloat("textOffset") < 0)) then
		e:setFloat("textOffset", 0)
	elseif (e:getBool("soundPlayed")==false) then
		e:playSound("./res/sounds/menuExplode.wav")
		e:setBool("soundPlayed", true)
		e:setFloat("shake", 1.0)
		e:setBool("canWiggle", true)
	end
	e:setFloat("selectedLast", e:getFloat("selected"))
	e:UpdateKeyPresses()
	e:setCanBeHit(false)	
	--Update selected item in menu
	mainMenu_UpdateCursor(e)
	if (e:getBool("inOptions") == true) then
		mainMenu_UpdateOptions(e)
		mainMenu_eraseStart(e)
		mainMenu_DrawOptions(e)
	else
		mainMenu_UpdateStart(e)
		mainMenu_drawStart(e)
		mainMenu_ClearOptions(e)
	end
	if (e:getKeyPressed() ~= "ENTER") then
		e:setBool("canEnterPress", true)
	end
	if (e:getBool("fadeText") == true) then
		if (e:getFloat("fadeValue")>0) then
			e:setFloat("fadeValue", e:getFloat("fadeValue")-0.03 )
		else
			e:setBool("fadeText", false)
			if (e:getBool("inOptions") == false) then
				e:setBool("inOptions", true)
			else
				e:setBool("inOptions", false)
			end
		end
	else
		if (e:getFloat("fadeValue")<1.0) then
			e:setFloat("fadeValue", e:getFloat("fadeValue")+0.03)		
		end
	end
	if (e:getFloat("shake") > 0.0) then
		e:setFloat("shake", e:getFloat("shake")-0.01)
	else
		e:setFloat("shake", 0.0)
	end
end
function mainMenu_Start(e)
	print(mainMenu_interp(e,1))
    e:setHP(9999)
	e:setPos(10000,10000,10000)
	e:setFloat("tutStage", 0)
	e:setFloat("fade", 1.0)
	e:showHealth(false)
	e:setString("fading", "")
	e:setTextColor(1.0,1.0,1.0,1.0)
	e:setFloat("selectCount", 0)
	e:setBool("fadeText", false)
	e:setFloat("fadeValue", 1.0)
	e:setTextColor(1.0,0.0,0.0,1.0)
	e:setFloat("textOffset", 2)
	--Start Text
	e:setText("startGame", "Start Game", -0.43, 0.1)
	e:setTextColor(1.0,1.0,1.0,1.0)
	e:setText("playTutorial", "Play Tutorial", -0.45, -0.2)
	e:setText("exitGame", "Quit the Game", -0.53, -0.5)
	e:setText("options", "Open Settings Menu", -0.7, -0.8)
	e:setBool("inOptions", false)

	e:setImageTransform(-0.12,-0.88,0.4,0.25)
	e:setImage("test", "./res/textures/menuDirections.png")

	e:setFloat("selected", 0.0)
	e:setFloat("selectedLast", 1.0)
	
	e:setBool("soundPlayed", false)
	e:setFloat("shake", 0.0)
	
	e:setFloat("textVerticalOffset", 0.03)
	--0 is 800x600
	--1 is 1024x768
	--2 is 1280x1024
	--3 is 1600x900
	--4 is 1680x1050
	--5 is 1920x1080
	e:setFloat("ResolutionSetting", 0)
	e:setBool("shakeScreen", true)
	e:setBool("canEnterPress", true)
	e:setFloat("shakeOffset", 0.0)
	e:setFloat("shakeCount", 0)
	e:setBool("canWiggle", false)
	e:playSound("./res/sounds/menuBuildUp.wav")
end

