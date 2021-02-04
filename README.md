# cli-tetris
A fun command line implementation of tetris.

The code for this was initially written in Turbo C++ for my high school capstone project and sadly, using Turbo C++ was a requirement. The program in itself is pretty decent and took hours of effort, so I will be attempting to port it from its DOS roots to a modern-er command line game. 



Here's a few images of the game running on DOS ;)

![img](https://lh4.googleusercontent.com/KZNT9YheNipzlOm0laqkZqrZmMBqX-_KJWY6xxsyWeeS5VnvrzS_mzqYI987xmB-Huci1OqpjuxOkQx7K5sTuZuLa-e4Kpo9VNIVs_xfbNQdmyDBIbH1T1oBmDkqqlBbQ967NQmS)

![img](https://lh5.googleusercontent.com/C_J9vsxcReY67-8wdUVao1C1V3eajxyGGJdyI6myf5z33u1x8W0JTMJ0iySi5ZHkg3ajW2Yiyx43pEAZs9ZBmHghYl1UCboeEV-Y4UdMbwjDOwoFiUmG7zJhxa99QWnDhuSJgD8a)

![img](https://lh5.googleusercontent.com/0gVufDmVGP09K1ih5dztbe36-jaOAlUjJE1wqQZoO3NFn0If6rT3GU67uOGRLcqEHwYDx53ilYynIQqN_pzFxV9v6ONRUTEl1iFfmwdmOpkxJBsvOQhYRxIU7ZsLWeqciI0b8hAV)

(Yes it's possible to mess up this bad)

![img](https://lh5.googleusercontent.com/t5BAyfLCIZ95GD82Ucn0oNzXncH8SWPOoIlyfPkgXQqWY41dNMdqw4-6TkSzCkD0S7v4uhA9PrMBVrORyq5lzSxvAL6cQJVKJBhD4gu5Q5OPqllGMfT4bvY4Hd6bBsKaiTGI_wL2)

The blocks are longer along the vertical because I use the ascii block character to display to terminal, hence giving a slightly skewed image. Something I am hoping to fix, soon.

Here's a list of improvements I plan on making,

- The major change is to remove any and all dependencies on conio.h to something like ncurses
- Fix some bugs related to taking long inputs
- Make the timings feel smoother and attempt to make the blocks look more 'square-er'
- Split the program into multiple files

Feel free to share any suggestions / ideas you may have!