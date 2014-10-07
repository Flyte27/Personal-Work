from time import sleep
import sys
import pygame
import random
from pygame import *

pygame.init()
window = pygame.display.set_mode((1000, 1000))
draw.rect(window, (255,255,255), (0,0,1000,1000),0)

running = True
posX = 500
posY = 500
dirX = 0.0
dirY = 3.0
click = False

while running:
    for EVENT in pygame.event.get():
        if EVENT.type == QUIT:
            running = False
            
    pos = pygame.mouse.get_pos()
    mouseX = pos[0]
    mouseY = pos[1]

    random.randint(0, 255)
    
    if posY < mouseY:
        dirY = dirY + 0.05
    else:
        dirY = dirY - 0.05
        
    if posX < mouseX:
        dirX = dirX + 0.05
    else:
        dirX = dirX - 0.05

    red = random.randint(0,200)
    green = random.randint(0,200)
    blue = random.randint(0,200)

    posX = posX + dirX
    posY = posY + dirY

    draw.circle(window, (red, green, blue), (int(posX), int(posY)), 4)
    display.update()
    sleep(0.02)
    
quit()
