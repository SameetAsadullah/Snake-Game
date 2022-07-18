//============================================================================
// Name        : Tetris.cpp
// Author      : Sibt ul Hussain
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game of Tetris...
//============================================================================
#ifndef TETRIS_CPP_
#define TETRIS_CPP_
#include "util.h"
#include <iostream>
#include<vector>
#include<algorithm>
#include<fstream>
//#include<cstdlib>
#include<ctime>
#include<string>
//#include<sys/wait.h>
//#include<stdlib.h>
//#include<stdio.h>
#include<unistd.h>
#include<cstdlib>
#include<sstream>
#include<cmath>      // for basic math functions such as cos, sin, sqrt
using namespace std;

/* Function sets canvas size (drawing area) in pixels...
 *  that is what dimensions (x and y) your game will have
 *  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
 * */
void SetCanvasSize(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.
    glMatrixMode( GL_MODELVIEW);
    glLoadIdentity();
}

void Movement();
void GenerateFood();
void CheckFood();
void Hurdles();
void Menu();

/*
 * Main Canvas drawing function.
 * */
double startx=320,starty=400;
int X[1000] , Y[1000];
int head_x=32 , head_y=32 , Ttails=0 , speed=130 , direction , menu , y=350;
int Hprev_x , Hprev_y , Sprev_x , Sprev_y , Fcount=0 , Fcount1=0 , Pcount=0 , Tscore=0 , high_score , resumeSize=0;
bool turn_up=false , turn_down=false , turn_left=false , turn_right=false , game_end=false , level=false , hurdle=false , resume_game=false , menu_working=true , resume_size=false;
int food1_x , food1_y , food2_x , food2_y , food3_x , food3_y , food4_x , food4_y , food5_x , food5_y , Pfood_x , Pfood_y;
string score;
int hurd_x1=0 , hurd_y1=0 , hurd_x2=0 , hurd_y2=0 , hurd_x3=0 , hurd_y3=0 , hurd1_x1=0 , hurd1_x2=0 , hurd1_y1=0 , hurd1_y2=0 , hurdles_count=0 , snake_line=400;



