# ChessQt - App to play and host chess games online

Program with a server, database, and client functionality to play chess solo or with your friends. 

**Current state: Local only, in development**

As someone who grew up with chess, I thought building my own chess app along with key components such as servers, databases, and logic algorithms was a perfect next step project
to level up my scaling of my work. After learning from my other 2 game projects, I went into this with much more experience to structure my program well and to keep it organized.

## 📂 Project Info

- **Engine:** Qt Creator 16.0.2 - Desktop Application
- **Language:** C++: Qt Libraries  
- **Platform:** PC (Windows)  

---
###  *Will Be Updated (Project in Progress)*
---

## Client Overview
Players will only have access to the client version of the program.

- Chess ui navigation & connection to server
  - Connects to the server and is displayed connection status on main menu
  - Player can select the option to play singleplayer, host a game, or enter a code to join a game
- Players experience fully functioning chess matches:  
  - Players move turn-based as white goes first 
  - Full piece functionality with correct piece movesets and captures
  - Includes checks, pins, blocking checks, castling, en peasant, and pawn promotion. Players can only do legal moves.
  - Clean chess board UI to move pieces and check indicator.
- Client Log to show information:  
  - Displays successful game session creation on the server and if someone joins your game 
  - Displays error messages from the server to notify players
  - Players can send messages to each other. Acts as a chat box

<img width="600" height="400" alt="image" src="https://github.com/user-attachments/assets/690a27f5-4834-45be-9874-becf66d674e4" />

---

## TCP Server
- Wraps client sockets with client information and keeps track of all connections
- Manages all traffic: game creation, joining sessions, moves played, player messages
- Creates and tracks all game sessions/matches
  - creates unique game session ID (join code)
  - houses both players to send/recieve messages (system, error, and chat messages)
  - game validation/security as server verified every move by creating its own version of the chess match
- Access to PostgreSQL database to manage logins

<img width="450" height="440" alt="image" src="https://github.com/user-attachments/assets/aa26c73a-0fe7-4696-8502-006dc15dfb28" />


---

## **Process Implemented**
1. Creating heavy algorithms effeciently (piece moveset, pin, and check calculations)
  - created generalized functions to handle multiple heavy logic algorithms (ex. king check and piece moveset algorithms use the same function)
  - minimal hardcoding
2. TCP Server - Client Connections
  - server and client have their own unique networking class that is purely to send and recieve binary messages through their socket
  - server has security measures as you cannot trust the client (move validation, join code precision)
  - both server and client have logs to debug/display statuses of connections and messages
3. Keeping clean program environment
  - strict seperation of ui and backend logic; priority on readibility/naming, commenting, and visual breaks; and broad functions - mindful of future expansions
  - clean imports, includes, and class hierarchy
  - delegation of tasks to classes (one class does 1 specific job)

## Class Hierarchy
```
|
|                            |  Server Side  |                                                |  Client Side  |
|                                                           <-Binary->                                
|                  |server.cpp| <——> |clientWrap.cpp| <——-——-——-——-——-——-——> |client.cpp| <——> |mainwindow.cpp|  <———— |main.cpp|
|                   |        |                                                                        |                
|                   |        |                                                                        |                         
| |server_security.cpp|    |gameSession.cpp|                                                   |chess_game.cpp| <——————> |board_state.cpp|       
|   |dataBase_chess|               |                                                                  |                         |    
|                                  |                                                                  |                         |        
|                          |board_state.cpp|                                               |clickable_piece_label|       |chess_pieces.cpp|
|                                                                                          |clickable_tile_label|
|                                                                                                                                                                                                                                                                                        
```                                                                                                                  

Note: This I would consider is my first major project as the final goal is deployment for others to download and play chess against each other. My intention is to learn the full
process of building an application: backend algorithm logic, UI connection to backend, server network to receive and send messages to clients, and a database to house client 
information. I plan to make the server remote with security measures and to add more features to aim for full functionality and professionalism in an app.
