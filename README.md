<h1 align="center">
  Dx Ball
  <br>
</h1>

<h4 align="center">A minimal imitation of the classic DX Ball Game written in <code>C++</code> & <code>OpenGL</code>.</h4>

<p align="center">
  <img src="https://img.shields.io/badge/C++-Language-informational?style=flat&logo=c%2B%2B&logoColor=blue&color=blueviolet">
  <img src="https://img.shields.io/badge/OpenGL-API-informational?style=flat&logo=opengl&logoColor=blue&color=blueviolet">
  <img src="https://img.shields.io/badge/Windows-OS-informational?style=flat&logo=windows&logoColor=green&color=orange">
  <img src="https://img.shields.io/badge/Code::Blocks-Editor-informational?style=flat&logo=codio&logoColor=yellow&color=red">
</p>

<p align="center">
  <a href="#Installation">Installation</a> •
  <a href="#Controls">Controls</a>
  <a href="#Key-Game-Features">Key Game Features</a>
</p>

## Installation

### For Players
* Go to <code>bin/release</code> and run <code>Dx Ball 2D.exe</code>

### For Developers
Ignore the first two steps if you have already setup OpenGL with Codeblocks.
* Follow step 1 and 2 from [here](https://www.codewithc.com/how-to-setup-opengl-glut-in-codeblocks/)
* Additionally, copy the <code>glut.dll</code> file and paste to the folder <code>"C:\Windows\SysWOW64"</code>
* Clone the repo on your machine
* Open <code> DX Ball 2D.cbp </code> using <code> Code::Blocks </code>
* From this point, modify whatever you want and hit <code>Build and run</code> at any moment after selecting the target (Debug/Release) to compile and run the game

## Controls

### Menu Navigation
* <code>`Up Arrow`</code> <code>`Down Arrow`</code> <code>`Enter`</code> <code>`Esc`</code>

### GamePlay
* <code>`Left Arrow`</code> <code>`Right Arrow`</code> <code>`a`</code> <code>`d`</code> for moving the paddle
* <code>`Spacebar`</code> for launching the ball

## Key Game Features

### New Game, Pause & Resume
* Start a new game from the menu. 
* Press <code>`Esc`</code> to pause the game and go back to the menu. 
* Resume playing again using the menu.

### Lives & Levels
* Five lives given at the start of a game. 
* To finish the game you need to complete three levels.

### Saving & Loading
* Four slots to save & load any game at any state.
* Overwritting is allowed. 
* Can be accessed from the menu.

### FPS & Sound
* Toggle the sound ON or OFF from the main menu.
* Four FPS options given to the player: 
  - 60 fps
  - 110 fps
  - 200 fps
  - 330 fps
* To verify FPS:
  - Set an FPS from the menu
  - Start a new game or resume playing an existing game
  - Pres <code>`Win`</code> + <code>`G`</code> to bring up Xbox Game Bar (It shows the actual FPS)
  - Check the actual FPS with the FPS you have just set
