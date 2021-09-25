# tictactoe-ncurses
A completely overengineered Tic Tac Toe Game for your Terminal

## What
You are currently looking at the **Version 1** release of a Tic Tac Toe game that I put way too much work in. 

While other Tic Tac Toe games are meant as a basic training in a programming language and only have some basic features, this one offers a little bit more.
What differentiates the game the most from many others is, that it **runs in the terminal using ncurses.** Expect a fully fledged keyboard-driven user experience.
You **select the tiles** on the grid (I call them pixels) interactively, **with** either **the vi keys, the arrow keys or WASD and confirm your choice with Space or Enter**.
At the start of a new game, you can **customize** the **size of your playing field**, the **appearance** of any player and **how many pixels in a row** you need to win (Some more options are planned).
You can play against a (pretty dumb) **computer opponent** or another **real human**.
The game can **detect wins** on any grid size. After someone won or the game finished with a **tie**, you can choose to **restart** the game with the current settings or **quit**.

Now, this game has met it's full release. I don't intend on developing it further, and it has more than everything you could expect of a tic-tac-toe game, especially if played with someone else.

## Why!?
In the beginning, I was a little bored and thought "Hey, why not make a basic Tic Tac Toe game?". I didn't have any real C++ experience and didn't write standalone programs that were larger than like, 3 lines at the time though. And so I imagined that it would be a great benefit to get some hands-on experience with larger projects. So I wrote a version that only utilized the basic iostreams of C++ until I noticed, that I can't really do much with that. So I stumbled upon ncurses, ANSI escape codes, terminfo and all that good stuff the big boys use and started over with the ncurses library. Some days later, I thought about the usefulness of backups and finally started using my GitHub account that was accumulating dust in the corner. So notice, I'm not good at using Git or Github yet and might do some dumb stuff. And now, after way too much time, I stand here, holding a pretty sophisticated Tic Tac Toe game in my hands. The code is pretty shitty at some spots but remember, **this is my first time writing a larger scale program**, so please excuse my bad practices and unoptimized code. But we are still dealing with a terminal application and the bad code mostly involves some unnecessary while loops, so if you don't use hardware from like 1990 you should be fine.

## Compile
The game has a v1.0 release, but only for **GNU/Linux**. You can compile it yourself for any other operating system without major modification (or so I hope).

You need: 
- gcc
- ncurses-devel

After confirming the installation of these packages, clone the repo to a new folder and run this command:

``g++ -o tictactoe vars.cpp main.cpp processing.cpp rendering.cpp -I $PWD -lncurses``

You can then run the game by typing ``./tictactoe`` or executing the file with your favorite file explorer

(It's probably bad practice to not use a Makefile but this works too and there aren't that many files, so...)

## License
The game is licensed under the [UNLICENSE](https://unlicense.org/), which means that you can do basically whatever the fuck you want. I really like that mindset.

--------------------------

Oh, and in case anyone ever finds  this interesting, **you can of course open issues and pull requests**. I'm of course moving on to other, more interesting projects (It's a damn Tic Tac Toe game!), but I will still try to fix bugs and all that.

If you are one of these people that need to see images of everything to understand it, here:

![image](https://user-images.githubusercontent.com/65275192/134777192-afc6fddd-55e2-4549-848f-c703f4981954.png)
*Game UI (Beautiful AwesomeWM rice not included)*

![image](https://user-images.githubusercontent.com/65275192/134777110-757bcad9-33e2-4bb5-8cec-d3fd53cda262.png)
*A multiplayer game with a giant grid size (and high pixels needed to win in a row)*-

![image](https://user-images.githubusercontent.com/65275192/134777162-63bab51e-4a00-4d17-bfd5-a41966a14e1c.png)
*Start screen when starting a new game*

![image](https://user-images.githubusercontent.com/65275192/134777233-03f4aee4-98c7-4bc8-ba36-3d3af89d65af.png)
*Screen when winning a game*
