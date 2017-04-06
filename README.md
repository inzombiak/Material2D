# Material2D
2D version of Material used to test mechanics

The UI was never cleaned up, but the buttons in the lower area change the element that is placed after the PC as it walks around, blue being ice and
red being fire. There may be some bugs I haven't ironed out yet

While working on a 3D surface material changing prototype, I realized it would take longer to test basic mechanics in 3D than in 2D
and since I already had a large part of the code for this from BeatHeroes, I decided to test it out in 2D.

Most of the logic for what I explain below can be found is implemented in Lua, the scripts can be found in the "Scripts" folder.

The devil looking enemy (Follower.lua) charges at the player if he/she is within range, but if it steps on a red tile it "catches fire"
and continues to run in the same direction. The barrels are explosive (Explosive.lua).
In the next area when the switch (Trigger.lua) is stepped on the ball (RollingStone.lua) rolls and hits a wall (Breakable.lua), but it moves too slow to break it.

All artwork is from OpenGameArt.org
 * Tiles - http://opengameart.org/content/rpg-tiles-cobble-stone-paths-town-objects
 * Enemies - http://opengameart.org/content/dungeon-crawl-32x32-tiles
 * Hero - cant find it
 
Animations were create using [darkFunction Editor](http://darkfunction.com/editor/)

## Build Requirements:
SFML 2.4.2 for rendering
