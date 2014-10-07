from time import sleep
import sys
import pygame
import random
from pygame import *

pygame.init()
window = pygame.display.set_mode((500, 500))
draw.rect(window, (0,0,0), (0,0,500,500),0)
draw.rect(window, (255,255,255),(5,5,490,490),0)

running = True
width = 0
drawing = False


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
            width = 0
            drawing = False
        elif EVENT.type == QUIT:
            running = False

    if drawing:
        width = width + 1
        draw.rect(window, (red,green,blue), (x-width, y-width, width*2, width*2), 0)

    sleep(0.01)
    display.update()
quit()
