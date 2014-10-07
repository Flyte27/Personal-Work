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
# 5. Explain casting and why it is necessary, help them implement it
#########################################################################################################

#Necessary imports
import sys

running = True

#Loops through code while program is still running
while running:
    #Initial print statements
    print("Please select a command.")
    print("Press 1 for addition.")
    print("Press 2 for subtraction.")
    print("Press 3 for multiplication.")
    print("Press 4 for division")
    print("Press 0 to quit")
    #Gets user input
    choice = raw_input()

    #Quits if option 0 is chosen
    if int(choice) == 0:
        break

    #Gets two numbers needed for arithmetic
    print("Please input the first number.")
    first = raw_input()

    print("Please input the second number.")
    second = raw_input()

    #If statements for each choice
    if int(choice) == 1:
        print(first + " + " + second + " = " + str(float(first)+float(second)))

    elif int(choice) == 2:
        print(first + " - " + second + " = " + str(float(first)-float(second)))

    elif int(choice) == 3:
        print(first + " * " + second + " = " + str(float(first)*float(second)))

    elif int(choice) == 4:
        print(first + " / " + second + " = " + str(float(first)+float(second)))
        
    else:
        print("Invalid command, please try again.")
quit()
