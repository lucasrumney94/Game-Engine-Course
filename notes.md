# General Setup
Could use Linux, Mac, Windows. Unix preferred. I will be using Windows Linux Subsystem/Ubuntu. If I use WSL I can use VSCode as an editor.

Using a make file that calls g++ compiler, and includes all of the lua and SDL libs.

## GNU Make
Important tool for building. There are Flags, rules, many many more features other than just building. 

# Game Engine Notes

## Game Loop
( Process Input -> Update Game -> Render )

Repeat Every Frame.

## Timesteps
Fixing our timestep is done through a deltaTime.
A frame deltaTime will allow us to stabilize our game.
Instead of just moving things every frame, tied to the framerate, 
we'd like the distance to be framerate independent. 

deltaTime is the amount of real time elapsed since the last frame.
`position.x += movement*deltaTime;`

## Rendering

### Double Buffer
- Back Buffer: Load this with information then wap
- Front Buffer: What's being shown

- Color Buffer: Colors of the screen pixels

### Process
1. Clear the back buffer
2. Draw all game objects on the back buffer
3. Swap the front and back buffers
4. Repeat