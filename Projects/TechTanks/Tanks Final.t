var currentx, currenty, ychange, check, counter, circlex, circley, countercheck, pic3, titlepic, leftside, rightside, redpara, bluepara, parachuteR, parachuteB, flat2, target, reticle, targetx2,
    targety2, player, redtank, bluetankS, player1, player2, screenpointer, redtankS, angle, power, gravity, bluetank, pic2, shot, font1, x, y, b, pointerarrow, greytank, greytankS, screenpointer2,
    groundcolor, skycolor, redscore, weaponcolor, bluescore, turn, terrainchecker, enemy, speedup, yspot, startpointx, startpointy, endpointx, endpointy, mapmenupic, mult2, laser, picLaser,
    laser2, font2, exitpic, winner, leftside2, leftside3, leftside4, leftside5, freezetank, freezetankS : int
var flat, landareax, landareay, maxwep : array 1 .. 2 of int
var angle2, power2, xspeed, yspeed, currentx2, currenty2, targetx, targety : real
var siege, tankfall : array 1 .. 2 of boolean
var angle3, power3, score2 : array 1 .. 2 of string
var weaponslist : array 1 .. 2, 1 .. 4 of string
var chainhit : array 1 .. 2, 1 .. 5 of boolean
var currentweapon, score, startpositionx, startpositiony, mult3 : array 1 .. 2 of int
var explosion, gravcheck, donefall, chain : boolean
var direction, turn2, wintext : string
var gameover : boolean := false
proc redraw %Procedure used to redraw the interface, in case in explosion draws over it. Also changes the score, power, angle, and weapon of each player.
    drawline (0, 653, 0, 150, black)
    drawline (0, 653, 999, 653, black)
    drawline (999, 653, 999, 150, black)
    drawline (0, 150, 999, 150, black)
    drawline (0, 150, 0, 0, black)
    drawline (0, 0, 999, 0, black)
    drawline (999, 0, 999, 150, black)
    Pic.Draw (pic3, 0, 0, 0)
    Font.Draw (angle3 (player), 465, 70, font1, black)
    Font.Draw (power3 (player), 615, 70, font1, black)
    Font.Draw (weaponslist (player, currentweapon (player)), 120, 70, font1, black)
    score2 (1) := intstr (score (1))
    score2 (2) := intstr (score (2))
    drawfillbox (2, 650, 100, 620, skycolor)
    drawfillbox (915, 650, 998, 620, skycolor)
    Font.Draw ("Score:", 10, 635, font1, bluescore)
    Font.Draw ("Score:", 880, 635, font1, redscore)
    Font.Draw (score2 (1), 60, 635, font1, bluescore)
    Font.Draw (score2 (2), 930, 635, font1, redscore)
    drawfillbox (440, 650, 600, 620, skycolor)
    turn2 := intstr (turn)
    turn2 := "Turn:" + turn2
    if player = 1 then %Colors the turn title as color of current player
	Font.Draw (turn2, 450, 630, font2, blue)
    elsif player = 2 then
	Font.Draw (turn2, 450, 630, font2, red)
    end if
    if siege (player) = true then %Draws the tank mode button depending on what mode current player is not
	drawfillbox (10, 50, 60, 100, 30)
	drawbox (10, 50, 60, 100, black)
	Pic.Draw (greytank, 18, 65, picMerge)
    elsif siege (player) = false then
	drawfillbox (10, 50, 60, 100, 30)
	drawbox (10, 50, 60, 100, black)
	Pic.Draw (greytankS, 12, 65, picMerge)
    end if
    drawfillbox (970, 625, 1000, 655, grey)
    drawbox (970, 625, 1000, 655, black)
    Font.Draw ("X", 980, 635, font1, black)
end redraw

proc fallground %Procedure used to cause the terrain to fall due to gravity
    var nofall : boolean := true %Boolean Variable used to check if there is any terrain actually falling due to the procedure.
    donefall := true
    for xspot : (currentx - circlex) .. (currentx + circlex) %For structure used to check each column within the explosion width for terrain which may fall
	yspot := currenty - circlex
	startpointx := 0
	loop
	    if (whatdotcolor (xspot, yspot) = groundcolor and whatdotcolor (xspot, yspot - 1) = skycolor) or (whatdotcolor (xspot, yspot) = black and whatdotcolor (xspot, yspot - 1) = skycolor) then
		%Finds the bottom point at which a column of terrain will fall
		startpointx := xspot
		startpointy := yspot
		nofall := false
		donefall := false
		loop
		    if whatdotcolor (xspot, yspot) = skycolor then %Finds the top point at which a column of terrain will fall
			endpointx := xspot
			endpointy := yspot - 1
			exit
		    end if
		    yspot := yspot + 1
		end loop
		exit
	    end if
	    if yspot = currenty + circlex + 1 and startpointx = 0 then %Checks if the y position in the column is past the width of the explosion, or if no terrain fell
		exit
	    end if
	    yspot := yspot + 1
	end loop
	if nofall = false then %If terrain is falling, this changes the top pixel of a column to the skycolor, then the bottom to ground color, to symbolize gravity affecting terrain
	    drawdot (startpointx, startpointy - 2, groundcolor)
	    drawdot (startpointx, startpointy - 1, groundcolor)
	    drawdot (endpointx, endpointy - 1, skycolor)
	    drawdot (endpointx, endpointy, skycolor)
	    drawline (0, endpointy, 0, startpointy - 2, black)
	end if
    end for
