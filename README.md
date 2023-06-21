# A space invaders clone made in C++ with a basic graphics library and OOP concepts.

### How the game works:

- Level:
  - A 2D array of integers read from a .txt file.
  - Boundary (wall) represented by 1s.
  - Empty space represented by 0s.

- Bullets:
  - Are represented by the integer 3.
  - Is shot by the player, and travels vertically.
  - The game maintains a vector list of shot bullets.
  - Bullets that hit the top border are removed from the list.
  - A bullet destroys the first enemy it collides with in it's path.

- Enemies:
  - Are represented by the integer 4.
  - Enemies travel vertically downwards after a certain time.

- Player:
  - Is represented by the integer 2.
  - There is only a single instance of the player.
  - Player can mover horizontally and vertically to a certain extent.
  - The player loses if any enemy manages to reach the bottom of the level.
  - Destroying all enemies results in the player winning.

![image](https://github.com/ziyadtalha/Space-Invaders/assets/105555466/b00cd190-7066-45b0-b439-08e0f3533c77)

![image](https://github.com/ziyadtalha/Space-Invaders/assets/105555466/7d8d06b9-2388-4373-b78c-ee6ca7e61230)
