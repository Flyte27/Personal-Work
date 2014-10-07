#Simon Says game, needs comments but the basic code works
#Might be too much draw.rect's (bit overwhelming).

from time import sleep
import pygame
import random
from pygame import *
from collections import deque
from random import randint

pygame.init()
window = pygame.display.set_mode((500, 500))
draw.rect(window, (0,0,0), (0,0,500,500), 0)
draw.rect(window, (0,0,255), (10,10,240,240), 0)
draw.rect(window, (0,255,0), (250,10,240,240), 0)
draw.rect(window, (255,0,0), (10,250,240,240), 0)
draw.rect(window, (255,0,255), (250,250,240,240), 0)
draw.rect(window, (0,0,0), (225,225,50,50), 0)
display.update()

running = True
patternMade = False
level = 1
timer = 300
queue = deque([])
while running:
    for event in pygame.event.get():
        if event.type == MOUSEBUTTONDOWN:
            pos = pygame.mouse.get_pos()
            x = pos[0]
            y = pos[1]
            
            if x >= 0 and x < 250 and y >= 0 and y < 250:
                draw.rect(window, (225,225,225), (10,10,240,240), 0)
                draw.rect(window, (0,0,0), (225,225,50,50), 0)
                display.update()
                choice = 1
                if choice != queue.popleft():
                    running = False
                else:
                    if not queue:
                        patternMade = False
                        level = level + 1
                    timer = 300
                sleep(0.4)
                draw.rect(window, (0,0,255), (10,10,240,240), 0)

            elif x >= 250 and x < 500 and y >= 0 and y < 250:
                draw.rect(window, (225,225,225), (250,10,240,240), 0)
                draw.rect(window, (0,0,0), (225,225,50,50), 0)
                display.update()
                choice = 2
                if choice != queue.popleft():
                    running = False
                else:
                    if not queue:
                        patternMade = False
                        level = level + 1
                    timer = 300
                sleep(0.4)
                draw.rect(window, (0,255,0), (250,10,240,240), 0)

            elif x >= 0 and x < 250 and y >= 250 and y < 500:
                draw.rect(window, (225,225,225), (10,250,240,240), 0)
                draw.rect(window, (0,0,0), (225,225,50,50), 0)
                display.update()
                choice = 3
                if choice != queue.popleft():
                    running = False
                else:
                    if not queue:
                        patternMade = False
                        level = level + 1
                    timer = 300
                sleep(0.4)
                draw.rect(window, (255,0,0), (10,250,240,240), 0)

            elif x >= 250 and x < 500 and y >= 250 and y < 500:
                draw.rect(window, (225,225,225), (250,250,240,240), 0)
                draw.rect(window, (0,0,0), (225,225,50,50), 0)
                display.update()
                choice = 4
                if choice != queue.popleft():
                    running = False
                else:
                    if not queue:
                        patternMade = False
                        level = level + 1
                    timer = 300
                sleep(0.4)
                draw.rect(window, (255,0,255), (250,250,240,240), 0)
                
            draw.rect(window, (0,0,0), (225,225,50,50), 0)
            display.update()

        elif event.type == QUIT:
            running = False
    
    if patternMade == False:
        for counter in range(0,level):
            sleep(0.3)
            choice = randint(1,4)
            queue.append(choice)
            if choice == 1:
                draw.rect(window, (225,225,225), (10,10,240,240), 0)
            elif choice == 2:
                draw.rect(window, (225,225,225), (250,10,240,240), 0)
            elif choice == 3:
                draw.rect(window, (225,225,225), (10,250,240,240), 0)
            elif choice == 4:
                draw.rect(window, (225,225,225), (250,250,240,240), 0)

            draw.rect(window, (0,0,0), (225,225,50,50), 0)
            display.update()
            sleep(0.4)
            
            draw.rect(window, (0,0,255), (10,10,240,240), 0)
            draw.rect(window, (0,255,0), (250,10,240,240), 0)
            draw.rect(window, (255,0,0), (10,250,240,240), 0)
            draw.rect(window, (255,0,255), (250,250,240,240), 0)
            draw.rect(window, (0,0,0), (225,225,50,50), 0)

            display.update()
        patternMade = True
    timer = timer - 1
    sleep(0.01)
    if timer == 0:
        running = False
    display.update()
quit()
