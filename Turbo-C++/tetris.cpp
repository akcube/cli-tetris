#include <fstream.h>
#include <conio.h>
#include <dos.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <graphics.h>


#define hscorelimit 11


#define DEG_0 0
#define DEG_90 1
#define DEG_180 2
#define DEG_270 3


#define BGBLOCK 0
#define IBLOCK 1
#define TBLOCK 2
#define OBLOCK 3
#define ZBLOCK 4
#define SBLOCK 5
#define LBLOCK 6
#define JBLOCK 7
#define WALLBLOCK 9


#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define SPACE 32


#define true 1
#define false 0
#define bool int


#define BLOCK_MOVES_RIGHT 1
#define BLOCK_MOVES_LEFT 2
#define BLOCK_MOVES_DOWN 3
#define BLOCK_ROTATES_ONE 4


#define BLACK 0
#define BLUE 1
#define GREEN 2
#define RED 4
#define ORANGE 6
#define GREY 8
#define CYAN 11
#define PINK 13
#define YELLOW 14


#define STARTGAME 0
#define INSTRUCTIONS 1
#define HIGHSCORES 2
#define CONFIGGAME 3
#define ENDGAME 4


int score;
bool isGameOn, bringDown;


void gameLoop();


void init()
{
   isGameOn = true;
   score = 0;
   randomize();
   bringDown = false;
}


void initg()
{
   int gd = DETECT, gm;
   initgraph(&gd, &gm, "C://TURBOC3//BGI");
}


class Settings;


class xColors
{
private:
   int data[10];
   int allcolors[9];
   int findColorIndex(int color);
   void resetColors();


public:
   //Getters==================================
   void setData(int x[10])
   {
      data[0] = x[0];
      data[1] = x[1];
      data[2] = x[2];
      data[3] = x[3];
      data[4] = x[4];
      data[5] = x[5];
      data[6] = x[6];
      data[7] = x[7];
      data[8] = x[8];
      data[9] = x[9];
      saveNewData();
   }
   void saveNewData();
   int getColorX(int block)
   {
      return (block >= 0 && block <= 9) ? data[block] : -1;
   }
   void setColor(int block, int color);
   void cName(int color, int defColor);
   int getNextColor(int color);
   int getPrevColor(int color);
   xColors();
   friend class Settings;
};


void xColors::resetColors()
{
   int x[] = {CYAN, RED, PINK, GREEN, BLUE, YELLOW, WHITE, BLACK, -1, ORANGE};
   setData(x);
   saveNewData();
}


int xColors::findColorIndex(int color)
{
   for (int i = 0; i <= 8; i++)
   {
      if (allcolors[i] == color)
      {
         return i;
      }
   }
   return -1;
}


int xColors::getNextColor(int color)
{
   int i = findColorIndex(color);
   return (allcolors[(i < 8) ? i + 1 : 0]);
}


int xColors::getPrevColor(int color)
{
   int i = findColorIndex(color);
   return (allcolors[(i > 0) ? i - 1 : 8]);
}


void xColors::setColor(int block, int color)
{
   data[block] = color;
   saveNewData();
}


void xColors::cName(int color, int defColor = CYAN)
{
   textcolor(color);
   switch (color)
   {
   case RED:
      cprintf("RED");
      break;
   case CYAN:
      cprintf("CYAN");
      break;
   case YELLOW:
      cprintf("YELLOW");
      break;
   case WHITE:
      cprintf("WHITE");
      break;
   case ORANGE:
      cprintf("ORANGE");
      break;
   case PINK:
      cprintf("PINK");
      break;
   case BLUE:
      cprintf("BLUE");
      break;
   case GREEN:
      cprintf("GREEN");
      break;
   case BLACK:
      cprintf("BLACK");
      break;
   }
   textcolor(defColor);
}


void xColors::saveNewData()
{
   fstream fin("COLORS.DAT", ios::out | ios::binary);
   fin.write((char *)&data, sizeof(int[10]));
   fin.close();
}


xColors::xColors()
{
   allcolors[0] = CYAN;
   allcolors[1] = RED;
   allcolors[2] = PINK;
   allcolors[3] = GREEN;
   allcolors[4] = BLUE;
   allcolors[5] = YELLOW;
   allcolors[6] = WHITE;
   allcolors[7] = BLACK;
   allcolors[8] = ORANGE;
   fstream fin("COLORS.DAT", ios::in | ios::binary);
   if (fin)
   {
      int temp[9];
      fin.read((char *)&temp, sizeof(int[10]));
      this->setData(temp);
   }
   else
   {
      int x[] = {CYAN, RED, PINK, GREEN, BLUE, YELLOW, WHITE, BLACK, -1, ORANGE};
      setData(x);
      saveNewData();
   }
   fin.close();
}


