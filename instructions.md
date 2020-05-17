# Linux Installation Instructions

## get g++ compiler
`apt install g++`

## get make
`apt install make`

## get sdl2
### develop version of this, access to functions
`sudo apt-get install libsdl2-dev`

### adds jpeg and png support
`sudo apt-get install libsdl2-image-dev`

### adds True Type Font (TTF) support
`sudo apt-get install libsdl2-ttf-dev`

### adds sound support (WAV/MP3)
`sudo apt-get install libsdl2-mixer-dev`

## Get Lua
`sudo apt-get install lua5.3`

## Get Lua Dev
`sudo apt-get install liblua5.3-dev`

## Get X Forwarding working with WSL
### Install X11 forwarding
`sudo apt-get install x11-apps`

### Install xMing on Windows 10
https://sourceforge.net/projects/xming/

### Export Display Env Variable
Set this in ~/.bash_rc file
`export DISPLAY=:0`

verify using 
`xclock`

## May need PNG supported on OS
`sudo apt-get install libpng-dev`
(didn't need this, SDL has it's own libpng)

## debugging and profiling with valigrind
`apt install valgrind`
`valgrind -v ./game` 