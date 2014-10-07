#########################################################################################################
# DIFFICULTY = VERY HARD(Only for advanced, probably shouldnt do this unless you thoroughly understand the code)
#
# 1. Give them the necessary intro code (imports, initilization)
#
# 2. Explain the draw.rect functionality and make them draw the main window
#
# 3. Explain loops, have them implement the movement of a single circle with the pygame.event code (given)
#
# 4. Have them draw a trail behind their moving circle using draw.rect
#
# 5. Implement collision detection which causes the program to quit if the player hits tail or wall
#########################################################################################################

#Necessary imports
from time import sleep
import sys
import pygame
import random
from pygame import *
from collections import namedtuple
from collections import deque

#Initializes pygame and sets window to be the screen
pygame.init()
window = pygame.display.set_mode((500, 400))

#Draws the screen
draw.rect(window, (0,0,255),(0,0,500,400),0)
draw.rect(window, (255,255,255),(10,10,480,380),0)

#Initializes all variables (positions, directions, and boolean running)
XPOS = 305
YPOS = 165
targetX = 10*(random.randint(1,48))+5
targetY = 10*(random.randint(1,28))+5
XDIR = 0
YDIR = -10
running = True
eat = True
Position = namedtuple('Position', 'x, y')
tail = deque([])

#While running is true, program continues
while running:
    #Checks for any key presses
    for EVENT in pygame.event.get():
        if EVENT.type == KEYDOWN:
            #Checks if W key is pressed
            if EVENT.key == K_w:
                YDIR = -10
                XDIR = 0
            #Checks if A key is pressed
            elif EVENT.key == K_a:
                YDIR = 0
                XDIR = -10
            #Checks if S key is pressed
            elif EVENT.key == K_s:
                YDIR = 10
                XDIR = 0
            #Checks if D key is pressed
            elif EVENT.key == K_d:
                YDIR = 0
                XDIR = 10
        #Checks if user closed the window
        elif EVENT.type == QUIT:
            running = False

    #Moves player 1
    XPOS = XPOS + XDIR
    YPOS = YPOS + YDIR

    #Adds the current position to the tail
    tail.append(Position(x = XPOS, y = YPOS))

    #Ends program if player 1 hits a wall
    if window.get_at((XPOS,YPOS)) == (0,0,255):
        running = False

    #If the player eats the target, picks a new position for target
    if (XPOS == targetX and YPOS == targetY):
        eat = True
        targetX = 10*(random.randint(1,48))+5
        targetY = 10*(random.randint(1,28))+5
        
    #Draws the food
    draw.circle(window, (0,255,0), (targetX,targetY), 5, 0)
    
    #Draws the player
    draw.circle(window, (255,0,0), (XPOS,YPOS), 5, 0)
    display.update()

    #Delays by 50 milliseconds
    sleep(0.05)

    #Draws the tail behind each player
    draw.circle(window, (0,0,255), (XPOS,YPOS), 5, 0)

    #If the target was not eaten, erase the end of tail by popping it off the queue and drawing it as white
    if eat == False:
        end = tail.popleft()
        draw.circle(window, (255,255,255), (end.x,end.y),5, 0)

    eat = False
    display.update()
quit()