xColors c;


class Block
{
private:
   char blocks[7][16];
   int r;
   int piece;
   int XPOS;
   int YPOS;
   int speed;
   int tickcount;


public:
   //Getters====================================
   int getRotation() { return r; }
   int getCurrentPiece() { return piece; }
   int getX() { return XPOS; }
   int getY() { return YPOS; }
   char *getBlock() { return blocks[piece]; }
   int getSpeed() { return speed; }
   int getTicks() { return tickcount; }
   //Setters====================================
   void setRotation(int x)
   {
      r = x % 4;
   }
   void setPiece(int p)
   {
      piece = p;
   }
   void setX(int x)
   {
      XPOS = x;
   }
   void setY(int y)
   {
      YPOS = y;
   }
   void setSpeed(int s)
   {
      speed = s;
   }
   void setTicks(int t)
   {
      tickcount = t;
   }
   //Constructor================================
   Block()
   {
      r = 0;
      piece = 0;
      XPOS = 5;
      YPOS = 0;
      speed = 20;
      tickcount = 0;
      //assign our blocks their pos vals=========
      strcpy(blocks[0], "--X---X---X---X-");
      //--X-
      //--X-
      //--X-
      //--X-
      strcpy(blocks[1], "--X--XX---X-----");
      //--X-
      //-XX-
      //--X-
      //----
      strcpy(blocks[2], "-----XX--XX-----");
      //----
      //-XX-
      //-XX-
      //----
      strcpy(blocks[3], "--X--XX--X------");
      //--X-
      //-XX-
      //-X--
      //----
      strcpy(blocks[4], "-X---XX---X-----");
      //-X--
      //-XX-
      //--X-
      //----
      strcpy(blocks[5], "-X---X---XX-----");
      //-X--
      //-X--
      //-XX-
      //----
      strcpy(blocks[6], "--X---X--XX-----");
      //--X-
      //--X-
      //-XX-
      //----
   }
   //Class Functions=========================
   int rotatedPos(int px, int py);
   bool shouldBringPieceDown();
};


bool Block::shouldBringPieceDown()
{
   if (tickcount == speed)
      return true;
   else
      return false;
}


int Block::rotatedPos(int px, int py)
{
   int i = 0;
   switch (r % 4)
   {
   case DEG_0:
      i = py * 4 + px;
      break;


   case DEG_90:
      i = 12 + py - (px * 4);
      break;


   case DEG_180:
      i = 15 - (py * 4) - px;
      break;


   case DEG_270:
      i = 3 - py + (px * 4);
      break;
   }
   return i;
}


class Playbox
{
   //private variables=======================
private:
   int pos_x;
   int pos_y;
   int playbox_width;
   int playbox_height;
   void init();


public:
   //Our playbox=============================
   char *playbox;
   //constructors============================
   Playbox()
   {
      pos_x = 0;
      pos_y = 0;
      playbox_width = 12;
      playbox_height = 21;
      init();
   }
   Playbox(int x, int y, int width = 12, int height = 21)
   {
      pos_x = x;
      pos_y = y;
      playbox_width = width;
      playbox_height = height;
      init();
   }
   //getters=================================
   int getHeight() { return playbox_height; }
   int getWidth() { return playbox_width; }
   int getPosX() { return pos_x; }
   int getPosY() { return pos_y; }
   //setters=================================
   void setHeight(int height)
   {
      playbox_height = height;
   }
   void setWidth(int width)
   {
      playbox_width = width;
   }
   void setPosX(int x)
   {
      pos_x = x;
   }
   void setPosY(int y)
   {
      pos_y = y;
   }
   //class functions=========================
   bool doesBlockFit(Block block, int movement = 0);
   void removeLine(int line);
};