end fallground

proc reticleloc %Procedure used to draw the targeting reticle to help players aim
    angle2 := strreal (angle3 (player))
    power2 := strreal (power3 (player))
    targetx := (power2 * 2) * cosd (angle2)
    targety := (power2 * 2) * sind (angle2)
    targetx2 := round (targetx)
    targety2 := round (targety)
    Sprite.Show (reticle)
    Sprite.SetHeight (reticle, 2)
    Sprite.SetPosition (reticle, landareax (player) + targetx2 + 18, landareay (player) + targety2 + 10, true)
end reticleloc

proc tankplacement %Draws the sprite of the tank, based on if it is in siege or tank mode.
    if mult3 (player) = 3 then
	if player = 1 then
	    if siege (player) = true then
		Sprite.Animate (player1, bluetankS, landareax (player) - 10, landareay (player) + 1, false)
		direction := "straight"
	    elsif siege (player) = false then
		Sprite.Animate (player1, bluetank, landareax (player), landareay (player) + 1, false)
		direction := "arc"
	    end if
	elsif player = 2 then
	    if siege (player) = true then
		Sprite.Animate (player2, redtankS, landareax (player) - 10, landareay (player) + 1, false)
		direction := "straight"
	    elsif siege (player) = false then
		Sprite.Animate (player2, redtank, landareax (player), landareay (player) + 1, false)
		direction := "arc"
	    end if
	end if
    elsif mult3 (player) = 1 then
	if player = 1 then
	    if siege (player) = true then
		Sprite.Animate (player1, freezetankS, landareax (player) - 10, landareay (player) + 1, false)
		direction := "straight"
	    elsif siege (player) = false then
		Sprite.Animate (player1, freezetank, landareax (player), landareay (player) + 1, false)
		direction := "arc"
	    end if
	elsif player = 2 then
	    if siege (player) = true then
		Sprite.Animate (player2, freezetankS, landareax (player) - 10, landareay (player) + 1, false)
		direction := "straight"
	    elsif siege (player) = false then
		Sprite.Animate (player2, freezetank, landareax (player), landareay (player) + 1, false)
		direction := "arc"
	    end if
	end if
    end if
end tankplacement

proc jaggedTerrain %Procedure used to randomly draw jagged terrain for the secondary map.
    randint (currenty, 250, 350) %Picks an initial y coordinate
    currentx := 0
    drawline (0, 653, 0, 150, black)
    drawline (0, 653, 999, 653, black)
    drawline (999, 653, 999, 150, black)
    drawline (0, 150, 999, 150, black)
    drawline (0, 150, 0, 0, black)
    drawline (0, 0, 999, 0, black)
    drawline (999, 0, 999, 150, black)
    loop
	if currentx = 170 or currentx = 829 then %Checks for certain x positions, to draw a flat plane for the tanks to sit on.
	    drawline (currentx, currenty, currentx + 65, currenty, black)
	    if player = 1 then
		landareax (1) := currentx + 15
		landareay (1) := currenty
		player := 2
	    elsif player = 2 then
		landareax (2) := currentx + 15
		landareay (2) := currenty
		player := 1
	    end if
	    currentx := currentx + 65
	else
	    randint (flat2, 1, 2)
	    if flat2 = 1 then %Generates flat terrain
		ychange := 0
	    elsif flat2 = 2 then %Generates slanted terrain
		if currenty < 240 then %If statement used to check if the randomly generated terrain is growing to high or falling too low; corrects the high accordingly if so
		    randint (ychange, 5, 10)
		elsif currenty > 350 then
		    randint (ychange, -10, -5)
		else
		    randint (ychange, -5, 5)
		end if
		drawline (currentx, currenty, currentx + 2, currenty + ychange, black)
		currentx := currentx + 2
		currenty := currenty + ychange
	    end if
	end if
	exit when currentx > 1097 %Exits when the x position has reached the end of the screen
    end loop
    drawfill (10, 160, 27, black)
    drawfill (10, 399, 53, black)
    skycolor := 53
    groundcolor := 27
    redscore := black
    bluescore := black
