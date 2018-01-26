# CAstroParty

A **Multiplayer Astro Party** written in C

Brought to you by **Khyber Sen**, **Adris Jautakas**, and **Jawad Kadir**

## What Is CAstroParty?

**CAstro Party** is a multiplayer version of the popular iPhone game Astro Party, written in C using SDL2.
Unlike in Astro Party, where everyone has to crowd around a single tiny phone, 
**CAstro Party** can be played on the luxury of a big computer with other plays on the other side of the world.
It uses low-level networking in C to access the Internet and deliver the optimal gaming experience, 
even when facing opponents thousands of miles away.

In **CAstro Party**, you control a spaceship that never stops accelerating.
You only have two controls: turning right (right click) and shooting (left click).
You have to compete against other spaceships trying to gun you down.
You have to evade all their bullets and kill them yourself.
Good luck!  Instructions for building and running the game are below.

## Instructions

### Starting the game

1) Build CAstroParty with `make`. 
    1) It might be a good idea to run `make clean` first.

2) Start a server with `./game.out -s`. 
    1) It runs on port 8000.

3) Start client(s) with `./game.out`. 
    1) Connect to a specific IP address with `./game.out [IP]`.
    2) If the server and client are running on the same computer, the IP address is not needed. 
       It will automatically use localhost.
    3) If the server is on a different computer, you need to supply the IP address.
       This can be found by running `ifconfig` on *nix systems and `ipconfig` on Windows.
       (However, the SDL Library often cannot find a display device when running on Windows).
    4) The server requires a certain number of clients to connect to start the game,
       so they must all connect before the game will start.

### Playing the game

#### Controls

| Control      | Action       |
|:------------:|:------------:|
| Right Click  | Turn Right   |
| Left Click   | Shoot        |
| Middle Click | Restart Game |