void Playbox::removeLine(int line)
{
   char *temp = NULL;
   temp = new char[line * playbox_width];
   //save all rows above 'line' to temp
   for (int y = 0; y < line; y++)
   {
      for (int x = 0; x < playbox_width; x++)
      {
         temp[y * playbox_width + x] = playbox[y * playbox_width + x];
      }
   }
   //push all rows down by one
   for (int py = 0; py < line; py++)
   {
      for (int px = 0; px < playbox_width; px++)
      {
         playbox[(py + 1) * playbox_width + px] = temp[py * playbox_width + px];
      }
   }
   //push new row on top
   for (int tx = 0; tx < playbox_width; tx++)
   {
      playbox[tx] = (tx == 0 || tx == playbox_width - 1) ? 9 : 0;
   }
}


bool Playbox::doesBlockFit(Block block, int movement)
{
   //change block state according to movement
   switch (movement)
   {
   case BLOCK_MOVES_RIGHT:
      block.setX(block.getX() + 1);
      break;
   case BLOCK_MOVES_LEFT:
      block.setX(block.getX() - 1);
      break;
   case BLOCK_MOVES_DOWN:
      block.setY(block.getY() + 1);
      break;
   case BLOCK_ROTATES_ONE:
      block.setRotation(block.getRotation() + 1);
      break;
   }
   for (int px = 0; px < 4; px++)
   {
      for (int py = 0; py < 4; py++)
      {
         //index of piece
         int pi = block.rotatedPos(px, py);


         //px & py in field
         int rx = block.getX() + px;
         int ry = block.getY() + py;


         //index in field
         int fi = ry * playbox_width + rx;


         //check for in bounds
         if (rx >= 0 && rx < playbox_width)
         {
            if (ry >= 0 && ry < playbox_height)
            {
               if (block.getBlock()[pi] != '-' && playbox[fi] != 0)
               {
                  return false;
               }
            }
         }
      }
   }
   return true;
}


void Playbox::init()
{
   //Do all initial var assignments here======
   playbox = NULL;
   playbox = new char[playbox_width * playbox_height];
   gotoxy(50, 20);
   //Create the boundaries for the box========
   for (int y = 0; y < playbox_height; y++)
   {
      for (int x = 0; x < playbox_width; x++)
      {
         playbox[y * playbox_width + x] = (x == 0 || x == playbox_width - 1 ||
                                           y == playbox_height - 1)
                                              ? 9
                                              : 0;
      }
   }
}


void outputPiece(int x)
{
   int t = c.getColorX(x);
   textcolor(t);
   cprintf("�");
}


void draw(Playbox p, Block b)
{
   //draw the playground instance============
   for (int y = 0; y < p.getHeight(); y++)
   {
      for (int x = 0; x < p.getWidth(); x++)
      {
         //moves the cursor to the x and y positions in the playbox
         gotoxy(p.getPosX() + x + 1, p.getPosY() + y + 1);
         outputPiece(p.playbox[y * p.getWidth() + x]);
      }
   }
   for (int px = 0; px < 4; px++)
   {
      for (int py = 0; py < 4; py++)
      {
         if (b.getBlock()[b.rotatedPos(px, py)] != '-')
         {
            gotoxy(p.getPosX() + b.getX() + px + 1, p.getPosY() + b.getY() + py + 1);
            outputPiece(b.getCurrentPiece() + 1);
         }
      }
   }
}


class Score
{
private:
   char name[30];
   int score;
   void inputName();
   void saveScoreToFile();
   void lower(char *str);


public:
   //Constructors=============================
   Score() {}
   Score(Score &score)
   {
      strcpy(name, score.getName());
      this->score = score.getScore();
   }
   //Getters==================================
   int getScore() { return score; }
   char *getName() { return name; }
   //Setters==================================
   void setScore(int score)
   {
      this->score = score;
   }
   void setName(char *name)
   {
      strcpy(this->name, name);
   }
   //Class Functions==========================
   void saveScoreScreen();


protected:
   bool deleteSavedScores();
   friend class Settings;
};


bool Score::deleteSavedScores()
{
   return ((remove("SCORES.DAT") != 0) ? false : true);
}


void Score::lower(char *str)
{
   int len = strlen(str);
   for (int i = 0; i < len; i++)
   {
      str[i] = tolower(str[i]);
   }
}


