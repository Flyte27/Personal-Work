#########################################################################################################
# DIFFICULTY = MEDIUM
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
#
# 6. Get them to add second player (same as first player, just different event keys and colour)
#########################################################################################################

#Necessary imports
from time import sleep
import sys
import pygame
from pygame import *

#Initializes pygame and sets window to be the screen
pygame.init()
window = pygame.display.set_mode((1000, 600))

#Draws the screen
draw.rect(window, (0,0,255),(0,0,1000,600),0)
draw.rect(window, (255,255,255),(10,10,980,580),0)

#Initializes all variables (positions, directions, and boolean running)
XPOS = 405
YPOS = 365
XPOS2 = 605
YPOS2 = 365
XDIR = 0
YDIR = -10
XDIR2 = 0
YDIR2 = -10
running = True

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
            #Checks if up arrow is pressed
            elif EVENT.key == K_UP:
                YDIR2 = -10
                XDIR2 = 0
            #Checks if left arrow is pressed
            elif EVENT.key == K_LEFT:
                YDIR2 = 0
                XDIR2 = -10
            #Checks if down arrow is pressed
            elif EVENT.key == K_DOWN:
                YDIR2 = 10
                XDIR2 = 0
            #Checks if right arrow is pressed
            elif EVENT.key == K_RIGHT:
                YDIR2 = 0
                XDIR2 = 10
        #Checks if user closed the window
        elif EVENT.type == QUIT:
            running = False

    #Moves player 1
    XPOS = XPOS + XDIR
    YPOS = YPOS + YDIR

    #Moves player 2
    XPOS2 = XPOS2 + XDIR2
    YPOS2 = YPOS2 + YDIR2

    #Ends program if player 1 hits a wall
    if window.get_at((XPOS,YPOS)) == (0,0,255):
        running = False

    #Ends program if player 2 hits a wall
    if window.get_at((XPOS2, YPOS2)) == (0,0,255):
        running = False

    #Draws each player
    draw.circle(window, (255,0,0), (XPOS,YPOS), 5, 0)
    draw.circle(window, (0,255,0), (XPOS2, YPOS2), 5, 0)
    display.update()

    #Delays by 50 milliseconds
    sleep(0.05)

    #Draws the tail behind each player
    draw.circle(window, (0,0,255), (XPOS,YPOS),5, 0)
    draw.circle(window, (0,0,255), (XPOS2,YPOS2),5,0)
    display.update()
quit()
