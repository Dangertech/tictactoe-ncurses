# tictactoe
A completely overengineered Tic Tac Toe Game for your Terminal

## What
You are currently looking at the Minimum Viable Product of a Tic Tac Toe game that I put way too much work in. 

While other Tic Tac Toe games are meant as a basic training in a programming language and only have some basic features, this one offers a little bit more. You **select the tiles** on the grid (I call them pixels) interactively, **with** either **the vi keys, the arrow keys or WASD and confirm your choice with Enter**. At the start of a new game, you can **customize** the size of your playing field and how many pixels in a row you need to win (Some more options are planned). You can currently only play against a (not overly intelligent, but hey, writing an opponent for a strategy game is pretty hard!) **computer opponent**, but real multiplayer is one of the main priorities. There is a **win detection**, but the game isn't overly excited to tell you that you won, you have to look at the bottom of the terminal to see a message appear. 

You see why I call this an MVP. There is some cleanup left to do, one or two easy to implement but great features missing and the UI could use some refinement too.

## Why!?
In the beginning, I was a little bored and thought "Hey, why not make a basic Tic Tac Toe game?". I didn't have any real C++ experience and didn't write standalone programs that were larger than like, 3 lines at the time though. And so I imagined that it would be a great benefit to get some hands-on experience with larger projects. So I wrote a version that only utilized the basic iostreams of C++ until I noticed, that I can't really do much with that. So I stumpled upon ncurses, ANSI escape codes, terminfo and all that good stuff the big boys use and started over with the ncurses library. Some days later, I thought about the usefulness of backups and finally started using my GitHub account that was rotting in the corner. And now, after way too much time, I stand here, holding a pretty sophisticated Tic Tac Toe game in my hands. The code is pretty shitty at some spots but remember, **this is my first time writing a larger scale program**, so please excuse my bad practices and unoptimized code. But we are still dealing with a terminal application and the bad code mostly involves some unnecessary while loops, so if you don't use hardware from like 1990 you should be fine.


Oh, and in case anyone ever finds  this interesting, you can of course open issues and pull requests. I'm of course planning to move on to other projects in the foreseeable future (It's a damn Tic Tac Toe game!), but I will still try to fix bugs and all that.