void Score::saveScoreToFile()
{
   fstream file("SCORES.DAT", ios::in | ios::binary);
   Score store[hscorelimit], temp;
   int numberOfScores = 0;
   int i, j;
   if (!file)
   {
      file.close();
      temp = *this;
      file.open("SCORES.DAT", ios::out | ios::binary);
      file.write((char *)&temp, sizeof(temp));
      file.close();
   }
   else
   {
      for (numberOfScores = 0; !file.eof(); numberOfScores++)
      {
         if (!file.eof())
         {
            file.read((char *)&store[numberOfScores], sizeof(store[0]));
         }
      }
      file.close();
      numberOfScores--;
      store[numberOfScores] = *this;
      numberOfScores++;
      //Insertion Sort=========================
      for (i = 1; i < numberOfScores; i++)
      {
         temp = store[i];
         for (j = i - 1; j >= 0 && (store[j].getScore() < temp.getScore()); j--)
         {
            store[j + 1] = store[j];
         }
         store[j + 1] = temp;
      }
      //Check if last two elements equal=======
      if (numberOfScores == hscorelimit)
      {
         if (store[hscorelimit - 2].getScore() == store[hscorelimit - 1].getScore())
         {
            //Sort via alphabetical order======
            char name1[30], name2[30];
            strcpy(name1, store[hscorelimit - 2].getName());
            strcpy(name2, store[hscorelimit - 1].getName());
            lower(name1);
            lower(name2);
            if (strcmp(name1, name2) > 0)
            {
               store[hscorelimit - 2] = store[hscorelimit - 1];
            }
         }
      }
      file.open("SCORES.DAT", ios::out | ios::binary);
      for (i = 0; i < numberOfScores && i < hscorelimit - 1; i++)
      {
         file.write((char *)&store[i], sizeof(store[i]));
      }
      file.close();
      //Go back to main menu
   }
}


void Score::inputName()
{
   clrscr();
   gotoxy(28, 11);
   textcolor(CYAN);
   cprintf("Name: ");
   gotoxy(28, 12);
   cprintf("Score: %d", score);
   gotoxy(34, 11);
   gets(name);
   saveScoreToFile();
   isGameOn = true;
   gameLoop();
}


void Score::saveScoreScreen()
{
   clrscr();
   char inputchar = '�';
   do
   {
      if (kbhit() || inputchar == '�')
      {
         gotoxy(22, 11);
         textcolor(CYAN);
         cprintf("Would you like to save your score?");
         gotoxy(22, 12);
         cprintf("Score: %d", score);
         gotoxy(22, 13);
         cprintf("[Y]es or [N]o: ");
         inputchar = getch();
         if (inputchar != 'y' && inputchar != 'n' && inputchar != 'Y' && inputchar != 'N' && inputchar != '�')
         {
            cout << "\b";
            cprintf("Invalid input, try again.");
            delay(500);
            int i;
            for (i = 0; i < 25; i++)
            {
               cout << "\b";
            }
            for (i = 0; i < 25; i++)
            {
               cout << " ";
            }
            for (i = 0; i < 25; i++)
            {
               cout << "\b";
            }
         }
      }
   } while (inputchar != 'y' && inputchar != 'n' && inputchar != 'Y' && inputchar != 'N');
   if (inputchar == 'y' || inputchar == 'Y')
   {
      inputName();
   }
   else
   {
      isGameOn = true;
      gameLoop();
   }
}


class Settings
{
private:
   int chosenOne;
   void printConfigOptions();


public:
   //Getters=================================
   int getChosen() { return chosenOne; }
   //Setters=================================
   void setChosen(int chosenOne)
   {
      this->chosenOne = chosenOne;
   }
   //Class Functions=========================
   void drawMenu();
   void runMenu();
   void drawIntro();
   void drawInstructions();
   void configScreen();
   void outputHighScores();
} settings;


void Settings::printConfigOptions()
{
   clrscr();
   gotoxy(5, 5);
   textcolor(RED);
   cprintf("CONFIG GAME");
   textcolor(CYAN);
   gotoxy(5, 7);
   cprintf("[ ] Delete all saved scores");
   gotoxy(5, 8);
   cprintf("[ ] Color of I Block <>");
   gotoxy(5, 9);
   cprintf("[ ] Color of T Block <>");
   gotoxy(5, 10);
   cprintf("[ ] Color of O Block <>");
   gotoxy(5, 11);
   cprintf("[ ] Color of Z Block <>");
   gotoxy(5, 12);
   cprintf("[ ] Color of S Block <>");
   gotoxy(5, 13);
   cprintf("[ ] Color of L Block <>");
   gotoxy(5, 14);
   cprintf("[ ] Color of J Block <>");
   gotoxy(5, 15);
   cprintf("[ ] Color of Background Block <>");
   gotoxy(5, 16);
   cprintf("[ ] Color of Wall Block <>");
   gotoxy(5, 17);
   cprintf("[ ] Reset to default colors");
   gotoxy(5, 18);
   cprintf("[ ] Go back to Main Menu");
}