void Display(){


	Menu();

	switch(menu)
	{

		case(1):

		menu_working=false;
		
 				// set the background color using function glClearColotgr.
				// to change the background play with the red, green and blue values belo
				// Note that r, g and b values must be in the range [0,1] where 0 means dim rid and 1 means pure red and so on.

			    glClearColor(0/*Red Component*/, 0/*Green Component*/,
	            0/*Blue Component*/, 0 /*Alpha component*/);// Red==Green==Blue==1 --> White Colour
    			glClear(GL_COLOR_BUFFER_BIT);   //Update the colors




				if (direction==13)
				{
					DrawString (250 , 500 , "Move the snake" , colors[WHITE]);			

					turn_up=false;
					turn_down=false;
					turn_left=false;
					turn_right=false;
				}




				score = to_string(Tscore); //score printing

	
	

				DrawLine( 0 , 0 ,  650 , 0, 10 , colors[MISTY_ROSE] ); //boundaries
				DrawLine( 0 , 0 ,  0 , 650 , 10 , colors[MISTY_ROSE] ); //boundaries
				DrawLine( 0 , 650 ,  650 , 650 , 10 , colors[MISTY_ROSE] ); //boundaries
				DrawLine( 650 , 0 ,  650 , 650 , 10 , colors[MISTY_ROSE] ); //boundaries
				DrawLine( 0 , 550 , 650 , 550 , 10 , colors[MISTY_ROSE] ); //line
				DrawString( 50 , 610 , "Your Score = " , colors[WHITE] );//score
				DrawString( 190 , 610 , score , colors[WHITE] );//score
				DrawString( 50 , 575 , "High Score = " , colors[WHITE]);
				DrawString (190 , 575 , to_string(high_score) , colors[WHITE]);
				DrawString( 300 , 592 , "Snake = " , colors[WHITE] );//snake
				DrawLine( 400 , 599 , 630 , 599 , 7 , colors[WHITE]);//snake
				DrawLine( 400 , 599 , snake_line , 599 , 7 , colors[RED]);//snake



				if (snake_line>=630)//level line indicator check
				{
					level=true;
				}


				if (game_end==false)
				{

					if (level==false)
					{

		
						Movement(); //controls movement of the snake
		

		
						if (head_x==64 || head_x==0 || head_y==54 || head_y==0)//collisions with the walls
						{
							game_end=true;	
						}

		
				
						DrawSquare (Hprev_x*10 , Hprev_y*10 , 10 , colors[5]); //tail1
						DrawSquare (Sprev_x*10 , Sprev_y*10 , 10 , colors[5]); //tail2
						DrawCircle ( (head_x*10)+5 , (head_y*10)+5 , 5 , colors[MISTY_ROSE]); //head
		

						
						for (int i=0 ; i<65 ; i++)
						{
							for (int j=0 ; j<65 ; j++)
							{
								for (int k=0 ; k<Ttails ; k++)
								{
									if (X[k]==j && Y[k]==i)
									{
										DrawSquare (j*10 , i*10 , 10 , colors[5]);//length increase and movement
									}
								}
							}
						}


						if (resume_size==true)
						{
							resumeSize++;

							if (resumeSize>5)
							{	
								for (int i=2 ; i<=Ttails ; i++)
								{
									if (X[i]==head_x && Y[i]==head_y)
									{
										game_end=true;//collisions with the snake tail
									}
								}	
							}	
						}

						else
						{
					
							for (int i=2 ; i<=Ttails ; i++)
							{
								if (X[i]==head_x && Y[i]==head_y)
								{
									game_end=true;//collisions with the snake tail
								}
							}
						}	

				
						
						


						//display and check food
					
						Fcount++;
						Fcount1++;
				
						if (Fcount==120 || Fcount1==1)
						{
							GenerateFood();
							Fcount=0;
						}

						for (int i=0 ; i<=Ttails ; i++)//checks for not generating food in snakes tail
						{
							while (X[i]==food1_x && Y[i]==food1_y)
							{
								food1_x= rand() % 63 +1;
								food1_y= rand() % 53 +1;
							}

							while (X[i]==food2_x && Y[i]==food2_y)
							{
								food2_x= rand() % 63 +1;
								food2_y= rand() % 53 +1;
							}
		
							while (X[i]==food3_x && Y[i]==food3_y)
							{
								food3_x= rand() % 63 +1;
								food3_y= rand() % 53 +1;
							}
			
							while (X[i]==food4_x && Y[i]==food4_y)
							{
								food4_x= rand() % 63 +1;
								food4_y= rand() % 53 +1;
							}
			
							while (X[i]==food5_x && Y[i]==food5_y)
							{
								food5_x= rand() % 63 +1;
								food5_y= rand() % 53 +1;
							}
						}
		

						//Drawfood

						DrawCircle (food1_x*10+5 , food1_y*10+5 , 5 , colors[5]);
						DrawCircle (food2_x*10+5 , food2_y*10+5 , 5 , colors[5]);
						DrawCircle (food3_x*10+5 , food3_y*10+5 , 5 , colors[5]);
						DrawCircle (food4_x*10+5 , food4_y*10+5 , 5 , colors[5]);
						DrawCircle (food5_x*10+5 , food5_y*10+5 , 5 , colors[5]);
		

						CheckFood ();//regeneration of the food eaten by snake 

						for (int i=0 ; i<=Ttails ; i++)//checks for not generating that food in snakes tail
						{
							while (X[i]==food1_x && Y[i]==food1_y)
							{
								food1_x= rand() % 63 +1;
								food1_y= rand() % 53 +1;
							}
		
							while (X[i]==food2_x && Y[i]==food2_y)
							{
								food2_x= rand() % 63 +1;
								food2_y= rand() % 53 +1;
							}
		
							while (X[i]==food3_x && Y[i]==food3_y)
							{
								food3_x= rand() % 63 +1;
								food3_y= rand() % 53 +1;
							}

							while (X[i]==food4_x && Y[i]==food4_y)
							{
								food4_x= rand() % 63 +1;
								food4_y= rand() % 53 +1;
							}

							while (X[i]==food5_x && Y[i]==food5_y)
							{
								food5_x= rand() % 63 +1;
								food5_y= rand() % 53 +1;
							}
						}
		
	
		



						//Power Food
				
						Pcount++;
				
						if (Pcount<=600)
						{
							if (Pcount==480)
							{
								Pfood_x= rand() %63 +1;
								Pfood_y= rand() %53 +1;

		
								for (int i=0 ; i<65 ; i++)
								{
									if (X[i]==Pfood_x || Y[i]==Pfood_y)
									{
										Pfood_x= rand() %63 +1;
										Pfood_y= rand() %53 +1;
									}
								}

								while (Pfood_x==food1_x || Pfood_x==food1_x+1 || Pfood_x==food1_x-1 || Pfood_y==food1_y || Pfood_y==food1_y+1 || Pfood_y==food1_y-1 || Pfood_x==food2_x || Pfood_x==food2_x+1 || Pfood_x==food2_x-1 || Pfood_y==food2_y || Pfood_y==food2_y+1 || Pfood_y==food2_y-1 || Pfood_x==food3_x || Pfood_x==food3_x+1 || Pfood_x==food3_x-1 || Pfood_y==food3_y || Pfood_y==food3_y-1 || Pfood_y==food3_y+1 || Pfood_x==food4_x || Pfood_x==food4_x+1 || Pfood_x==food4_x-1 || Pfood_y==food4_y || Pfood_y==food4_y+1 || Pfood_y==food4_y-1 || Pfood_x==food5_x || Pfood_x==food5_x-1 || Pfood_x==food5_x+1 || Pfood_y==food5_y || Pfood_y==food5_y+1 || Pfood_y==food5_y-1)
								{
									Pfood_x= rand() %63 +1;
									Pfood_y= rand() %53 +1;
								} 		

								while ((Pfood_x==hurd_x1 || Pfood_x==hurd_x1+1 || Pfood_x==hurd_x1-1) || (Pfood_y==hurd_y1+1 || Pfood_y==hurd_y1-1 || Pfood_y==hurd_y1) || (Pfood_y==hurd1_y1 || Pfood_y==hurd1_y1+1 || Pfood_y==hurd1_y1-1))
								{
									Pfood_x= rand() %63 +1;
									Pfood_y= rand() %53 +1;
								}

								while (Pfood_x==food1_x || Pfood_x==food2_x || Pfood_x==food3_x || Pfood_x==food4_x || Pfood_x==food5_x)
								{
									Pfood_x= rand() %63 +1;
								}	
			
								while (Pfood_y==food1_y || Pfood_y==food2_y || Pfood_y==food3_y || Pfood_y==food4_y || Pfood_y==food5_y)
								{
									Pfood_y= rand() %53 +1;
								}			
							}
				
		
							if (Pcount>480)
							{
								DrawCircle ( (Pfood_x*10)+5 , (Pfood_y*10)+5 , 10 , colors[MISTY_ROSE]);	
							}
		
		
							if ((abs(Pfood_x-head_x)<=1 && (head_y==Pfood_y || head_y==Pfood_y+1 || head_y==Pfood_y-1)) || (abs(Pfood_y-head_y)<=1 && (head_x==Pfood_x || head_x==Pfood_x+1 || head_x==Pfood_x-1)))
							{
								Tscore+=20;
								Pcount=0;
								Pfood_x=0;
								Pfood_y=0;
								snake_line+=20;
							}
						}
		
		
						else
						{
							Pcount=0;
							Pfood_x=0;
							Pfood_y=0;
						}







						//Hurdles
	
						hurdles_count++;
	
						if (hurdles_count<=240)
						{
		
							if (hurdles_count==1)
							{
								Hurdles();
							}
	

							if (hurdles_count>1)
							{
								DrawLine (hurd_x1*10+5 , hurd_y1*10 , hurd_x2*10+5 , hurd_y2*10 , 10 , colors[MISTY_ROSE]);
								DrawLine (hurd_x2*10+1 , (hurd_y1*10)+4 , hurd_x3*10 , (hurd_y3*10)+4 , 10 , colors[MISTY_ROSE]);
						
								if (hurdle==true)
								{
									DrawLine (hurd1_x1*10 , hurd1_y1*10+5 , hurd1_x2*10 , hurd1_y2*10+5 , 10 , colors[MISTY_ROSE]);
								}				
							}

	
							if ( (head_y>=hurd_y1 && head_y<hurd_y2 && head_x==hurd_x1) || (head_x>=hurd_x1 && head_x<hurd_x3 && head_y==hurd_y1))
							{
								game_end=true;			
							}
		
							if (hurdle==true)
							{
								if (head_y==hurd1_y1 && head_x>=hurd1_x1 && head_x<hurd1_x2)
								{
									game_end=true;					
								}			
							}
						}
	
						else
						{
							hurdles_count=0;
							hurd_x1=0;
							hurd_x2=0;
							hurd_x3=0;
							hurd_y1=0;
							hurd_y2=0;
							hurd_y3=0;
							hurd1_x1=0;
							hurd1_y1=0;
							hurd1_x2=0;
							hurd1_y2=0;
						}		
					}	


					else
					{
						if (speed>0)
						{
							speed-=30;
					
							if (speed==100)
							{
								speed-=20;
							}
			
							else if (speed==80)
							{
								speed-=30;
							}
			
							else if (speed==50)
							{
								speed-=20;
							}
	
							else if (speed==30)
							{
								speed-=20;
							}

						}

						level=false;
						snake_line=400;
						hurdle=true;
					}	
				}


				else
				{
		
					if (Tscore>high_score)//stores highscore in the txt file
					{
						high_score=Tscore;
	
						ofstream highscore("highscore.txt" , ios::out);
			
						highscore<<high_score<<endl;
		
						highscore.close();

					}

					resume_game= true;
					menu_working=true;
					Menu ();
					
					DrawString (260 , 450 , "Game Over." , colors[WHITE]);
					DrawString (260 , 420 , "Your Score = " , colors[WHITE]);
					DrawString (400 , 420 , score , colors[WHITE]);
				}

		break;

	}
	

     
   glutSwapBuffers(); // do not modify this line..
}





