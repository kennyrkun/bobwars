# bobwars
bobwars ia real-time strategy game focusing on the intense struggle between Bob and Google+ which took place in late 2013.

## Setting up
Clone the project to the desired location on your machine.
Make sure you have [SFML 2.5.1](http://www.sfml-dev.org/download/sfml/2.5.1/) in the project's root directory.

You'll also need [SFUI2](https://github.com/kennyrkun/SFUI/) (latest) and [DiscordRPC](https://github.com/discordapp/discord-rpc) (Windows only).

### Linux
If all prerequisites are satisfied, you should be able to build the project very easily:
```
git clone https://github.com/kennyrkun/bobwars/
cd bobwars
mkdir build && cd build
ln -s ../bobwars
cmake .. && make
```
### Windows
I don't know.

## Playing
While in the game, the WASD are used to control the main view. Units can be created with the create button, and delete with the delete button or the DEL key. To select a unit, simply left click the desired unit. To deselect that unit, either click another unit or click something that is not a unit. 

#### Controls

##### General
<kbd>Escape</kbd> - Pause game<br>
<kbd>F12</kbd> - Screenshot<br>
<kbd>MouseWheel</kbd> - Change player camera zoom level by 2.0f (+/-)<br>
<kbd>Left Shift</kbd> - Speed up player camera by 250u/s<br>
<kbd>Control</kbd> + <kbd>+</kbd> - Zoom in<br>
<kbd>Control</kbd> + <kbd>-</kbd> - Zoom out<br>

##### Unit Controls
<kbd>LMB</kbd> - Selects unit<br>
<kbd>Ctrl</kbd> + <kbd>LMB</kbd> - Adds unit to group of already selected units.<br>
<kbd>RMB</kbd> (Unit) - Set unit movement destination.<br>
<kbd>RMB</kbd> (Building) - Set unit rally point.<br>
<kbd>Del</kbd> - Deletes selected unit(s).<br>

##### Debug
<kbd>~</kbd> or <kbd>`</kbd> - Toggle debug mode<br>

## Built With
* [Simple and Fast Media Library (SFML)](http://www.sfml-dev.org) - The Graphics library used.
* some bits of the [Create Me Engine](https://github.com/roglew/CreateMeEngine)
* anyone who had their code copied from StackOverflow