void Settings::configScreen()
{
   printConfigOptions();
   char inpx;
   int endCheck = false;
   int option;
   char j;
   int col;
   gotoxy(6, 7);
   cprintf("X");
   for (option = 0; endCheck != true;)
   {
      if (kbhit())
      {
         inpx = getch();
         switch (inpx)
         {
         case UP:
            option = (option > 0) ? option - 1 : 11;
            cout << "\b ";
            gotoxy(6, 7 + option);
            cprintf("X");
            break;


         case DOWN:
            option = (option < 11) ? option + 1 : 0;
            cout << "\b ";
            gotoxy(6, 7 + option);
            cprintf("X");
            break;


         case SPACE:
         {
            j = -1;
            switch (option)
            {
            case 0:
               //Delete saved scores
               Score s;
               if (s.deleteSavedScores() == true)
               {
                  clrscr();
                  gotoxy(7, 10);
                  textcolor(RED);
                  cprintf("Scores successfully deleted");
                  delay(3000);
                  printConfigOptions();
               }
               else
               {
                  clrscr();
                  gotoxy(7, 10);
                  textcolor(RED);
                  cprintf("No Saved Scores to Delete");
                  delay(3000);
                  printConfigOptions();
               }
               break;


            case 8:
               //BGBlock color
               gotoxy(36, 7 + option);
               col = c.getColorX(BGBLOCK);
               c.cName(col);
               cprintf(">");
               while (j != SPACE)
               {
                  if (kbhit())
                  {
                     gotoxy(36, 7 + option);
                     j = getch();
                     switch (j)
                     {
                     case RIGHT:
                        col = c.getNextColor(col);
                        c.cName(col);
                        cprintf(">         ");
                        break;
                     case LEFT:
                        col = c.getPrevColor(col);
                        c.cName(col);
                        cprintf(">         ");
                        break;
                     }
                  }
               }
               c.setColor(BGBLOCK, col);
               clrscr();
               printConfigOptions();
               break;


            case 9:
               //WALLBlock color
               gotoxy(30, 7 + option);
               col = c.getColorX(WALLBLOCK);
               c.cName(col);
               cprintf(">");
               while (j != SPACE)
               {
                  if (kbhit())
                  {
                     gotoxy(30, 7 + option);
                     j = getch();
                     switch (j)
                     {
                     case RIGHT:
                        col = c.getNextColor(col);
                        c.cName(col);
                        cprintf(">         ");
                        break;
                     case LEFT:
                        col = c.getPrevColor(col);
                        c.cName(col);
                        cprintf(">         ");
                        break;
                     }
                  }
               }
               c.setColor(WALLBLOCK, col);
               clrscr();
               printConfigOptions();
               break;


            case 10:
               //Reset default colors
               c.resetColors();
               clrscr();
               gotoxy(7, 10);
               textcolor(RED);
               cprintf("Colors successfully reset");
               delay(3000);
               printConfigOptions();
               break;
            case 11:
               //Go back to main menu
               endCheck = true;
               break;


            default:
               //All the other blocks
               if (option > 0 && option < 8)
               {
                  gotoxy(27, 7 + option);
                  //option matches block id
                  col = c.getColorX(option);
                  c.cName(col);
                  cprintf(">");
                  while (j != SPACE)
                  {
                     if (kbhit())
                     {
                        gotoxy(27, 7 + option);
                        j = getch();
                        switch (j)
                        {
                        case RIGHT:
                           col = c.getNextColor(col);
                           c.cName(col);
                           cprintf(">         ");
                           break;
                        case LEFT:
                           col = c.getPrevColor(col);
                           c.cName(col);
                           cprintf(">         ");
                           break;
                        }
                     }
                  }
                  c.setColor(option, col);
                  clrscr();
                  printConfigOptions();
               }
            }
            break;
         }
         }
         gotoxy(6, option + 7);
         cprintf("X");
      }
   }
   clrscr();
   drawMenu();
}