void Movement()
{		
		int prev_X= X[0];
		int prev_Y= Y[0];
		int prev_X1 , prev_Y1;

		X[0]=Sprev_x;
		Y[0]=Sprev_y;	

		for (int i=1 ; i<=Ttails ; i++)
		{
			prev_X1= X[i];
			prev_Y1= Y[i];
			X[i]= prev_X;
			Y[i]= prev_Y;
			prev_X= prev_X1;
			prev_Y= prev_Y1;
		}		

		Sprev_x=Hprev_x;
		Sprev_y=Hprev_y;
		Hprev_x=head_x;
		Hprev_y=head_y;
				
	
		if (head_y!=54 && turn_up==true)
		{
			head_y++;	
		}


		if (head_y!=0 && turn_down==true)
		{
			head_y--;
		}


		if (head_x!=0 && turn_left==true)
		{
			head_x--;	
		}


		if (head_x!=64 && turn_right==true)
		{
			head_x++;	
		}
}






void Hurdles()
{
	
	hurd_y1= rand() % 30 +5;
	hurd_x1= rand() % 25 +20;
	
	hurd_y2= hurd_y1+15;	
	hurd_x2= hurd_x1;
	
	hurd_x3= hurd_x1+(hurd_y2-hurd_y1)-5;
	hurd_y3= hurd_y1;

	hurd1_x1= rand () % 7 + 5;
	hurd1_y1= rand () % 30 +5;
	hurd1_y2=hurd1_y1;
	hurd1_x2= hurd_x1-5;
	
	
	if (hurd_y1==32 || hurd_x1==32)
	{
		Hurdles();
	}

	if (hurd_y2==32 || hurd_x2==32)
	{
		Hurdles();
	}


	if (hurd1_x2==32 || hurd1_y2==32)
	{
		Hurdles();
	}


	for (int i=0 ; i<=Ttails ; i++)
	{
		while (X[i]==hurd_x1 && Y[i]>=hurd_y1 && Y[i]<=hurd_y2)
		{
			Hurdles();
		}

		while (Y[i]==hurd_y1 && X[i]>=hurd_x1 && X[i]<=hurd_x3)
		{
			Hurdles();
		}

		while (Y[i]==hurd1_y1 && X[i]>=hurd1_x1 && X[i]<=hurd1_x2)
		{
			Hurdles();
		}
	}

}






