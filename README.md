ConnectX
=====================
**ConnectX** is an interactive cross-platform C++ application based on the classical Connect Four game. 
It offers more variants in terms of the number of pieces to connect and the size of the board.

![Demo ConnectX](./ConnectXDemo.gif)

There is no GUI; this is a text-based application.

Computer strategy uses a version of the Minimax with alpha-beta pruning.

It was a **pet project** to try out a more generic design of the game.
It should be reasonably simple to other variants with more players or dimensions.

Variants:
---------
- 6-by-7 Connect Four
- 7-by-10 Connect Four
- 8-by-8 Connect Four    
- 3-by-6 Connect Three          
- Five-in-a-Row          

Features and Commands:
----------------------
It is possible to play against the computer or another human.

- Undo `u`
- Restart program `r`
- Change difficulty level `d`
- Quit game  `q`


Author
------
Franck Nassé (original version coded in February 2016)


Build Requirements
------------
- C++ compiler which supports C++11 at the minimum.
- [Ncurses library](https://www.gnu.org/software/ncurses/) 
Example
> Installation for Centos

```sudo yum install ncurses-devel```


- Windows (PDCurses 3.8)


Build
------
On Linux:

At the project root directory
- `make clean`
- `make release`

The program is generated in the `./binary/bin/` folder

Start the program by typing `./binary/bin/ConnectX`

On Windows with MinGW:

- Install PDCurses at `C:\PDCurses-3.8`
- [CodeLite](https://codelite.org/) IDE can be used; the workspace is located in the `ConnectXProject` folder.
- Modify the project according to the location of `pdcurses.dll` or `pdcurses.a` on your system.

Remarks
-------
Adjust the size of the terminal (Fullscreen).

Sometimes flickering can occur.