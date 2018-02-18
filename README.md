# bobwars
bobwars is a game like age of empires (2), except instead of being all medieval and stuff it's just the bob copy pasta and google plus

## Setting up
Clone the project to the desired location on your machine.
Make sure you have [SFML 2.4](http://www.sfml-dev.org/download/sfml/2.4.2/) in the project's root directory.

If you want to build the game without dynamic runtime library linking then you'll have to [build SFML yourself](http://en.sfml-dev.org/forums/index.php?topic=8158.msg54553#msg54553).

---

## Playing
While in the game, the WASD are used to control the main view, and the arrow keys are used to move selected units. Units can be created with the create button, and delete with the delete button or the DEL key. To select a unit, simply left click the desired unit. To deselect that unit, either click another unit or click something that is not a unit. 

#### Controls

##### General
<kbd>Escape</kbd> - Pause game<br>
<kbd>F12</kbd> - Screenshot<br>
<kbd>MouseWheel</kbd> - Change player camera zoom level by 2.0f (+/-)<br>
<kbd>Left Shift</kbd> - Speed up player camera by 250u/s<br>
<kbd>Control</kbd> + <kbd>+</kbd> - Zoom in<br>
<kbd>Control</kbd> + <kbd>-</kbd> - Zoom out<br>

##### Units
<kbd>LMB</kbd> - Selects unit<br>
<kbd>Ctrl</kbd> + <kbd>LMB</kbd> - Adds unit to group of already selected units.<br>
<kbd>RMB</kbd> (Unit) - Set unit movement destination.<br>
<kbd>RMB</kbd> (Building) - Set unit rally point.<br>
<kbd>Del</kbd> or <kbd>End</kbd> - Deletes selected unit(s).<br>

##### Debug
<kbd>~</kbd> or <kbd>`</kbd> - Toggle debug mode<br>

---

## Built With
* [Simple and Fast Media Library (SFML)](http://www.sfml-dev.org) - The Graphics library used.
* some bits of the [Create Me Engine](https://github.com/roglew/CreateMeEngine)
* anyone who had their code copied from StackOverflow