void GenerateFood()
{	

	srand(time(NULL));


		food1_x= rand() % 63 +1;
		food1_y= rand() % 53 +1;
		food2_x= rand() % 63 +1;
		food2_y= rand() % 53 +1;
		food3_x= rand() % 63 +1;
		food3_y= rand() % 53 +1;
		food4_x= rand() % 63 +1;
		food4_y= rand() % 53 +1;
		food5_x= rand() % 63 +1;
		food5_y= rand() % 53 +1;


		while (food1_x==food2_x || food1_y==food2_y)
		{
			food1_x= rand() % 63 +1;
			food1_y= rand() % 53 +1;
		}

		while (food1_x==food3_x || food1_y==food3_y)
		{
			food1_x= rand() % 63 +1;
			food1_y= rand() % 53 +1;
		}

		while (food1_x==food4_x || food1_y==food4_y)
		{
			food1_x= rand() % 63 +1;
			food1_y= rand() % 53 +1;	
		}

		while (food1_x==food5_x || food1_y==food5_y)
		{
			food1_x= rand() % 63 +1;
			food1_y= rand() % 53 +1;
		}

		while (food2_x==food3_x || food2_y==food3_y)
		{
			food2_x= rand() % 63 +1;
			food2_y= rand() % 53 +1;
		}

		while (food2_x==food4_x || food2_y==food4_y)
		{
			food2_x= rand() % 63 +1;
			food2_y= rand() % 53 +1;
		}

		while (food2_x==food5_x || food2_y==food5_y)
		{
			food2_x= rand() % 63 +1;
			food2_y= rand() % 53 +1;
		}

		while (food3_x==food4_x || food3_y==food4_y)
		{
			food3_x= rand() % 63 +1;
			food3_y= rand() % 53 +1;
		}

		while (food3_x==food5_x || food3_y==food5_y)
		{
			food3_x= rand() % 63 +1;
			food3_y= rand() % 53 +1;
		}

		while (food4_x==food5_x || food4_y==food5_y)
		{
			food4_x= rand() % 63 +1;
			food4_y= rand() % 53 +1;
		}
		
	
		while ((food1_x==hurd_x1 || food1_x==hurd_x1+1 || food1_x==hurd_x1-1) || (food1_y==hurd_y1 || food1_y==hurd_y1+1 || food1_y==hurd_y1-1))
		{
			food1_x= rand() % 63 +1;
			food1_y= rand() % 53 +1;	
		}

		while ((food2_x==hurd_x1 || food2_x==hurd_x1+1 || food2_x==hurd_x1-1) || (food2_y==hurd_y1 || food2_y==hurd_y1+1 || food2_y==hurd_y1-1))
		{
			food2_x= rand() % 63 +1;
			food2_y= rand() % 53 +1;
		}

		while ((food3_x==hurd_x1 || food3_x==hurd_x1+1 || food3_x==hurd_x1-1) || (food3_y==hurd_y1 || food3_y==hurd_y1+1 || food3_y==hurd_y1-1))
		{
			food3_x= rand() % 63 +1;
			food3_y= rand() % 53 +1;
		}

		while ((food4_x==hurd_x1 || food4_x==hurd_x1+1 || food4_x==hurd_x1-1) || (food4_y==hurd_y1 || food4_y==hurd_y1+1 || food4_y==hurd_y1-1))
		{
			food4_x= rand() % 63 +1;
			food4_y= rand() % 53 +1;
		}

		while ((food5_x==hurd_x1 || food5_x==hurd_x1+1 || food5_x==hurd_x1-1) || (food5_y==hurd_y1 || food5_y==hurd_y1+1 || food5_y==hurd_y1-1))
		{
			food5_x= rand() % 63 +1;
			food5_y= rand() % 53 +1;
		}

		while (food1_y==hurd1_y1 || food1_y==hurd1_y1-1 || food1_y==hurd1_y1+1)
		{
			food1_y= rand() % 53 +1;
		}

		while (food2_y==hurd1_y1 || food2_y==hurd1_y1-1 || food2_y==hurd1_y1+1)
		{
			food2_x= rand() % 63 +1;
			food2_y= rand() % 53 +1;
		}

		while (food3_y==hurd1_y1 || food3_y==hurd1_y1-1 || food3_y==hurd1_y1+1)
		{
			food3_x= rand() % 63 +1;
			food3_y= rand() % 53 +1;
		}

		while (food4_y==hurd1_y1 || food4_y==hurd1_y1-1 || food4_y==hurd1_y1+1)
		{
			food4_x= rand() % 63 +1;
			food4_y= rand() % 53 +1;
		}

		while (food5_y==hurd1_y1 || food5_y==hurd1_y1-1 || food5_y==hurd1_y1+1)
		{
			food5_x= rand() % 63 +1;
			food5_y= rand() % 53 +1;
		}
}		







