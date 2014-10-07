from time import sleep
import sys
import pygame
import random
from pygame import *
from collections import deque
from collections import namedtuple

pygame.init()
window = pygame.display.set_mode((600,600))

running = True
width = 0
drawing = False
missiles = [None]*500
levelUp = True
level = 1
Missile = namedtuple('Position', 'x, y, xSp, ySp')

current = 0
while current < 600:
    draw.rect(window, (255,255,255), (current, 600, -30, -(random.randint(1,3)*10)))
    current = current + 20
    
while running:
    for EVENT in pygame.event.get():
        if EVENT.type == MOUSEBUTTONDOWN:
            pos = pygame.mouse.get_pos()
            x = pos[0]
            y = pos[1]
            drawing = True
            red = random.randint(0,200)
            green = random.randint(0,200)
            blue = random.randint(0,200)
        elif EVENT.type == MOUSEBUTTONUP:
            draw.circle(window, (0,0,0), (x, y), width, 0)
            width = 0
            drawing = False
        elif EVENT.type == QUIT:
            running = False

    if levelUp:
        draw.rect(window, (0,0,0), (0,0,600,570))
        for count in range(0, level):
            xPos = random.randint(0, 600)
            xSpeed = random.randint(-2, 2)
            ySpeed = random.randint(1, 3)
            missiles[count] = (Missile(x = xPos, y = 0, xSp = xSpeed, ySp = ySpeed))
        levelUp = False
        remaining = level
        
    for count in range(0, level):
        if missiles[count] != None:
            current = missiles[count]
            currentX = current[0]
            currentY = current[1]
            xSpeed = current[2]
            ySpeed = current[3]
            if currentX >= 600:
                currentX = 1
            if currentX <= 0:
                 currentX = 599
            if currentY > 599:
                running = False
                print "Game over!"
                break
            if window.get_at((currentX, currentY)) == (255,0,0):
                missiles[count] = None
                remaining = remaining - 1
            else:                
                draw.circle(window, (0,255,0), (currentX, currentY), 1)
                currentX = currentX + xSpeed
                currentY = currentY + ySpeed
                missiles[count] = (Missile(x = currentX, y = currentY, xSp = xSpeed, ySp = ySpeed))

    if remaining == 0:
        levelUp = True
        level = level + 1
        
    if drawing:
        width = width + 4
        draw.circle(window, (255,0,0), (x, y), width, 0)

    if width > 50:
        draw.circle(window, (0,0,0), (x, y), width, 0)
        width = 0
        drawing = False
    
    sleep(0.02)
    display.update()
quit()