void Settings::drawIntro()
{
   int i, j;
   for (i = 0; i < 100; i++)
   {
      cleardevice();
      settextstyle(10, 0, 8);
      outtextxy(110, i + 30, "TETRIS");
      for (j = 0; j < 1000; j++)
      {
         putpixel(random(620), random(480), random(16));
      }
   }
   delay(4000);
   for (i = 0; i < 100; i++)
   {
      cleardevice();
      settextstyle(5, 1, 6);
      outtextxy(60, i - 15, "A Project By");
      settextstyle(10, 0, 4);
      outtextxy(290 - i, 80, "Kishore");
   }
   delay(1000);
}


void Settings::drawInstructions()
{
   clrscr();
   gotoxy(20, 7);
   textcolor(CYAN);
   cprintf("INSTRUCTIONS: ");
   gotoxy(20, 9);
   textcolor(RED);
   cprintf("LEFT: ");
   textcolor(CYAN);
   cprintf("Move Left");
   gotoxy(20, 11);
   textcolor(RED);
   cprintf("RIGHT: ");
   textcolor(CYAN);
   cprintf("Move Right");
   gotoxy(20, 13);
   textcolor(RED);
   cprintf("DOWN: ");
   textcolor(CYAN);
   cprintf("Move Down");
   gotoxy(20, 15);
   textcolor(RED);
   cprintf("SPACE: ");
   textcolor(CYAN);
   cprintf("Rotate by 90 deg");
   gotoxy(20, 17);
   textcolor(RED);
   cprintf("X: ");
   textcolor(CYAN);
   cprintf("End game");
   gotoxy(20, 19);
   textcolor(CYAN);
   cprintf("Press any key to continue...");
   getch();
   clrscr();
   drawMenu();
}


void Settings::outputHighScores()
{
   clrscr();
   gotoxy(5, 3);
   textcolor(CYAN);
   cprintf("HIGH SCORES");
   //TODO===============
   fstream fin("SCORES.DAT", ios::in | ios::binary);
   if (!fin)
   {
      gotoxy(7, 5);
      textcolor(RED);
      cprintf("There are no saved high scores.");
   }
   else
   {
      Score temp;
      gotoxy(5, 5);
      textcolor(CYAN);
      cprintf("NO.");
      cout << "\t";
      cprintf("NAME");
      gotoxy(39, 5);
      cprintf("SCORES");
      for (int i = 0; !fin.eof(); i++)
      {
         fin.read((char *)&temp, sizeof(temp));
         if (!fin.eof())
         {
            gotoxy(5, 7 + i);
            cout << i + 1 << "\t" << temp.getName();
            gotoxy(40, 7 + i);
            cout << temp.getScore();
         }
      }
      fin.close();
   }
   gotoxy(26, 23);
   textcolor(CYAN);
   cprintf("Press any key to continue...");
   getch();
   clrscr();
   drawMenu();
}


void Settings::runMenu()
{
   clrscr();
   switch (chosenOne)
   {
   case STARTGAME:
   {
      return;
   }
   case INSTRUCTIONS:
   {
      this->drawInstructions();
      break;
   }
   case HIGHSCORES:
   {
      this->outputHighScores();
      break;
   }
   case CONFIGGAME:
   {
      this->configScreen();
      //TODO: Dimens config
      break;
   }
   case ENDGAME:
   {
      exit(0);
      break;
   }
   }
}


void Settings::drawMenu()
{
   int endCheck = 0;
   int pointerY = 10;
   clrscr();
   do
   {
      //Draw menu
      settings.setChosen((pointerY - 10) / 2);
      textcolor(CYAN);
      gotoxy(35, 10);
      cprintf("Start Game");
      gotoxy(35, 12);
      cprintf("Instructions");
      gotoxy(35, 14);
      cprintf("High Scores");
      gotoxy(35, 16);
      cprintf("Config Game");
      gotoxy(35, 18);
      cprintf("Endgame");
      gotoxy(32, pointerY);
      cprintf(">");
      if (kbhit())
      {
         clrscr();
         char i = getch();
         switch (i)
         {
         //TODO: REMOVE AFTER TESTING
         case SPACE:
         {
            endCheck = 1;
            break;
         }
         case DOWN:
         {
            pointerY += (pointerY == 18) ? -8 : 2;
            break;
         }
         case UP:
         {
            pointerY -= (pointerY == 10) ? -8 : 2;
            break;
         }
         }
      }
   } while (endCheck != 1);
   settings.runMenu();
}