void CheckFood()
{

		if (head_x==food1_x && head_y==food1_y)
		{
			food1_x= rand() % 63 +1;
			food1_y= rand() % 53 +1;

			while ((food1_x==hurd_x1 || food1_x==hurd_x1+1 || food1_x==hurd_x1-1) || (food1_y==hurd_y1 || food1_y==hurd_y1+1 || food1_y==hurd_y1-1))
			{
				food1_x= rand() % 63 +1;
				food1_y= rand() % 53 +1;	
			}

			while (food1_y==hurd1_y1 || food1_y==hurd1_y1-1 || food1_y==hurd1_y1+1)
			{
				food1_y= rand() % 53 +1;
			}
	
			Tscore+=10;
			Ttails++;
			snake_line+=10;
		}
	
		else if (head_x==food2_x && head_y==food2_y)
		{
			food2_x= rand() % 63 +1;
			food2_y= rand() % 53 +1;

			while ((food2_x==hurd_x1 || food2_x==hurd_x1+1 || food2_x==hurd_x1-1) || (food2_y==hurd_y1 || food2_y==hurd_y1+1 || food2_y==hurd_y1-1))
			{
				food2_x= rand() % 63 +1;
				food2_y= rand() % 53 +1;
			}

			while (food2_y==hurd1_y1 || food2_y==hurd1_y1-1 || food2_y==hurd1_y1+1)
			{
				food2_x= rand() % 63 +1;
				food2_y= rand() % 53 +1;
			}

			Tscore+=10;
			Ttails++;
			snake_line+=10;
		}
	
		else if (head_x==food3_x && head_y==food3_y)
		{
			food3_x= rand() % 63 +1;
			food3_y= rand() % 53 +1;

			while ((food3_x==hurd_x1 || food3_x==hurd_x1+1 || food3_x==hurd_x1-1) || (food3_y==hurd_y1 || food3_y==hurd_y1+1 || food3_y==hurd_y1-1))
			{
				food3_x= rand() % 63 +1;
				food3_y= rand() % 53 +1;
			}

			while (food3_y==hurd1_y1 || food3_y==hurd1_y1-1 || food3_y==hurd1_y1+1)
			{
				food3_x= rand() % 63 +1;
				food3_y= rand() % 53 +1;
			}		
	
			Tscore+=10;
			Ttails++;
			snake_line+=10;
		}
	
		else if (head_x==food4_x && head_y==food4_y)
    	{
			food4_x= rand() % 63 +1;
			food4_y= rand() % 53 +1;

			while ((food4_x==hurd_x1 || food4_x==hurd_x1+1 || food4_x==hurd_x1-1) || (food4_y==hurd_y1 || food4_y==hurd_y1+1 || food4_y==hurd_y1-1))
			{
				food4_x= rand() % 63 +1;
				food4_y= rand() % 53 +1;
			}

			while (food4_y==hurd1_y1 || food4_y==hurd1_y1-1 || food4_y==hurd1_y1+1)
			{
				food4_x= rand() % 63 +1;
				food4_y= rand() % 53 +1;
			}
		
			Tscore+=10;	
			Ttails++;
			snake_line+=10;
		}
	
		else if (head_x==food5_x && head_y==food5_y)
		{
			food5_x = rand() % 63 +1;
			food5_y = rand() % 53 +1;

			while ((food5_x==hurd_x1 || food5_x==hurd_x1+1 || food5_x==hurd_x1-1) || (food5_y==hurd_y1 || food5_y==hurd_y1+1 || food5_y==hurd_y1-1))
			{
				food5_x= rand() % 63 +1;
				food5_y= rand() % 53 +1;
			}

			while (food5_y==hurd1_y1 || food5_y==hurd1_y1-1 || food5_y==hurd1_y1+1)
			{
				food5_x= rand() % 63 +1;
				food5_y= rand() % 53 +1;
			}

			Tscore+=10;
			Ttails++;
			snake_line+=10;
		}	

		
		while (food1_x==food2_x || food1_y==food2_y)
		{
			food1_x= rand() % 63 +1;
			food1_y= rand() % 53 +1;
		}

		while (food1_x==food3_x || food1_y==food3_y)
		{
			food1_x= rand() % 63 +1;
			food1_y= rand() % 53 +1;
		}

		while (food1_x==food4_x || food1_y==food4_y)
		{
			food1_x= rand() % 63 +1;
			food1_y= rand() % 53 +1;	
		}

		while (food1_x==food5_x || food1_y==food5_y)
		{
			food1_x= rand() % 63 +1;
			food1_y= rand() % 53 +1;
		}

		while (food2_x==food3_x || food2_y==food3_y)
		{
			food2_x= rand() % 63 +1;
			food2_y= rand() % 53 +1;
		}

		while (food2_x==food4_x || food2_y==food4_y)
		{
			food2_x= rand() % 63 +1;
			food2_y= rand() % 53 +1;
		}

		while (food2_x==food5_x || food2_y==food5_y)
		{
			food2_x= rand() % 63 +1;
			food2_y= rand() % 53 +1;
		}

		while (food3_x==food4_x || food3_y==food4_y)
		{
			food3_x= rand() % 63 +1;
			food3_y= rand() % 53 +1;
		}

		while (food3_x==food5_x || food3_y==food5_y)
		{
			food3_x= rand() % 63 +1;
			food3_y= rand() % 53 +1;
		}

		while (food4_x==food5_x || food4_y==food5_y)
		{
			food4_x= rand() % 63 +1;
			food4_y= rand() % 53 +1;
		}


}




