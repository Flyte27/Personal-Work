from time import sleep
import sys
import pygame
import random
import math
from pygame import *

pygame.init()
window = pygame.display.set_mode((1000, 1000))
draw.rect(window, (255,255,255), (0,0,1000,1000),0)

running = True
posX = 500
posY = 500
dirX = 0.0
dirY = 3.0
red = random.randint(0,200)
green = random.randint(0,200)
blue = random.randint(0,200)
myfont = pygame.font.SysFont("monospace", 15)

planetX = random.randint(250, 750)
planetY = random.randint(250, 750)
red = random.randint(0,200)
green = random.randint(0,200)
blue = random.randint(0,200)
draw.circle(window, (red, green, blue), (planetX, planetY), 15)
            
while running:
    for EVENT in pygame.event.get():
        if EVENT.type == MOUSEBUTTONDOWN:
            draw.rect(window, (255,255,255), (0, 0, 1000, 1000))
            pos = pygame.mouse.get_pos()
            planetX = pos[0]
            planetY = pos[1]
            red = random.randint(0,200)
            green = random.randint(0,200)
            blue = random.randint(0,200)
            draw.circle(window, (red, green, blue), (planetX, planetY), 15)
        elif EVENT.type == QUIT:
            running = False
            
    draw.rect(window, (255,255,255), (480, 0, 80, 15))
    draw.rect(window, (0,0,0), (480, 0, 80, 15), 1)
    label = myfont.render(str(int(posX))+", "+str(int(posY)), 1, (0,0,0))
    window.blit(label, (480, 0))

    distX = abs(planetX - posX)
    distY = abs(planetY - posY)
    radius = math.sqrt(distX**2 + distY**2)
    gravity = 1000/radius**2

    if posY < planetY:
        dirY = dirY + gravity
    else:
        dirY = dirY - gravity
        
    if posX < planetX:
        dirX = dirX + gravity
    else:
        dirX = dirX - gravity

    posX = posX + dirX
    posY = posY + dirY

    if posX > 0 and posX < 1000 and posY > 0 and posY < 1000:
        if window.get_at((int(posX), int(posY))) == (red, green, blue):
            running = False
    
    draw.circle(window, (0, 0, 0), (int(posX), int(posY)), 3)
    display.update()
    sleep(0.02)
    
quit()
