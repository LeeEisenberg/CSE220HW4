# CSE 220: Systems Level Programming Assignment 4

## Overview
Chess program written in C that allows one player to start a server on their computer that another player can connect to as a client, and then play a game with the other player.
This program does not check for complex chess rules such as en passant, check, castling, and checkmate, but still otherwise checks move legality.
Chess states are sent from the client to the server and back in FEN notation, and are displayed in the terminal on either side, with black pieces in lowercase and white pieces in capitals, all represented by the letters for pieces and underscores for empty squares.
Test cases provided by professor Kevin McDonnell.
