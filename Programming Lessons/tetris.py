from time import sleep
import sys
import pygame
from pygame import *
    
def drawShape(window, colour):
    draw.rect(window, colour, (xPos[0],yPos[0],xPos[0]+10,yPos[0]+10), 0)
    draw.rect(window, colour, (xPos[1],yPos[1],xPos[1]+10,yPos[1]+10), 0)
    draw.rect(window, colour, (xPos[2],yPos[2],xPos[2]+10,yPos[2]+10), 0)
    draw.rect(window, colour, (xPos[3],yPos[3],xPos[3]+10,yPos[3]+10), 0)
    
pygame.init()
window = pygame.display.set_mode((500, 500))
draw.rect(window, (0,0,0),(0,0,500,500),0)

XDIR = 0
YDIR = 10
pieceExists = 0;
xPos=[200,210,220,230]
yPos=[0,0,0,0]
while True:
    for EVENT in pygame.event.get():
        if EVENT.type == KEYDOWN:
            if EVENT.key == K_LEFT:
                XDIR = -10
            elif EVENT.key == K_RIGHT:
                XDIR = 10
            elif EVENT.key == K_DOWN:
                YDIR = 20
            elif EVENT.key == K_ESCAPE:
                break
            #elif EVENT.key == K_LSHIFT:
                #rotateLeft()
            #elif EVENT.key == K_LALT:
                #rotateRight()
                
    drawShape(window, (0,0,0))
    xPos[:]=[xPos[0]+XDIR,xPos[1]+XDIR,xPos[2]+XDIR,xPos[3]+XDIR]
    yPos[:]=[yPos[0]+YDIR,yPos[1]+YDIR,yPos[2]+YDIR,yPos[3]+YDIR]
    drawShape(window, (200,50,50))
    
    display.update()
    sleep(0.05)
    if yPos[0] > 500:
        quit()

#def rotateLeft():
    
#def rotateRight():