void Menu()
{
	DrawSquare (0 , 0 , 660 , colors[BLACK]);
	DrawLine( 0 , 0 ,  650 , 0, 10 , colors[MISTY_ROSE] ); //boundaries
	DrawLine( 0 , 0 ,  0 , 650 , 10 , colors[MISTY_ROSE] ); //boundaries
	DrawLine( 0 , 650 ,  650 , 650 , 10 , colors[MISTY_ROSE] ); //boundaries
	DrawLine( 650 , 0 ,  650 , 650 , 10 , colors[MISTY_ROSE] ); //boundaries
	DrawRoundRect(235 , y , 210 , 36 , colors[BLUE] , 8);

	DrawString (260 , 360 , "Start New Game" , colors[WHITE]);
	DrawString (260 , 320 , "Resume Game" , colors[WHITE]);
	DrawString (260 , 280 , "High Score" , colors[WHITE]);
	DrawString (260 , 240 , "Exit" , colors[WHITE]);

	

	if (direction==GLUT_KEY_DOWN && y>230)
	{
		y-=	40;
		direction=0;
	}

	if (direction==GLUT_KEY_UP && y<350)
	{
		y+=40;
		direction=0;
	}

	if (direction==13 && y==350)
	{
		menu=1;

		startx=320,starty=400;
		head_x=32 , head_y=32 , Ttails=0 , speed=130;
		Fcount=0 , Fcount1=0 , Pcount=0 , Tscore=0;
		turn_up=false , turn_down=false , turn_left=false , turn_right=false , game_end=false , level=false , hurdle=false;
		hurd_x1=0 , hurd_y1=0 , hurd_x2=0 , hurd_y2=0 , hurd_x3=0 , hurd_y3=0 , hurd1_x1=0 , hurd1_x2=0 , hurd1_y1=0 , hurd1_y2=0 , hurdles_count=0 , snake_line=400;
	}

	if (direction==13 && y==310)
	{
		if (resume_game==false)
		{
			menu=1;

			startx=320,starty=400;
			head_x=32 , head_y=32 , Ttails=0 , speed=130;
			Fcount=0 , Fcount1=0 , Pcount=0 , Tscore=0;
			turn_up=false , turn_down=false , turn_left=false , turn_right=false , game_end=false , level=false , hurdle=false;
			hurd_x1=0 , hurd_y1=0 , hurd_x2=0 , hurd_y2=0 , hurd_x3=0 , hurd_y3=0 , hurd1_x1=0 , hurd1_x2=0 , hurd1_y1=0 , hurd1_y2=0 , hurdles_count=0 , snake_line=400;			
		}

		if (resume_game==true)
		{
			game_end=false;
			resume_size=true;
	
			resumeSize=0;
			startx=320,starty=400;
			head_x=32,head_y=32;
			turn_up=false , turn_down=false , turn_left=false , turn_right=false , game_end=false;
			
			
	
		}
	}

	if (direction==13 && y==270)
	{
		DrawSquare (0 , 0 , 660 , colors[BLACK]);
		DrawLine( 0 , 0 ,  650 , 0, 10 , colors[MISTY_ROSE] ); //boundaries
		DrawLine( 0 , 0 ,  0 , 650 , 10 , colors[MISTY_ROSE] ); //boundaries
		DrawLine( 0 , 650 ,  650 , 650 , 10 , colors[MISTY_ROSE] ); //boundaries
		DrawLine( 650 , 0 ,  650 , 650 , 10 , colors[MISTY_ROSE] ); //boundaries
		DrawRoundRect(235 , y , 210 , 36 , colors[BLUE] , 8);
		
		DrawString( 253 , 280 , "High Score= " , colors[WHITE]);
		DrawString (383 , 280 , to_string(high_score) , colors[WHITE]);
	}

	if (direction==13 && y==230)
	{
		exit(1);
	}	

}