end jaggedTerrain

proc hillsTerrain %Procedure used to draw hills for randomly generated terrain used for the primary map.
    randint (currenty, 250, 350) %Picks an initial y coordinate
    currentx := 0
    drawline (0, 653, 0, 150, black)
    drawline (0, 653, 999, 653, black)
    drawline (999, 653, 999, 150, black)
    drawline (0, 150, 999, 150, black)
    drawline (0, 150, 0, 0, black)
    drawline (0, 0, 999, 0, black)
    drawline (999, 0, 999, 150, black)
    flat (1) := 1
    flat (2) := 1
    loop
	if currenty < 240 then %If statement used to check if the randomly generated terrain is growing to high or falling too low; corrects the high accordingly if so
	    randint (ychange, 50, 100)
	    drawline (currentx, currenty, currentx + 40, currenty + ychange, black)
	    currentx := currentx + 40
	    currenty := currenty + ychange
	elsif currenty > 350 then
	    randint (ychange, -100, -50)
	    drawline (currentx, currenty, currentx + 40, currenty + ychange, black)
	    currentx := currentx + 40
	    currenty := currenty + ychange
	else
	    randint (check, 1, 3) %Randomly generates a number to choose the type of terrain
	    if check = 1 then %If statement used to choose what type of terrain will be generated
		randint (countercheck, 1, 3)
		if countercheck = 1 then %If statement used to choose what type of hill will be generated
		    counter := 24
		    loop
			if currentx > 200 and flat (1) = 1 then %Checks to draw the tanks flat planes
			    drawline (currentx, currenty, currentx + 65, currenty, black)
			    landareax (1) := currentx + 15
			    landareay (1) := currenty
			    currentx := currentx + 65
			    flat (1) := 0
			elsif currentx > 750 and flat (2) = 1 then
			    drawline (currentx, currenty, currentx + 65, currenty, black)
			    landareax (2) := currentx + 15
			    landareay (2) := currenty
			    currentx := currentx + 65
			    flat (2) := 0
			end if
			drawline (currentx, currenty, currentx + 4, currenty + counter, black)
			currenty := currenty + counter
			currentx := currentx + 4
			counter := counter - 2
			exit when counter = -24
		    end loop
		elsif countercheck = 2 then
		    counter := 16
		    loop
			if currentx > 200 and flat (1) = 1 then %Checks to draw the tanks flat planes
			    drawline (currentx, currenty, currentx + 65, currenty, black)
			    landareax (1) := currentx + 15
			    landareay (1) := currenty
			    currentx := currentx + 65
			    flat (1) := 0
			elsif currentx > 750 and flat (2) = 1 then
			    drawline (currentx, currenty, currentx + 65, currenty, black)
			    landareax (2) := currentx + 15
			    landareay (2) := currenty
			    currentx := currentx + 65
			    flat (2) := 0
			end if
			drawline (currentx, currenty, currentx + 4, currenty + counter, black)
			currenty := currenty + counter
			currentx := currentx + 4
			counter := counter - 4
			exit when counter = -16
		    end loop
		elsif countercheck = 3 then
		    counter := 10
		    loop
			if currentx > 200 and flat (1) = 1 then %Checks to draw the tanks flat planes
			    drawline (currentx, currenty, currentx + 65, currenty, black)
			    landareax (1) := currentx + 15
			    landareay (1) := currenty
			    currentx := currentx + 65
			    flat (1) := 0
			elsif currentx > 750 and flat (2) = 1 then
			    drawline (currentx, currenty, currentx + 65, currenty, black)
			    landareax (2) := currentx + 15
			    landareay (2) := currenty
			    currentx := currentx + 65
			    flat (2) := 0
			end if
			drawline (currentx, currenty, currentx + 10, currenty + counter, black)
			currenty := currenty + counter
			currentx := currentx + 10
			counter := counter - 1
			exit when counter = -10
		    end loop
		end if
	    else
		if currentx > 200 and flat (1) = 1 then %Checks to draw the tanks flat planes
		    drawline (currentx, currenty, currentx + 65, currenty, black)
		    landareax (1) := currentx + 15
		    landareay (1) := currenty
		    currentx := currentx + 65
		    flat (1) := 0
		elsif currentx > 750 and flat (2) = 1 then
		    drawline (currentx, currenty, currentx + 65, currenty, black)
		    landareax (2) := currentx + 15
		    landareay (2) := currenty
		    currentx := currentx + 65
		    flat (2) := 0
		end if
		randint (ychange, -50, 50)
		drawline (currentx, currenty, currentx + 40, currenty + ychange, black)
		currentx := currentx + 40
		currenty := currenty + ychange
	    end if
	end if
	exit when currentx > 998 %Exits when the xposition reaches the end of the screen
    end loop
    drawfill (10, 499, 101, black)
    skycolor := 101
    groundcolor := white
    redscore := black
    bluescore := black
