from time import sleep
import sys
import pygame
import random
from pygame import *

pygame.init()
window = pygame.display.set_mode((500, 500))
draw.rect(window, (0,0,0), (0,0,500,500),0)
draw.rect(window, (255,255,255),(15,15,470,470),0)

running = True
x = 0
y = 0
width = 0
drawing = False
red = 0
green = 0
blue = 0
posX = 260
posY = 250
dirX = 2
dirY = -2
size = 0
sizeUp = 0
timer = 5000
numClicks = 0
greenX = random.randint(30, 455)
greenY = random.randint(30, 455)
win = False

while running:
    for EVENT in pygame.event.get():
        if EVENT.type == MOUSEBUTTONDOWN:
            if EVENT.button == 1:
                size = 0
                sizeUp = 0
                widthUp = 1
            elif EVENT.button == 3:
                width = 5
                size = 1
                sizeUp = 2
                widthUp = 0
            pos = pygame.mouse.get_pos()
            x = pos[0]
            y = pos[1]
            drawing = True
            red = random.randint(0,200)
            green = random.randint(0,200)
            blue = random.randint(0,200)
            numClicks = numClicks + 1
        elif EVENT.type == MOUSEBUTTONUP:
            width = 0
            drawing = False
        elif EVENT.type == QUIT:
            running = False
    
    if drawing:
        width = width + widthUp
        draw.rect(window, (red,green,blue), (x-width, y-width, width*2, width*2), size)

    posX = posX + dirX
    posY = posY + dirY

    if window.get_at((posX, posY)) == (0,255,0):
        win = True
        running = False
    
    if window.get_at((posX, posY+dirY-1)) != (255,255,255) and window.get_at((posX, posY+dirY-1)) != (0,0,255) and window.get_at((posX, posY+dirY-1)) != (0,255,0):
        dirY = -dirY
    if window.get_at((posX+dirX, posY)) != (255,255,255) and window.get_at((posX+dirX, posY)) != (0,0,255) and window.get_at((posX+dirX, posY)) != (0,255,0):
        dirX = -dirX

    draw.circle(window, (0,0,255), (posX, posY), 1, 0)
    draw.circle(window, (0,255,0), (greenX, greenY), 5)
    sleep(0.01)
    display.update()
    size = size + sizeUp
    timer = timer - 1
    if timer == 0:
        print "Game over, out of time!"
        running = False
quit()

if win:
    print "Time left: " + str(timer)
    print "Number of clicks made: " + str(numClicks)
    finalScore = timer/numClicks
    print "Final score: " + str(finalScore)
