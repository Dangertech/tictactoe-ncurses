# tictactoe
A completely overengineered Tic Tac Toe Game for your Terminal

## What
You are currently looking at the **Minimum Viable Product** of a Tic Tac Toe game that I put way too much work in. 

While other Tic Tac Toe games are meant as a basic training in a programming language and only have some basic features, this one offers a little bit more. You **select the tiles** on the grid (I call them pixels) interactively, **with** either **the vi keys, the arrow keys or WASD and confirm your choice with Enter**. At the start of a new game, you can **customize** the size of your playing field and how many pixels in a row you need to win (Some more options are planned). You can currently only play against a (not overly intelligent, but hey, writing an opponent for a strategy game is pretty hard!) **computer opponent**, but real multiplayer is one of the main priorities. There is a **win detection**, but the game isn't overly excited to tell you that you won, you have to look at the bottom of the terminal to see a message appear. 

You see why I call this an MVP. There is some cleanup left to do, one or two easy to implement but great features missing and the UI could use some refinement too.

## Why!?
In the beginning, I was a little bored and thought "Hey, why not make a basic Tic Tac Toe game?". I didn't have any real C++ experience and didn't write standalone programs that were larger than like, 3 lines at the time though. And so I imagined that it would be a great benefit to get some hands-on experience with larger projects. So I wrote a version that only utilized the basic iostreams of C++ until I noticed, that I can't really do much with that. So I stumpled upon ncurses, ANSI escape codes, terminfo and all that good stuff the big boys use and started over with the ncurses library. Some days later, I thought about the usefulness of backups and finally started using my GitHub account that was accumulating dust in the corner. So notice, I'm not good at using Git or Github yet and might do some dumb stuff. And now, after way too much time, I stand here, holding a pretty sophisticated Tic Tac Toe game in my hands. The code is pretty shitty at some spots but remember, **this is my first time writing a larger scale program**, so please excuse my bad practices and unoptimized code. But we are still dealing with a terminal application and the bad code mostly involves some unnecessary while loops, so if you don't use hardware from like 1990 you should be fine.

## Compile
Right now, you can only compile the program yourself, there are no releases. 

You need: 
- gcc
- ncurses-devel

After confirming the installation of these packages, clone the repo to a new folder and run this command:

``g++ -o tictactoe vars.cpp main.cpp processing.cpp rendering.cpp -I $PWD -lncurses``

You can then run the game by typing ``./tictactoe`` or executing the file with your favorite file explorer

(It's probably bad practice to not use a Makefile but this works too and there aren't that many files, so...)

--------------------------

Oh, and in case anyone ever finds  this interesting, **you can of course open issues and pull requests**. I'm of course planning to move on to other projects in the foreseeable future (It's a damn Tic Tac Toe game!), but I will still try to fix bugs and all that.

If you are one of these people that need to see images of everything to understand it, here:

![UI](https://user-images.githubusercontent.com/65275192/132101673-e4d2b476-1e0b-487e-a02c-e5fdb18fe1fc.png)
*Game UI (Beautiful AwesomeWM rice not included)*

![Max_Size](https://user-images.githubusercontent.com/65275192/132101793-e0cddef7-32f0-4cb3-98ec-972f50e7d7c0.png)
*The current maximum grid size*

![New_Game](https://user-images.githubusercontent.com/65275192/132101858-d7a92452-97e0-4d35-b72b-f3b77eb33a7c.png)
*Start screen when starting a new game*