end hillsTerrain

proc exitscreen %Exit screen for when user leaves program
    View.Set ("nooffscreenonly")
    View.Set ("graphics:379;388")
    exitpic := Pic.FileNew ("pics/Exit Wallpaper.bmp")
    Pic.Draw (exitpic, 0, 0, 0)
    if gameover = true then
	Font.Draw (wintext, 150, 280, font2, winner)
    elsif gameover = false then
	Music.PlayFileStop
    end if
end exitscreen

View.Set ("graphics:666;500")
titlepic := Pic.FileNew ("pics/Main Menu.jpg")
Pic.Draw (titlepic, 0, 0, 0)
player := 1
enemy := 2
speedup := 150
turn := 1
loop %Mousewhere loop used to continually check for mouse position
    mousewhere (x, y, b)
    if x > 41 and x < 167 and y > 281 and y < 337 and b = 1 then %Checks which option the user selects
	View.Set ("graphics:666;500")
	mapmenupic := Pic.FileNew ("pics/Map Menu Screen.bmp")
	Pic.Draw (mapmenupic, 0, 0, 0)
	loop
	    mousewhere (x, y, b)
	    if x > 1 and x < 210 and y > 90 and y < 190 and b = 1 then %Checks which map the user selects
		terrainchecker := 2
		exit
	    elsif x > 413 and x < 618 and y > 385 and y < 478 and b = 1 then
		terrainchecker := 1
		exit
	    end if
	end loop
	exit
    elsif x > 627 and x < 660 and y > 470 and y < 500 and b = 1 then
	exitscreen
	return
    end if
end loop
View.Set ("graphics:1000;655")
View.Set ("offscreenonly")
if terrainchecker = 1 then
    jaggedTerrain
elsif terrainchecker = 2 then
    hillsTerrain
end if

pic3 := Pic.FileNew ("pics/interface.bmp")
bluetank := Pic.FileNew ("pics/Tank(Blue).bmp")
redtank := Pic.FileNew ("pics/Tank(Red).bmp")
bluetankS := Pic.FileNew ("pics/Tank(Blue) Siege.bmp")
redtankS := Pic.FileNew ("pics/Tank(Red) Siege.bmp")
screenpointer := Pic.FileNew ("pics/Blue Arrow.bmp")
screenpointer2 := Pic.FileNew ("pics/Red Arrow.bmp")
greytank := Pic.FileNew ("pics/Tank(Grey).bmp")
greytankS := Pic.FileNew ("pics/Tank(Grey) Siege.bmp")
freezetank := Pic.FileNew ("pics/Tank(Frozen).bmp")
freezetankS := Pic.FileNew ("pics/Tank(Frozen) Siege.bmp")
parachuteR := Pic.FileNew ("pics/Parachute Red.bmp")
parachuteB := Pic.FileNew ("pics/Parachute Blue.bmp")
target := Pic.FileNew ("pics/Reticle.bmp")
reticle := Sprite.New (target)
redpara := Sprite.New (parachuteR)
bluepara := Sprite.New (parachuteB)

Pic.Draw (pic3, 0, 0, 0)
player1 := Sprite.New (bluetank)
player2 := Sprite.New (redtank)
Sprite.SetPosition (player1, landareax (1), landareay (1) + 1, false)
Sprite.SetPosition (player2, landareax (2), landareay (2) + 1, false)
Sprite.SetHeight (player1, 2)
Sprite.SetHeight (player2, 2)
Sprite.Show (player1)
Sprite.Show (player2)
View.Update

for z : 1 .. 2 %Sets weapons
    for w : 1 .. 4
	if w = 1 then
	    weaponslist (z, w) := "Magma Shot"
	elsif w = 2 then
	    weaponslist (z, w) := "Confuse Shot"
	elsif w = 3 then
	    weaponslist (z, w) := "Frost Shot"
	end if
    end for
    score (z) := 0
    mult3 (z) := 3
