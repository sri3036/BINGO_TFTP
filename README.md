# BINGO_TFTP

This is my another project based on socket programming in C language. This code is for playing a BINGO game with friends.

Code is separated into two parts :
    1. SERVER ( as player 1 )
    2. CLIENT ( as player 2 )

Steps to play the game:
    1. First you should connect the two laptops with the same Wi-Fi.
    2. Compile tha server code first.
    3. Then compile the client code.

  SERVER:

        gcc main_server.c source_server.c -o server

        ./server

  CLIENT: You need to add ip of connected Wi-Fi ( you can find in command prompt )

        gcc main_client.c source_clinet.c -o client

        ./client ip
  
  Example : ./client  192.167.182.19


While compiling, if you found the error : " bad address received " (or) " Server is already binded " , Just close the ubuntu and do compile again.


That'S it !!.....

Just enjoy the game with your friends and have fun.....

