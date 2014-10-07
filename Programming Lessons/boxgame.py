#########################################################################################################
# DIFFICULTY = MEDIUM (Basics meant for intermediate, advanced additions can be done)
#
# 1. Give them the necessary intro code (imports, initilization)
#
# 2. Explain the draw.rect functionality and make them draw the main window
#
# 3. Explain the random generator, and have them randomly generate and draw a box on the screen
#
# 4. Have them loop through the code checking for mouse clicks, and comparing mouse clicks with if statements
#
# 5. Check if mouse clicked inside a box. If so, generate a new box location.
#
# 6. (Advanced) Have the boxes shrink in size (by using a decreasing variable), have a red box that shouldnt be clicked,
#       Have an order for boxes to be clicked. Keep score, etc. 
#########################################################################################################

#Necessary imports
from time import sleep
import sys
import pygame
import random
from pygame import *

#Initializes pygame and sets window to be the screen
pygame.init()
window = pygame.display.set_mode((500, 500))
draw.rect(window, (0,0,0), (0,0,500,500),0)
draw.rect(window, (255,255,255),(10,10,480,480),0)

#Initializes all variables (positions, directions, and boolean running)
running = True
x = 0
y = 0
boxX = random.randint(40,410)
boxY = random.randint(40,410)
width = 50

#While running is true, program continues
while running:
    #Checks for any key presses
    for EVENT in pygame.event.get():
        if EVENT.type == MOUSEBUTTONDOWN:
            pos = pygame.mouse.get_pos()
            x = pos[0]
            y = pos[1]
        elif EVENT.type == QUIT:
            running = False

    #Draws the box
    draw.rect(window, (0,0,255), (boxX,boxY,width,width), 0)
    if x > boxX and x < boxX+50 and y > boxY and y < boxY+50:
        draw.rect(window, (255,255,255), (boxX, boxY, 50, 50), 0)
        boxX = random.randint(40,410)
        boxY = random.randint(40,410)
        width = 50
    
    #Delays by 50 milliseconds
    sleep(0.05)
    display.update()
quit()