void gameLoop()
{
   //Menu Screen=============================
   settings.drawMenu();
   //Enter Menu==============================
   //settings.runMenu();
   //Game setup==============================
   Playbox pg(2, 2, 18, 23);
   Block block;
   block.setX(pg.getWidth() / 2);
   block.setSpeed(2);
   score = 0;
   //Main Game Loop==========================
   while (isGameOn == true)
   {
      //Timing control========================
      if (score <= 1000)
      {
         delay(50);
      }
      else if (score <= 1500)
      {
         delay(40);
      }
      else if (score <= 2000)
      {
         delay(30);
      }
      else if (score <= 2500)
      {
         delay(20);
      }
      else
      {
         delay(10);
      }
      block.setTicks(block.getTicks() + 1);
      bringDown = block.shouldBringPieceDown();


      //Handling keyboard inputs==============
      if (kbhit())
      {
         char inpx = getch();
         //INPUT = RIGHT KEY==================
         if (inpx == RIGHT && pg.doesBlockFit(block, BLOCK_MOVES_RIGHT))
         {
            block.setX(block.getX() + 1);
         }
         //INPUT = LEFT KEY===================
         else if (inpx == LEFT && pg.doesBlockFit(block, BLOCK_MOVES_LEFT))
         {
            block.setX(block.getX() - 1);
         }
         //INPUT = DOWN KEY==================
         else if (inpx == DOWN && pg.doesBlockFit(block, BLOCK_MOVES_DOWN))
         {
            block.setY(block.getY() + 1);
         }
         //INPUT = SPACE KEY (ROTATE)========
         else if (inpx == SPACE && pg.doesBlockFit(block, BLOCK_ROTATES_ONE))
         {
            block.setRotation(block.getRotation() + 1);
         }
         //INPUT = X (Force Quit Testing)====
         else if (inpx == 'X' || inpx == 'x')
         {
            isGameOn = false;
         }
      }
      //Game Logic==========================
      //Bring block down a cell if its time
      if (bringDown)
      {
         block.setTicks(0);
         //Validation check if piece can move down
         if (pg.doesBlockFit(block, BLOCK_MOVES_DOWN))
         {
            //Yes, so move down by one cell
            block.setY(block.getY() + 1);
         }
         else
         {
            //No, so store & lock the block as part of the playbox
            for (int px = 0; px < 4; px++)
            {
               for (int py = 0; py < 4; py++)
               {
                  if (block.getBlock()[block.rotatedPos(px, py)] != '-')
                  {
                     pg.playbox[(block.getY() + py) * pg.getWidth() + (block.getX() + px)] = block.getCurrentPiece() + 1;
                  }
               }
            }
            //Increment score for successful block placement
            score += 50;
            // Check if any lines were formed
            for (int py = 0; py < 4; py++)
            {
               if (block.getY() + py < pg.getHeight() - 1)
               {
                  bool isLineFormed = true;
                  for (int px = 1; px < pg.getWidth() - 1; px++)
                  {
                     if (isLineFormed == true && pg.playbox[(block.getY() + py) * pg.getWidth() + px] == 0)
                     {
                        isLineFormed = false;
                     }
                  }
                  if (isLineFormed)
                  {
                     //Increment score for line formation
                     score += 250;
                     //Form the line
                     for (int x = 0; x < pg.getWidth(); x++)
                     {
                        gotoxy(pg.getPosX() + x + 1, pg.getPosY() + block.getY() + py + 1);
                        cout << "=";
                     }
                     delay(200);
                     //Remove the line
                     pg.removeLine(block.getY() + py);
                  }
               }
            }
            //Select a new piece
            block.setX(pg.getWidth() / 2);
            block.setY(0);
            block.setRotation(DEG_0);
            block.setPiece(random(7));
            //game over logic
            if (pg.doesBlockFit(block) == false)
            {
               isGameOn = false;
               clrscr();
               textcolor(WHITE);
               cprintf("Game Over. Score: %d", score);
               // cout<<score;
            }
         }
      }
      //draw to screen
      draw(pg, block);
   }
   Score sc;
   sc.setScore(score);
   sc.saveScoreScreen();
}


void main()
{
   clrscr();
   init();
   //Draw Intro==============================
   initg();
   settings.drawIntro();
   delay(4000);
   cleardevice();
   closegraph();
   gameLoop();
   getch();
}
