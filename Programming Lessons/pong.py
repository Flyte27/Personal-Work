#########################################################################################################
# DIFFICULTY = EASY-MEDIUM (Probably best suited for intermediate)
#
# 1. Give them the necessary intro code (imports, initilization)
#
# 2. Explain loops, have them implement the movement of the paddle with the pygame.event code (given) and draw.rect
#
# 3. Implement movement of the ball with XPOS, YPOS, XDIR, YDIR variables
#
# 5. Implement collision detection which causes the ball to bounce if it hits a wall, or the paddle. End program if it exits bottom of the screen
#
# 6. Freestyle a bit! You can make it a two player VS game, a two player coop game, create objects that the ball bounces off of, or keep score!
#########################################################################################################

#Necessary Imports
from time import sleep
import sys
import pygame
from pygame import *

#Initializes pygame and sets window to be the screen
pygame.init()
window = pygame.display.set_mode((1000, 800))

#Initializes all variables(positions, directions, and boolean running)
XPOS = 300
YPOS = 200
XDIR = -1
YDIR = 1
p1 = 50
p1Dir = 0
running = True

#While running is true, program continues
while running:
    #Checks for any key presses
    for EVENT in pygame.event.get():
        if EVENT.type == KEYDOWN:
            #Checks for left key press, and moves paddle left
            if pygame.key.get_pressed()[K_LEFT]:
                if p1 > 0:
                    p1Dir = -2
            #Checks for right key press, and moves paddle right
            elif pygame.key.get_pressed()[K_RIGHT]:
                if p1 < 900:
                    p1Dir = 2
        elif EVENT.type == QUIT:
                running = False
        #Checks for key release and stops paddle movement
        elif EVENT.type == KEYUP:
            p1Dir = 0
        
    #Moves the ball
    XPOS = XPOS + XDIR
    YPOS = YPOS + YDIR

    #Moves the paddle
    p1 = p1 + p1Dir

    #Ball reverses X direction upon hitting right side of screen
    if XPOS >= 995:
        XDIR = -XDIR

    #Ball reverses X direction upon hitting left side of screen
    if XPOS <= 5:
        XDIR = -XDIR

    #Ball reverses Y direction upon hitting top of screen
    if YPOS <= 5:
        YDIR = -YDIR

    #Ball reverses Y direction upon hitting the paddle
    if YPOS >= 780 and YPOS <= 785 and XPOS >= p1 and XPOS <= p1 + 100:
        YDIR = -YDIR

    #Game ends if ball passes paddle
    if YPOS >= 800:
        running = False

    #Delays by 2 milliseconds
    sleep(0.002)

    #Clears the screen
    window.fill((255,255,255))
    #Draws the ball
    draw.circle(window, (200,50,50), (XPOS,YPOS), 10, 0)
    #Draws the paddle
    draw.rect(window, (50,50,200), (p1, 790, 100, 10), 0)
    #Updates the screen
    display.flip()
quit()
