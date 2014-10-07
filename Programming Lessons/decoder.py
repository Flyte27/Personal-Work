print "Welcome to the decoder."

while True:
    print "Press 1 to encode, 2 to decode, 3 to exit."
    choice = raw_input()
    if int(choice) == 1:
        print "Please input message to encode."
        message = raw_input().lower()
        newMessage = ""
        numChars = len(message)
        shift = 1

        for i in range (0, numChars):
            char = message[i]
            num = ord(char) - 97
            if num >= 0 and num <= 26:
                num = (num + shift) % 26
            newMessage = newMessage + chr(num+97)

        print newMessage
    elif int(choice) == 2:
        break
    elif int(choice) == 3:
        break
