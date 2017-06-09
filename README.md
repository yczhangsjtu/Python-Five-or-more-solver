# Python-Five-or-more-solver
A simple python solver for linux gnome game five-or-more.
Including bash scripts helping to get the game state from screen.

The core solver algorithm is written in C++, and other operations are handled
by python and bash scripts.

The screen helper in bash that gets game state from screen usable is heavily
relied on the specifics of the five-or-more game.
This version specifically targets the ubuntu gnome version, with the
appearance setting of style being `balls`.

To make it usable on your system, you probably have to modify the `getpix.sh`
file. Set `prd` to be the grid size, `offx` and `offy` to be the offset from
the up-left corner of the grid where the pixel color is standard.
If the color scheme of your version of game is different, you probably have
to modify the subsequent color values.

This solver is stateless, i.e. it does not store the history, just analyzes the
current board and tells you how to move.

In each step, execute in the commandline `./run`.
The script will do the following things:

1. Take a screenshot, save the picture in `/tmp` directory.
2. Search for the background color (if yours is different, change it in the script),
   locate the up-left corner `(x0,y0)` of the entire board.
3. For the 8x8 grid of pixels `(x0+i*prd+offx,y0+j*prd+offy)`
   for `i=0,...,7` and `j=0,...,7`, obtain the colors and figure out the ball color.
	 After this step, the board state is determined.
4. Pass the board state to the core solver, then print the solution.
5. The solution is printed in the following format: an empty grid is a dot (`.`),
   a ball is represented by a letter corresponding to its color.
	 If the solution moves one ball to another position, the original position is
	 a dot (`.`) with blue background color, and the new position is the ball color
	 letter with red foreground color.

If you are not using bash, the print result may be a lot of garbage.
You'd better modify the output format according to the color scheme of the shell
you use.