/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
 * is pressed from the keyboard
 *
 * You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
 *
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 *
 * */

void NonPrintableKeys(int key, int x, int y) {


    if (key == GLUT_KEY_LEFT   /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) {
											// what to do when left key is pressed...

			direction= GLUT_KEY_LEFT;

			if (turn_right!=true)
			{
				turn_left=true;

				turn_up=false;
				turn_down=false;
				turn_right=false;
			}

      
    } else if (key == GLUT_KEY_RIGHT  /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) {


			direction= GLUT_KEY_RIGHT;

			if (turn_left!=true)
			{
				turn_right=true;	

				turn_up=false;
				turn_down=false;
				turn_left=false;
			}
		

    } else if (key == GLUT_KEY_UP) /*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/ {


			direction=  GLUT_KEY_UP;

			if (turn_down!=true)
			{			
				turn_up=true;

				turn_down=false;
				turn_left=false;
				turn_right=false;
			}			

		}

   
    else if (key == GLUT_KEY_DOWN)   /*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/ {


			direction= GLUT_KEY_DOWN;

			if (turn_up!=true)
			{			
				turn_down=true;

				turn_left=false;
				turn_right=false;
				turn_up=false;
			}		
	
    }


    /* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
     * this function*/
     glutPostRedisplay();

}