end for
currentweapon (1) := 1
currentweapon (2) := 1
direction := "arc"
font1 := Font.New ("Arial:12")
font2 := Font.New ("Arial:16")
pointerarrow := Sprite.New (screenpointer)
maxwep (1) := 3
maxwep (2) := 3
angle3 (1) := "60"
power3 (1) := "15"
angle3 (2) := "120"
power3 (2) := "15"
siege (1) := false
siege (2) := false
Music.PlayFileLoop ("music/Tank Music 1.mp3") %In game Music File
loop %Loop repeated for each turn of each player
    if turn = 5 then %Checks to see is the player has earned the orbital bombardment weapon
	chain := true
	for gg : 1 .. 4 %Checks if turn to see if player hit enemy
	    if chainhit (player, gg) = false then %Disables the ability to obtain special weapon if miss shot occured
		chain := false
	    end if
	end for
	if chain = true then %If each shot was hit, sets all variables to cause next shot to be orbital bombardment
	    weaponslist (player, 1) := "Orbital Bombardment"
	    maxwep (player) := 1
	    currentweapon (player) := 1
	end if
    end if
    redraw
    tankplacement
    reticleloc
    View.Update
    loop %Mousewhere loop used to check if the mouse clicks on any interface buttons
	mousewhere (x, y, b)
	if x > 700 and x < 950 and y > 50 and y < 100 and b = 1 then %Player presses fire button, exiting mousewhere loop
	    exit
	elsif x > 425 and x < 450 and y > 57 and y < 93 and b = 1 then %Player increases angle
	    if angle3 (player) = "0" then
		angle3 (player) := "359"
		if speedup not= 20 then
		    speedup := speedup - 10
		end if
		delay (speedup)
		drawfillbox (450, 50, 500, 100, 30)
		drawbox (450, 50, 500, 100, black)
	    else
		angle2 := strreal (angle3 (player))
		angle2 := angle2 - 1
		angle3 (player) := realstr (angle2, 1)
		if speedup not= 20 then
		    speedup := speedup - 10
		end if
		delay (speedup)
		drawfillbox (450, 50, 500, 100, 30)
		drawbox (450, 50, 500, 100, black)
	    end if
	    redraw
	    reticleloc
	    tankplacement
	    View.Update
	elsif x > 500 and x < 525 and y > 57 and y < 93 and b = 1 then %Player decreases angle
	    if angle3 (player) = "359" then
		angle3 (player) := "0"
		if speedup not= 20 then
		    speedup := speedup - 10
		end if
		delay (speedup)
		drawfillbox (450, 50, 500, 100, 30)
		drawbox (450, 50, 500, 100, black)
	    else
		angle2 := strreal (angle3 (player))
		angle2 := angle2 + 1
		angle3 (player) := realstr (angle2, 1)
		if speedup not= 20 then
		    speedup := speedup - 10
		end if
		delay (speedup)
		drawfillbox (450, 50, 500, 100, 30)
		drawbox (450, 50, 500, 100, black)
	    end if
	    redraw
	    reticleloc
	    tankplacement
	    View.Update
	elsif x > 575 and x < 600 and y > 57 and y < 93 and b = 1 then %Player decreases power
	    if power3 (player) not= "1" and direction = "arc" or power3 (player) not= "10" and direction = "straight" then
		power2 := strreal (power3 (player))
		power2 := power2 - 1
		power3 (player) := realstr (power2, 1)
		if speedup not= 20 then
		    speedup := speedup - 10
		end if
		delay (speedup)
		drawfillbox (600, 50, 650, 100, 30)
		drawbox (600, 50, 650, 100, black)
	    end if
	    redraw
	    reticleloc
	    tankplacement
	    View.Update
	elsif x > 651 and x < 675 and y > 57 and y < 93 and b = 1 then %Player increases power
	    if power3 (player) not= "30" then
		power2 := strreal (power3 (player))
		power2 := power2 + 1
		power3 (player) := realstr (power2, 1)
		if speedup not= 20 then
		    speedup := speedup - 10
		end if
		delay (speedup)
		drawfillbox (600, 50, 650, 100, 30)
		drawbox (600, 50, 650, 100, black)
	    end if
	    redraw
	    reticleloc
	    tankplacement
	    View.Update
	elsif x > 10 and x < 60 and y > 57 and y < 93 and b = 1 then %Player switches tank stances
	    if siege (player) = false then
		siege (player) := true
	    elsif siege (player) = true then
		siege (player) := false
	    end if
	    delay (500)
	    redraw
	    reticleloc
	    tankplacement
	    View.Update
	elsif x > 85 and x < 110 and y > 57 and y < 93 and b = 1 then %Player switches weapons to the right
	    if currentweapon (player) > 1 then
		currentweapon (player) := currentweapon (player) - 1
		delay (150)
	    end if
	    redraw
	    reticleloc
	    tankplacement
	    View.Update
	elsif x > 341 and x < 365 and y > 57 and y < 93 and b = 1 then %Player switches weapons to the left
	    if currentweapon (player) < maxwep (player) then
		currentweapon (player) := currentweapon (player) + 1
		delay (150)
	    end if
	    redraw
	    reticleloc
	    tankplacement
	    View.Update
	elsif x > 970 and x < 1000 and y > 625 and y < 655 and b = 1 then
	    exitscreen
	    return
	elsif b = 0 then %If player releases the mouse button, the rate of change of the angle/power restore to their slowest rate
	    speedup := 150
	end if
    end loop
    if weaponslist (player, currentweapon (player)) = "Magma Shot" then
	%Selects the image to represent the sprite + the explosion color, based on weapon. Also gives a bonus multiplier to using the standard magma shot
	pic2 := Pic.FileNew ("pics/bullet.bmp")
	weaponcolor := 4
	mult2 := 2
    elsif weaponslist (player, currentweapon (player)) = "Frost Shot" then
	pic2 := Pic.FileNew ("pics/bulletFrost.bmp")
	weaponcolor := 11
	mult2 := 1
    elsif weaponslist (player, currentweapon (player)) = "Confuse Shot" then
	pic2 := Pic.FileNew ("pics/bulletConfuse.bmp")
	mult2 := 1
    elsif weaponslist (player, currentweapon (player)) = "Orbital Bombardment" then
	pic2 := Pic.FileNew ("pics/bulletFlare.bmp")
    end if
    shot := Sprite.New (pic2)
    Sprite.SetHeight (shot, 1)
    Sprite.SetHeight (pointerarrow, 1)
    View.Update
    angle2 := strreal (angle3 (player))
    power2 := strreal (power3 (player))
    currentx := landareax ((player)) + 18
    currenty := landareay ((player)) + 10
    currentx2 := intreal (currentx)
    currenty2 := intreal (currenty)
    xspeed := power2 * cosd (angle2)
    yspeed := power2 * sind (angle2)
    gravity := 0
    gravcheck := true
    loop
	currentx := round (currentx2)
	currenty := round (currenty2)
	if currenty < 655 then %If shot is above view range for user, shows an arrow to represent its x coordinate. Otherwise, draws the projectile
	    Sprite.SetPosition (shot, currentx, currenty, true)
	    Sprite.Show (shot)
	    Sprite.Hide (pointerarrow)
	elsif currenty > 655 then
	    Sprite.SetPosition (shot, currentx, currenty, true)
	    if player = 2 then
		Sprite.ChangePic (pointerarrow, screenpointer2)
	    elsif player = 1 then
		Sprite.ChangePic (pointerarrow, screenpointer)
	    end if
	    Sprite.SetPosition (pointerarrow, currentx - 13, 637, true)
	    Sprite.Show (pointerarrow)
	end if
	if direction = "arc" then %If the weapon is shot with an arc(tank mode) the shot will fall due to gravity
	    if gravcheck = true then %Check used to slow the rate at which gravity causes the object to fall
		gravity := gravity + 1
		gravcheck := false
	    elsif gravcheck = false then
		gravcheck := true
	    end if
	end if
	View.Update
	Time.DelaySinceLast (25)
	if direction = "arc" then %Goes through all possible ways for an arced shot to end its projectile motion
	    if currenty < 645 then %Checks if the shot is viewable on screen
		if currentx < 1 or currentx > 998 then %Checks if the object exited the left or right side of the screen, exits if so
		    explosion := false
		    exit
		elsif whatdotcolor (currentx, currenty) = groundcolor or currenty < 155 or currentx > landareax (enemy) and currentx < landareax (enemy) + 36 and currenty > landareay (enemy) and
			currenty < landareay (enemy) + 24 then
		    %Checks if the shot hits the minimum ground area (top of interface), hits the ground or if it hits the player,exits if so
		    explosion := true
		    exit
		end if
	    elsif currenty > 645 then
		if currentx < 1 or currentx > 998 then %Checks if the object exited the left or right side of the screen, exits if so
		    explosion := false
		    exit
		end if
	    end if
	elsif direction = "straight" then %Goes through all possible ways for a straight shot to end its projectile motion
	    if currenty < 645 then %Checks if the shot is viewable on screen
		if currentx < 1 or currentx > 998 then %Checks if the object exited the left or right side of the screen, exits if so
		    explosion := false
		    exit
		elsif currenty < 155 or currentx > landareax (enemy) and currentx < landareax (enemy) + 36 and currenty > landareay (enemy) and currenty < landareay (enemy) + 24 then
		    %Checks if the shot hit the minimum ground area(top of interface) or hit the player, exits if so
		    explosion := true
		    exit
		end if
	    elsif currenty > 645 then %Checks if shot reached the top of the screen, exits if so
		explosion := false
		exit
	    end if
	end if
	currentx2 := currentx2 + xspeed
	currenty2 := currenty2 + yspeed - gravity
    end loop
    Sprite.Hide (shot)
    Sprite.Hide (pointerarrow)
    if weaponslist (player, currentweapon (player)) = "Orbital Bombardment" then %Checks if the user has achieved a more powerful attack through hitting each earlier attack
	picLaser := Pic.FileNew ("pics/Laser.bmp")
	laser := Sprite.New (picLaser)
	laser2 := Sprite.New (picLaser)
	Sprite.Show (laser)
	Sprite.Show (laser2)
	Sprite.SetHeight (laser, 1)
	Sprite.SetHeight (laser2, 1)
	counter := 0
	loop %Loop structure used to increase score and animate the laser downwards
	    counter := counter + 1
	    Sprite.SetPosition (laser, currentx, 608 - counter, true)
	    Sprite.SetPosition (laser2, currentx, 42 - counter, true)
	    redraw
	    if abs (currentx - (landareax (enemy) + 18)) < 20 then %Checks if the laser is hitting the tank, and adds points to score if so
		score (player) := score (player) + 2
	    end if
	    delay (20)
	    exit when counter = 75
	end loop
	Sprite.Hide (laser)
	Sprite.Hide (laser2)
    else
	if explosion = true then %Checks if there was an explosion which would have made a change to increases player score
	    circlex := 1
	    circley := 1
	    loop %Loop used to draw expanding explosion
		if weaponslist (player, currentweapon (player)) = "Confuse Shot" then %Checks if shot fired was "Confuse Shot", and alternates colors each frame if so
		    randint (weaponcolor, 1, 10)
		end if
		drawfilloval (currentx, currenty, circlex, circley, weaponcolor)
		delay (20)
		circlex := circlex + 1
		circley := circley + 1
		exit when (circlex = 40 and (weaponslist (player, currentweapon (player)) = "Magma Shot" or weaponslist (player, currentweapon (player)) = "Confuse Shot")) or circlex = 28 and
		    weaponslist (player, currentweapon (player)) = "Frost Shot" %Exits when each weapon has hit its appropriate explosion radius
	    end loop
	    drawfilloval (currentx, currenty, circlex, circley, skycolor)
	    leftside := (currentx - (landareax (enemy) + 14)) ** 2 + (currenty - (landareay (enemy) + 5)) ** 2
	    leftside2 := (currentx - landareax (enemy)) ** 2 + (currenty - landareay (enemy)) ** 2
	    leftside3 := (currentx - landareax (enemy)) ** 2 + (currenty - (landareay (enemy) + 10)) ** 2
	    leftside4 := (currentx - (landareax (enemy) + 30)) ** 2 + (currenty - landareay (enemy)) ** 2
	    leftside5 := (currentx - (landareax (enemy) + 30)) ** 2 + (currenty - (landareay (enemy) + 10)) ** 2
	    rightside := circlex ** 2 %Calculates the area of the explosion
	    if rightside > leftside or rightside > leftside2 or rightside > leftside3 or rightside > leftside4 or rightside > leftside5 then %Checks if enemy point is within explosion area
		score (player) := score (player) + 5 * mult2 * mult3 (player)
		if weaponslist (player, currentweapon (player)) = "Confuse Shot" then %Checks if shot fired was "Confuse Shot", if so, randomizes enemy power and angle
		    if player = 1 then
			power := strint (power3 (2))
			angle := strint (angle3 (2))
			randint (angle, 1, 359)
			randint (power, 10, 30)
			power3 (2) := intstr (power)
			angle3 (2) := intstr (angle)
		    elsif player = 2 then
			power := strint (power3 (1))
			angle := strint (angle3 (1))
			randint (angle, 1, 359)
			randint (power, 10, 30)
			power3 (1) := intstr (power)
			angle3 (1) := intstr (angle)
		    end if
		elsif weaponslist (player, currentweapon (player)) = "Frost Shot" then %Checks if shot fired was "Frost Shot", if so, lowers enemy multiplier to 33%
		    if player = 1 then
			mult3 (2) := 1
			if siege (enemy) = false then
			    Sprite.ChangePic (player2, freezetank)
			elsif siege (enemy) = true then
			    Sprite.ChangePic (player2, freezetankS)
			end if
		    elsif player = 2 then
			mult3 (1) := 1
			if siege (enemy) = false then
			    Sprite.ChangePic (player1, freezetank)
			elsif siege (enemy) = true then
			    Sprite.ChangePic (player1, freezetankS)
			end if
		    end if
		end if
		chainhit (player, turn) := true
	    else
		chainhit (player, turn) := false
	    end if
	else
	    chainhit (player, turn) := false
	end if
    end if
    if mult3 (player) = 1 then %If the player was frozen(hit by frost shot) 100% multiplier is now restored
	mult3 (player) := 3
	if player = 1 then
	    if siege (player) = false then
		Sprite.ChangePic (player1, bluetank)
	    elsif siege (player) = true then
		Sprite.ChangePic (player1, bluetankS)
	    end if
	elsif player = 2 then
	    if siege (player) = false then
		Sprite.ChangePic (player2, redtank)
	    elsif siege (player) = true then
		Sprite.ChangePic (player2, redtankS)
	    end if
	end if
    end if
    redraw
    if explosion = true then %Checks if an explosion happened, resulting in the chance of displacing terrain
	tankfall (1) := false
	tankfall (2) := false
	donefall := false
	loop %Loop used to cause terrain and tanks to fall simultaneously
	    if donefall = false then %If terrain is not done falling, calls to fallground procedure to shift terrain downwards
		fallground
	    end if
	    if tankfall (1) = false then %If tank 1 is not done falling, starts action of causing tank to fall, and deploy parachute
		if whatdotcolor (landareax (1) + 18, landareay (1) - 1) not= groundcolor and whatdotcolor (landareax (1) + 18, landareay (1) - 1) not= black then %Checks if pixel below tank is sky
		    Sprite.Show (bluepara)
		    Sprite.SetHeight (bluepara, 1)
		    Sprite.SetPosition (bluepara, landareax (1) + 18, landareay (1) + 20, true)
		    landareay (1) := landareay (1) - 2
		    if siege (1) = false then
			Sprite.SetPosition (player1, landareax (1), landareay (1), false)
		    elsif siege (1) = true then
			Sprite.SetPosition (player1, landareax (1) - 10, landareay (1), false)
		    end if
		    if donefall = true then %Causes a minor delay to program in case ground is done falling, and program processes faster
			Time.DelaySinceLast (35)
		    end if
		    View.Update
		elsif whatdotcolor (landareax (1) + 18, landareay (1) - 1) = groundcolor or whatdotcolor (landareax (1) + 18, landareay (1) - 1) = black then
		    Sprite.Hide (bluepara)
		    tankfall (1) := true
		end if
	    end if
	    if tankfall (2) = false then %If tank 2 is not done falling, starts action of causing tank to fall, and deploy parachute
		if whatdotcolor (landareax (2) + 18, landareay (2) - 1) not= groundcolor and whatdotcolor (landareax (2) + 18, landareay (2) - 1) not= black then %Checks if pixel below tank is sky
		    Sprite.Show (redpara)
		    Sprite.SetHeight (redpara, 1)
		    Sprite.SetPosition (redpara, landareax (2) + 18, landareay (2) + 20, true)
		    landareay (2) := landareay (2) - 2
		    if siege (2) = false then
			Sprite.SetPosition (player2, landareax (2), landareay (2), false)
		    elsif siege (2) = true then
			Sprite.SetPosition (player2, landareax (2) - 10, landareay (2), false)
		    end if
		    if donefall = true then %Causes a minor delay to program in case ground is done falling, and program processes faster
			Time.DelaySinceLast (35)
		    end if
		    View.Update
		elsif whatdotcolor (landareax (2) + 18, landareay (2) - 1) = groundcolor or whatdotcolor (landareax (2) + 18, landareay (2) - 1) = black then
		    Sprite.Hide (redpara)
		    tankfall (2) := true
		end if
	    end if
	    exit when tankfall (1) = true and tankfall (2) = true and donefall = true %Exits when all three possible things (terrain, tank 1, tank 2) have all obeyed gravity and landed
	end loop
    end if
    if player = 1 then %Ends turn, switching the current player and current enemy, as well as increasing the turn by one when player two finishes their turn
	enemy := 1
	player := 2
    elsif player = 2 then
	enemy := 2
	player := 1
	turn := turn + 1
    end if
    redraw
    exit when turn = 6
end loop

if score (1) > score (2) then %If statement used to decide the outcome of the match
    wintext := "Blue Wins!"
    winner := blue
elsif score (1) < score (2) then
    wintext := "Red Wins!"
    winner := red
elsif score (1) = score (2) then
    wintext := "It's a Tie!"
    winner := black
end if
exitscreen
