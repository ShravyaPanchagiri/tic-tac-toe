 
# In-Kernel Tic-Tac-Toe Game Character Device Driver


This is a character device driver that runs a Tic Tac Toe game engine inside the Linux Kernel.I wrote this to demonstrate how to handle system calls, manage kernel memory, and use synchronization primitives like spinlocks to prevent race conditions.

# Technical Overview

The project is split into three main parts.

First, the kernel module in tictactoe.c handles the file operations.I used register_chardev to dynamically allocate a major number and implemented read and write funcions to communicate with user space.The write function parses coordinates using sscanf and validates the game state before allowing a move.

Second, the tictactoe.h header defines the game structure.I included a spinlock to protect the board data from concurrent access and a wait queue for process synchronization.This ensures that the game state remains consistent even if multiple processes try to access the device.

Third,test_user.c is a C application that provides a terminal interface for the game. It opens the device node, reads the current board state, and writes player moves back to the kernel.

# How to Build and Run

To compile the module and the test application, run make in the terminal.

To load the driver, use sudo insmod tictactoe.ko. You will need to check dmesg to see which major number was assigned to the device.Once you have the number,create the device node using mknod/dev/tictactoe c Major 0 and give it read/write permisiions.

After the device is set up, compile the user app with gcc test_user.c ttt and run it to play game.

# Environment

I developed and tested this code on an Ubuntu system.It is designed to be portable across different architectures since it uses standard Linux Kernel APIs.


Author

Shravya Panchagiri
Embedded Software Engineer
Austin, Texas 


