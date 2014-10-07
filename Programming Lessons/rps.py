#########################################################################################################
# DIFFICULTY = EASY (Intended as an introduction to coding for intermediate)
#
# 1. Give them the necessary intro code (imports, initilization)
#
# 2. Explain the print functionality and make them create the opening print statements and get the input
#
# 3. Explain loops, have them set up the running loop
#
# 4. Explain if statements, help them implement the the choice
#
# 5. Explain how it should be decided who wins, and help them implement it.
#########################################################################################################

#Necessary imports
import sys
import random
running = True

#Loops through code while program is still running
while running:
    #Initial print statements
    print("Rock Paper Scissors!")
    print("Press 1 for rock.")
    print("Press 2 for paper.")
    print("Press 3 for scissors.")
    print("Press 0 to quit.")
    
    #Gets user input
    player = int(raw_input())

    print("")

    #Quits if option 0 is chosen
    if player == 0:
        break

    #Gets two numbers needed for arithmetic
    comp = random.randint(1,3)
    print("Computer played " + str(comp))

    #If statements for each choice
    if player == comp:
        print("It's a tie!")
        
    elif player == 1 and comp == 3:
        print("Player wins!")

    elif player == 3 and comp == 1:
        print("Computer wins!")

    elif player > comp:
        print("Player wins!")

    elif player < comp:
        print("Computer wins!")
        
    else:
        print("Invalid command, please try again.")
quit()