/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
 * is pressed from the keyboard
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 * */
void PrintableKeys(unsigned char key, int x, int y) {
    if (key == KEY_ESC/* Escape key ASCII*/) {
        exit(1); // exit the program when escape key is pressed.
    }
    if (key == 'R' || key=='r'/* Escape key ASCII*/) {
        
	
    }
    
    else if (int(key) == 13)
    {  
		if (menu_working==true)
		{
			direction=13;
		}	
	}
    
    glutPostRedisplay();
}

/*
 * This function is called after every 1000.0/FPS milliseconds
 * (FPS is defined on in the beginning).
 * You can use this function to animate objects and control the
 * speed of different moving objects by varying the constant FPS.
 *
 * */

void Timer(int m) {

// implement your functionality here
	glutPostRedisplay();
// once again we tell the library to call our Timer function after next 1000/FPS
    glutTimerFunc(speed, Timer, 0);
}


/*
 * our gateway main function
 * */
int main(int argc, char*argv[]) {

	//reads highscore from the txt file
	ifstream highscore;
	highscore.open("highscore.txt" , ios::in);
	highscore>>high_score;
	highscore.close();


    int width = 650, height = 650; // i have set my window size to be 800 x 600
    InitRandomizer(); // seed the random number generator...
    glutInit(&argc, argv); // initialize the graphics library...

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
    glutInitWindowPosition(50, 50); // set the initial position of our window
    glutInitWindowSize(width, height); // set the size of our window
    glutCreateWindow("PF's Snake Game"); // set the title of our game window
    SetCanvasSize(width, height); // set the number of pixels...

// Register your functions to the library,
// you are telling the library names of function to call for different tasks.
//glutDisplayFunc(display); // tell library which function to call for drawing Canvas.
    glutDisplayFunc(Display); // tell library which function to call for drawing Canvas.
    glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
    glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
    glutTimerFunc(5.0 / FPS, Timer, 0);

// now handle the control to library and it will call our registered functions when
// it deems necessary...
    glutMainLoop();
    return 1;
}
#endif /* Snake Game */
