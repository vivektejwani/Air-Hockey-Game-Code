#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <conio.h>

void init();
void display();

static int animeringsmeny;
static int springmeny;
static int win;
static int val = 0;
static int menyid;

int paused = 0;
int touching = 0;
int test = 0;
int startTime;
int prevTime;
int pauseTime=0;
int prevMouseTime;
int elapsedTime;
int plGoalTime, compGoalTime;
int level = 1;
int start= 0;

const float horiSize=700.0;
const float vertSize=700.0;
const float puckSize=20.0;
const float halfLineSize=1.0;
const float goalLineSize=10.0;
const float vertHalfCoord=350.0;
const float halfLineCoord= 350.0;

float malletSize=30.0;
float compMallet[2];
float playerMallet[2];
float playerMalletPrev[2];
float playerMalletVelo[2];
float compMalletVelo[2];
float puckVelo[2];
float puckCoord[2]= {350.0,350.0};
float goal[2]={(700/2)-(0.0625*700)-50 ,(700/2)+(0.0625*700)+50 };
float puckSpeed=20*(float)sqrt(2);
float initial;

long int playerMalletSpeed;
long int compMalletSpeed;

char plScore[4] = "0 0", compScore[4] = "0 0";

pthread_t mouseThread;

//Defines gotoxy() for ANSI C compilers.
void gotoxy(short x, short y) {
COORD pos = {x, y};
SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

/*Distance between 
object with coordinates a[] and object with coordinates b[]*/
float distance(float a[2],float b[2])
{
	float x_sq=(float)pow(a[0]-b[0],2);
	float y_sq=(float)pow(a[1]-b[1],2);
	return (float)sqrt(x_sq+y_sq);
}


void collision()
{
	if(distance(playerMallet,puckCoord)<=(malletSize+puckSize))
	{
		if(playerMalletSpeed==0)
		{
			float x_diff=puckCoord[0]-playerMallet[0];
			float y_diff=puckCoord[1]-playerMallet[1];
			puckSpeed = 14;
			puckVelo[0]=puckSpeed*x_diff/(float)(puckSize+malletSize);
			puckVelo[1]=puckSpeed*y_diff/(float)(puckSize+malletSize);
		}
		else
		{
			if(touching==0)
			{
				/*puckVelo[0]=((playerMalletVelo[0])+(1*puckVelo[0]));
				puckVelo[1]=((playerMalletVelo[1])+(1*puckVelo[1]));
				puckSpeed = (float)sqrt(pow(puckVelo[0],2)+pow(puckVelo[1],2));
				*/
				float x_diff=puckCoord[0]-playerMallet[0];
				float y_diff=puckCoord[1]-playerMallet[1];
				float diff = distance(playerMallet,puckCoord);
				float sint = y_diff/diff;
				float cost = x_diff/diff;
				//printf("%d\n",playerMalletSpeed);
				if(playerMalletSpeed<10)
				{
					puckSpeed = 10;
				}
				else if (playerMalletSpeed<20)
				{
					puckSpeed = 30;
				}
				else if(playerMalletSpeed<40)
				{
					puckSpeed = 40;
				}
				else
				{
					puckSpeed = 50;
				}
				puckVelo[0]= puckSpeed * cost;
				puckVelo[1]= puckSpeed * sint;
						touching = 1;
			}
					
				
		}
		/*
		float x_diff=puckCoord[0]-playerMallet[0];
		float y_diff=puckCoord[1]-playerMallet[1];
		float diff = distance(playerMallet,puckCoord);
		float u1x= ((playerMalletVelo[0]*x_diff)+(playerMalletVelo[1]*y_diff))/diff;
		float u1y= ((playerMalletVelo[0]*y_diff)+(playerMalletVelo[1]*x_diff))/diff;
		float u2x= ((puckVelo[0]*x_diff)+(puckVelo[1]*y_diff))/diff;
		float u2y= ((puckVelo[0]*y_diff)+(puckVelo[1]*x_diff))/diff;
		float v1x= (u1x+(2*u2x))/2;
		float v2x= (u2x+(2*u1x))/2;
		puckVelo[0]= ((v2x*x_diff)+(u2y*y_diff))/diff;
		puckVelo[1]= ((v2x*y_diff)+(u2y*x_diff))/diff;
		puckSpeed = (float)sqrt(pow(puckVelo[0],2)+pow(puckVelo[1],2));
		float sint = puckVelo[1]/puckSpeed;
		float cost = puckVelo[0]/puckSpeed;
		if(puckSpeed>5 && puckSpeed<=10)
		{
			puckSpeed = 10;
		}
		else if (puckSpeed<=20)
		{
			puckSpeed = 20;
		}
		else if (puckSpeed<=30)
		{
			puckSpeed = 30;
		}
		else if (puckSpeed <=40)
		{
			puckSpeed = 40;
		}
		else
			puckSpeed = 50;
		//puckSpeed=5; // test
		puckVelo[0]=puckSpeed*cost;
		puckVelo[1]=puckSpeed*sint;
		
	}
		float x_diff=puckCoord[0]-playerMallet[0];
		float y_diff=puckCoord[1]-playerMallet[1];
		float diff = distance(playerMallet,puckCoord);
		float sint = y_diff/diff;
		float cost = x_diff/diff;
		//printf("%d\n",playerMalletSpeed);
		if(playerMalletSpeed<10)
		{
			puckSpeed = 10;
		}
		else if (playerMalletSpeed<20)
		{
			puckSpeed = 30;
		}
		else if(playerMalletSpeed<40)
		{
			puckSpeed = 40;
		}
		else
		{
			puckSpeed = 50;
		}
		puckVelo[0]= puckSpeed * cost;
		puckVelo[1]= puckSpeed * sint;*/
	}
	else if(distance(compMallet,puckCoord)<=(malletSize+puckSize))
	{
		/*float x_diff=puckCoord[0]-compMallet[0];
		float y_diff=puckCoord[1]-compMallet[1];
		puckVelo[0]=puckSpeed*x_diff/(float)(puckSize+malletSize);
		puckVelo[1]=puckSpeed*y_diff/(float)(puckSize+malletSize);*/
		
		if(compMalletSpeed==0)
		{
			float x_diff=puckCoord[0]-compMallet[0];
			float y_diff=puckCoord[1]-compMallet[1];
			puckSpeed = 14;
			puckVelo[0]=puckSpeed*x_diff/(float)(puckSize+malletSize);
			puckVelo[1]=puckSpeed*y_diff/(float)(puckSize+malletSize);
		}
		else
		{
			if(touching==0)
			{
				/*puckVelo[0]=((playerMalletVelo[0])+(1*puckVelo[0]));
				puckVelo[1]=((playerMalletVelo[1])+(1*puckVelo[1]));
				puckSpeed = (float)sqrt(pow(puckVelo[0],2)+pow(puckVelo[1],2));
				*/
				float x_diff=puckCoord[0]-compMallet[0];
				float y_diff=puckCoord[1]-compMallet[1];
				float diff = distance(compMallet,puckCoord);
				float sint = y_diff/diff;
				float cost = x_diff/diff;
				//printf("%d\n",playerMalletSpeed);
				if(compMalletSpeed<1)
				{
					puckSpeed = 10;
				}
				else if (playerMalletSpeed<2)
				{
					puckSpeed = 30;
				}
				else if(playerMalletSpeed<4)
				{
					puckSpeed = 40;
				}
				else
				{
					puckSpeed = 50;
				}
				puckVelo[0]= puckSpeed * cost;
				puckVelo[1]= puckSpeed * sint;
						touching = 1;
			}
					
				
		}
	} 
	else
	{
		touching = 0;
	}
}


void drawBitmapText(char *string, float x, float y, float z)
{
    char *c;
    glRasterPos3f(x, y, z);
    //glScalef(0.5, 0.5, 1);

    for (c=string; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
}

void drawText(char *string, float transX, float transY, float scaleX, float scaleY)
{
	char *c;
	glTranslatef(transX, transY, 0);
	glScalef(scaleX, scaleY, 0);
	for (c=string; *c != '\0'; c++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
    }
	glScalef(1/scaleX, 1/scaleY, 0);
	glTranslatef(-transX, -transY, 0);
}
void drawMallet(float a, float b)
{
	float x1,y1,x2,y2;
	float angle;
	float radius=malletSize;
 	x1 = a,y1 = b;
 	float i=1.0,j=1.0,k=1.0;
	glColor3f(i,i,i);
 	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1,y1);
	for(angle=0.0f ; angle<=2*3.14159 ; angle+=0.01)
	{
	    x2 = x1+sin(angle)*radius;
	    y2 = y1+cos(angle)*radius;
	    if(i>=0)
	    {
	    	i=i-(20/(2*3.14159));
	    	glColor3f(i,i,i);
	    }
	    glVertex2f(x2,y2);
	}
	glEnd();
	//glutSolidSphere(10,20,20);
}

void drawPuck(float x1,float y1) 
{ 
	float x2,y2;
	float angle;
	float radius=puckSize;
	float i=0.4, j= 0.4;
	glColor3f(1.0,i,j);
 	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1,y1);
	for(angle=0.0f ; angle<=2*3.14159 ; angle+=0.01)
	{
	    x2 = x1+sin(angle)*radius;
	    y2 = y1+cos(angle)*radius;
	    if(i>=0&&j>=0)
	    {
	    	i=i-(0.4/2*3.14159*100);
	    	j=j-(0.4/2*3.14159*100);
	    }
	    glColor3f(1.0,i,j);
	    glVertex2f(x2,y2);
	}
	glEnd();
//	glutSolidSphere(10,20,20);
}

void drawGoalCircles()
{
	float x1,y1,x2,y2;
	float angle;
	float radius=50 + (0.0625*800);
 	x1 = horiSize/2,y1=0;
	glColor3f(1.0,1.0,1.0);
 	glBegin(GL_POINTS);
	glVertex2f(x1,y1);
	for(angle=0.0f ; angle<=2*3.14159 ; angle+=0.01)
	{
	    x2 = x1+sin(angle)*radius;
	    y2 = y1+cos(angle)*radius;
	    glVertex2f(x2,y2);
	}
	glEnd();
 	x1 = horiSize/2,y1=vertSize;
	glColor3f(1.0,1.0,1.0);
 	glBegin(GL_POINTS);
	glVertex2f(x1,y1);
	for(angle=0.0f ; angle<=2*3.14159 ; angle+=0.01)
	{
	    x2 = x1+sin(angle)*radius;
	    y2 = y1+cos(angle)*radius;
	    glVertex2f(x2,y2);
	}
	glEnd();
}

void drawCenter()
{
	float x1,y1,x2,y2;
	float angle;
	float radius=vertSize / 8;
 	x1 = horiSize/2,y1=halfLineCoord;
	glColor3f(1.0,1.0,1.0);
 	glBegin(GL_POINTS);
	glVertex2f(x1,y1);
	for(angle=0.0f ; angle<=2*3.14159 ; angle+=0.01)
	{
	    x2 = x1+sin(angle)*radius;
	    y2 = y1+cos(angle)*radius;
	    glVertex2f(x2,y2);
	}	 
	glEnd();
}

void init()
{	
    glEnable(GL_LIGHTING);
    glClearColor(0.3,0.3,1.0,0.5);
    glDisable(GL_LIGHTING);
    gluOrtho2D(0.0,horiSize, 0.0, vertSize);    
}

void halfLine()
{
	
	glLineWidth(halfLineSize);
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_LINES);
		glVertex2f(0,halfLineCoord);
		glVertex2f(horiSize,halfLineCoord);
	glEnd();
}

void drawgoals()
{
	glLineWidth(goalLineSize);
	
	glBegin(GL_LINES);
		glVertex2f(goal[0],vertSize);
		glVertex2f(goal[1],vertSize);
	glEnd();
	glBegin(GL_LINES);
		glVertex2f(goal[0],0);
		glVertex2f(goal[1],0);
	glEnd();
}

void plGoal()
{ 
	int currTime = glutGet(GLUT_ELAPSED_TIME);
	if(currTime - plGoalTime < 1000)
	{
		glColor3f(255, 0, 0);
	}
	if(currTime - plGoalTime > 1000 && currTime - plGoalTime < 1500)
	{
		glColor3f(0, 255, 0);
	}
	if(currTime - plGoalTime > 1500 && currTime - plGoalTime < 2000)
	{
		glColor3f(255, 255, 0);
	}
	if(currTime - plGoalTime > 3000)
	{
		plGoalTime = 0;
	}
	if(plGoalTime == 0)
	{
		glColor3f(255, 255, 255);
	}
	glPushMatrix();
	drawBitmapText(plScore, 650, 275, 0);
	if(plGoalTime != 0)
	{
	   drawBitmapText("GOAL !", 300, 200, 0);
	   if(plScore[2] == '7')
	   {
	   	   drawBitmapText("YOU WIN!", 300, 500, 0);
	   	   glPushMatrix();
		   glColor3f(0, 55, 0);
		   drawBitmapText("Next Level!", 310, 300, 0);
		   glPopMatrix();
	   }
    }
	glPopMatrix();
	glutPostRedisplay();
	
}

void compGoal()
{
	int currTime = glutGet(GLUT_ELAPSED_TIME);
	if(currTime - compGoalTime < 1000)
	{
		glColor3f(255, 0, 0);
	}
	if(currTime - compGoalTime > 1000 && currTime - compGoalTime < 1500)
	{
		glColor3f(0, 255, 0);
	}
	if(currTime - compGoalTime > 1500 && currTime - compGoalTime < 2000)
	{
		glColor3f(255, 255, 0);
	}
	if(currTime - compGoalTime > 3000)
	{
		compGoalTime = 0;
	}
	if(compGoalTime == 0)
	{
		glColor3f(255, 255, 255);
	}
	glPushMatrix();
	drawBitmapText(compScore, 650, 425, 0);
	if(compGoalTime != 0)
	{
	   drawBitmapText("GOAL !", 300, 500, 0);
	   if(compScore[2] == '7')
	   {
	   	   drawBitmapText("YOU LOSE!", 300, 200, 0);
	   	   glPushMatrix();
		   glColor3f(0, 55, 0);
		   drawBitmapText("TRY AGAIN!", 300, 300, 0);
		   glPopMatrix();
	   }
    }
	glPopMatrix();
	glutPostRedisplay();
}

void *puckTranslate()
{
		collision();
		if(puckCoord[1]>=vertSize-puckSize && puckCoord[0]>=goal[0]+puckSize&&puckCoord[0]<=goal[1]-puckSize)
		{
			start=0;
			if(plScore[1] != '9')
			{
			    plScore[0] = plScore[0];
			    plScore[2] = plScore[2] + 1;
			    plScore[3] = '\0';
			}
			plGoalTime = glutGet(GLUT_ELAPSED_TIME);
			plGoal();
		}
		if(puckCoord[1]<=puckSize && puckCoord[0]>=goal[0]+puckSize&&puckCoord[0]<=goal[1]-puckSize)
		{
			start=0;
			if(compScore[1] != '9')
			{
			    compScore[0] = compScore[0];
			    compScore[2] = compScore[2] + 1;
			    compScore[3] = '\0';
			}
			compGoalTime = glutGet(GLUT_ELAPSED_TIME);
			compGoal();
		}
		if(puckCoord[1]>=vertSize-puckSize||puckCoord[1]<=puckSize)
		{
			if(puckCoord[0]>=goal[0]+puckSize&&puckCoord[0]<=goal[1]-puckSize)
			{
				start=0;
			}
			else
			{
				if(puckCoord[1]>=vertSize-puckSize)
				{
					if(puckVelo[1]>0)
					{
						float temp =2* puckVelo[1];
						puckVelo[1]-=temp;
					}
				}
				else
				{
					if(puckVelo[1]<0)
					{
						float temp =2* puckVelo[1];
						puckVelo[1]-=temp;
					}
				}
//				k=1;
			}
		}
		if(puckCoord[0]<=puckSize||puckCoord[0]>=horiSize-puckSize)
		{
			if(puckCoord[0]<=puckSize)
			{
				if(puckVelo[0]<0)
				{
					float temp = 2 * puckVelo[0];
					puckVelo[0]-=temp;
				}
			}//			k=1;
			else
			{
				if(puckVelo[0]>0)
				{
					float temp = 2 * puckVelo[0];
					puckVelo[0]-=temp;
				}
			}
		}
		puckCoord[0]+=puckVelo[0]*elapsedTime/35;
		puckCoord[1]+=puckVelo[1]*elapsedTime/35;
		glutPostRedisplay();
}
void drawSpots()
{
	float l = 0, m = 0,i= 0,j= 0; 
    for(i = l; i < vertSize+20; i= i+ 20)
    {
    	for(j = m; j<horiSize+20; j = j + 20)
    	{
    		glPointSize(1.0);
    		glColor3f(1.0, 1.0, 1.0);
    		glBegin(GL_POINTS);
        	glVertex2f(j,i);
    		glEnd();    		
    	}
    	if(m==0)
    		m=1;
    }
}

void mouse(int mouse, int state, int x, int y){
	
    switch(mouse){
        case GLUT_LEFT_BUTTON:
            if(start==0 && state == GLUT_DOWN){
	                start=1;
	                puckVelo[0]=10;
	                puckVelo[1]=10;
	                if(plScore[2]=='7')
					{	
	                	level++;
	                	plScore[2]='0';
	                	compScore[2]='0';  	
	                }
	                if(compScore[2]=='7')
					{
						
	                	plScore[2]='0';
	                	compScore[2]='0';
	                	
	                }
	                
	                
	                glutPostRedisplay();
            }
        break;
    }
}

void drag(int x, int y)
{
	
	if(start==1&&paused==0)
	{
		if((y<=vertSize-malletSize && y>=halfLineCoord) && (x>=0+malletSize&&x<=horiSize-malletSize))
		{
				int currMouseTime = glutGet(GLUT_ELAPSED_TIME);
				int elapsedMouseTime = currMouseTime - prevMouseTime;
				playerMallet[0] = x/1.0;
    			playerMallet[1] = vertSize - (y/1.0);
    			playerMalletVelo[0]=10*(playerMallet[0]-playerMalletPrev[0])/(elapsedMouseTime);
				playerMalletVelo[1]=10*(playerMallet[1]-playerMalletPrev[1])/(elapsedMouseTime);
				playerMalletSpeed = (float)sqrt(pow(playerMalletVelo[0],2)+pow(playerMalletVelo[1],2));
				playerMalletSpeed*=10;
  				playerMalletPrev[0]=playerMallet[0];
  				playerMalletPrev[1]=playerMallet[1];
  				prevMouseTime=currMouseTime;
				glutPostRedisplay();
  		}
  		else
  		{
  			glutWarpPointer(playerMallet[0], vertSize - playerMallet[1]);
  		}
		  		
    }    
}

void *mouseFunction(void *args)
{
	glutPassiveMotionFunc(drag);
    glutMouseFunc(mouse);
}

void compTranslate()
{
	
	if(level==1)
	{
		if((compMallet[1] <= vertSize-malletSize && compMallet[1]>=halfLineCoord) && (compMallet[0]>=malletSize&&compMallet[0]<=horiSize-malletSize))
		{
			if(puckCoord[1] < halfLineCoord)  // puck in player's half
			{
				if(compMallet[0]!=vertHalfCoord)
				{
					float temp = compMallet[0]-vertHalfCoord;
					if(temp>0)
					{
						compMalletVelo[0] = -0.25;
					}
					else if(temp<0)
					{
						compMalletVelo[0] = 0.25;
					}
				}
				else
				{
					compMalletVelo[0]=0;
				}
				if(compMallet[1]!=((3*vertSize/4)+(vertSize/8)))
				{
					float temp = compMallet[1]-((3*vertSize/4)+(vertSize/8));
					if(temp>0)
					{
						compMalletVelo[1] = -0.25;
					}
					else if(temp<0)
					{
						compMalletVelo[1] = 0.25;
					}
				}
				else
				{
					compMalletVelo[1]=0;
				}
			}
			else
			{
				/*if(puckVelo[1]<0)
				{
					if(compMallet[0]!=vertHalfCoord)
					{
						float temp = compMallet[0]-vertHalfCoord;
						if(temp>0)
						{
							compMalletVelo[0] = -0.25;
						}
						else if(temp<0)
						{
							compMalletVelo[0] = 0.25;
						}
					}
					else
					{
						compMalletVelo[0]=0;
					}
					if(compMallet[1]!=((3*vertSize/4)+(vertSize/8)))
					{
						float temp = compMallet[1]-((3*vertSize/4)+(vertSize/8));
						if(temp>0)
						{
							compMalletVelo[1] = -0.25;
						}
						else if(temp<0)
						{
							compMalletVelo[1] = 0.25;
						}
					}
					else
					{
						compMalletVelo[1]=0;
					}
				}
				else
				{*/

				if(puckCoord[0]>compMallet[0])
				{
					compMalletVelo[0]=0.25;
				}
				else if (puckCoord[0]<compMallet[0])
				{
					compMalletVelo[0]=-0.25;
				}
				else
				{
					compMalletVelo[0]=0;
				}
				
				if(puckCoord[1]>compMallet[1])
				{
					compMalletVelo[1]=0.25;
				}
				
				else if(puckCoord[1]<compMallet[1])
				{
					compMalletVelo[1]=-0.25;
				}
				
				else
				{
					compMalletVelo[1]=0;
				}
				
				
				/*if(puckCoord[1]>=(vertSize - (2*puckSize)))
				{
					if(puckCoord[0]<=(2*puckSize))
					{
						compMalletVelo[0]=0.25;
						compMalletVelo[1]=-0.25;
					}
					else if (puckCoord[0]>=(horiSize-(2*puckSize)))
					{
						compMalletVelo[0]=-0.25;
						compMalletVelo[1]=-0.25;
					}
					else
					{
						if(puckCoord[0]==compMallet[0])
						{
							compMalletVelo[0]=0.25;
							compMalletVelo[1]=-0.25;
						}
					}
				}
				if((puckCoord[0]<=(2*puckSize))||(puckCoord[0]>=(horiSize-(2*puckSize))))
				{
					if(puckCoord[1]==compMallet[1])
					{
						compMalletVelo[0]=0;
						compMalletVelo[1]=-0.25;
					}
				}*/
			}
		}
	}
	else if(level==2)
	{
		compMalletVelo[1]=0;
		if((compMallet[1] <= vertSize-malletSize && compMallet[1]>=halfLineCoord) && (compMallet[0]>=malletSize&&compMallet[0]<=horiSize-malletSize))
		{
			//printf("test %d\n",test);
			if(puckCoord[1] < halfLineCoord)  // puck in player's half
			{
				if(compMallet[0]!=vertHalfCoord)
				{
					float temp = compMallet[0]-vertHalfCoord;
					if(temp>0)
					{
						compMalletVelo[0] = -0.5;
					}
					if(temp<0)
					{
						compMalletVelo[0] = 0.5;
					}
				}
				else
				{
					compMalletVelo[0]=0;
				}
			}
			else
			{
				if(puckVelo[1]<0)
				{
					if(compMallet[0]!=vertHalfCoord)
					{
						float temp = compMallet[0]-vertHalfCoord;
						if(temp>0)
						{
							compMalletVelo[0] = -0.5;
						}
						if(temp<0)
						{
							compMalletVelo[0] = 0.5;
						}
					}
					else
					{
						compMalletVelo[0]=0;
					}					
				}
				else
				{
					if(puckCoord[0]<=(goal[0])/2)
					{
						if(compMallet[0]<goal[0])
						{
							compMalletVelo[0]=0.5;
						}
						else if(compMallet[0]>goal[0])
						{
							compMalletVelo[0]=-0.5;
						}
						else
						{
							compMalletVelo[0]=0;
						}
					}
					else if(puckCoord[0]>(goal[0])/2 && puckCoord[0]<=(goal[0]+goal[1])/2)
					{
						if(compMallet[0]<(vertHalfCoord+puckCoord[0])/2)
						{
							compMalletVelo[0]=0.5;
						}
						else if(compMallet[0]>(vertHalfCoord+puckCoord[0])/2)
						{
							compMalletVelo[0]=-0.5;
						}
						else
						{
							compMalletVelo[0]=0;
						}
					}
					else if(puckCoord[0]>(goal[0]+goal[1])/2 && puckCoord[0]<=(goal[1]+horiSize)/2)
					{
						if(compMallet[0]<(vertHalfCoord+puckCoord[0])/2)
						{
							compMalletVelo[0]=0.5;
						}
						else if(compMallet[0]>(vertHalfCoord+puckCoord[0])/2)
						{
							compMalletVelo[0]=-0.5;
						}
						else
						{
							compMalletVelo[0]=0;
						}
					}
					else
					{
						if(compMallet[0]<goal[1])
						{
							compMalletVelo[0]=0.5;
						}
						else if(compMallet[0]>goal[1])
						{
							compMalletVelo[0]=-0.5;
						}
						else
						{
							compMalletVelo[0]=0;
						}
					}
				}
			}
			
		}
	}
	else if(level == 3)
	{
		if((compMallet[1] <= vertSize-malletSize && compMallet[1]>=halfLineCoord) && (compMallet[0]>=malletSize&&compMallet[0]<=horiSize-malletSize))
		{
			if(puckCoord[1] < halfLineCoord)  // puck in player's half
			{
				if(compMallet[1] > (3*vertSize/4)+(vertSize/8))
				{
					compMalletVelo[1] = 0;
				}
				if(compMallet[0]!=vertHalfCoord)
				{
					float temp = compMallet[0]-vertHalfCoord;
					if(temp>0)
					{
						compMalletVelo[0] = -1;
					}
					if(temp<0)
					{
						compMalletVelo[0] = 1;
					}
				}
				else
				{
					compMalletVelo[0]=0;
				}
			}
			else
			{
				
				//Region 1
				if(puckCoord[0] < goal[0] / 2)
				{
					//angle:quadrant 1
					if(puckVelo[0] > 0 && puckVelo[1] >= 0)
					{
						if(compMallet[0] < (goal[0] + vertHalfCoord)/2)
						{
							compMalletVelo[0] = 1;
						}
						else if(compMallet[0] > (goal[0] + vertHalfCoord)/2)
						{
							compMalletVelo[0] = -1;
						}
						else
						{
							compMalletVelo[0]= 0;
						}
						if(compMallet[1] < (3*vertSize/4)+(vertSize/8))
						{
							compMalletVelo[1] = 1;
						}
						else if(compMallet[1] > (3*vertSize/4)+(vertSize/8))
						{
							compMalletVelo[1] = -1;
						}
						else
						{
							compMalletVelo[1]=0;
						}
					}
					//angle:quadrant 2
					if(puckVelo[0] <= 0 && puckVelo[1] > 0)
					{
						if(compMallet[0] < goal[0])
						{
							compMalletVelo[0] = 1;
						}
						else if(compMallet[0] > goal[0])
						{
							compMalletVelo[0] = -1;
						}
						else
						{
							compMalletVelo[0] = 0;
						}
						if(compMallet[1] < (3*vertSize/4)+(vertSize/8))
						{
							compMalletVelo[1] = 1;
						}
						else if(compMallet[1] > (3*vertSize/4)+(vertSize/8))
						{
							compMalletVelo[1] = -1;
						}
						else
						{
							compMalletVelo[1]=0;
						}
					}
					//angle:quadrant 3
					if(puckVelo[0] < 0 && puckVelo[1] <= 0)
					{
						if(puckCoord[0] < compMallet[0])
						{
							compMalletVelo[0] = -1;
						}
						if(puckCoord[1] < compMallet[1])
						{
							compMalletVelo[1] = -1;
						}
						if(puckCoord[0] > compMallet[0])
						{
							compMalletVelo[0] = 1;
						}
						if(puckCoord[1] > compMallet[1])
						{
							compMalletVelo[1] = 1;
						}
						if(distance(compMallet,puckCoord)<=(malletSize+puckSize))
						{
							if(compMallet[0] < vertHalfCoord)
							{
								compMalletVelo[0] = 1;
							}
							if(compMallet[1] < (3*vertSize/4)+(vertSize/8))
							{
								compMalletVelo[1] = 1;
							}
							if(compMallet[0] > vertHalfCoord)
							{
								compMalletVelo[0] = -1;
							}
							if(compMallet[1] >= (3*vertSize/4)+(vertSize/8))
							{
								compMalletVelo[1] = 0;
							}
							if(compMallet[0] == vertHalfCoord)
							{
								compMalletVelo[0] = 0;
							}
						}	
					}
					//angle:quadrant 4
					if(puckVelo[0] >= 0 && puckVelo[1] < 0)
					{
						if(puckCoord[0] < compMallet[0])
						{
							compMalletVelo[0] = -1;
						}
						if(puckCoord[1] < compMallet[1])
						{
							compMalletVelo[1] = -1;
						}
						if(puckCoord[0] > compMallet[0])
						{
							compMalletVelo[0] = 1;
						}
						if(puckCoord[1] > compMallet[1])
						{
							compMalletVelo[1] = 1;
						}
						if(distance(compMallet,puckCoord)<=(malletSize+puckSize))
						{
							if(compMallet[0] < vertHalfCoord)
							{
								compMalletVelo[0] = 1;
							}
							if(compMallet[1] < (3*vertSize/4)+(vertSize/8))
							{
								compMalletVelo[1] = 1;
							}
							if(compMallet[0] > vertHalfCoord)
							{
								compMalletVelo[0] = -1;
							}
							if(compMallet[1] >= (3*vertSize/4)+(vertSize/8))
							{
								compMalletVelo[1] = 0;
							}
							if(compMallet[0] == vertHalfCoord)
							{
								compMalletVelo[0] = 0;
							}
						}	
					}
					
				}
				//Region 4
				if(puckCoord[0] >= (goal[1] + horiSize) / 2)
				{
					//angle:quadrant 2
					if(puckVelo[0] <= 0 && puckVelo[1] > 0)
					{
						if(compMallet[0] < (goal[1] + vertHalfCoord)/2)
						{
							compMalletVelo[0] = 1;
						}
						if(compMallet[1] < (3*vertSize/4)+(vertSize/8))
						{
							compMalletVelo[1] = 1;
						}
						if(compMallet[0] > (goal[1] + vertHalfCoord)/2)
						{
							compMalletVelo[0] = -1;
						}
						if(compMallet[1] > (3*vertSize/4)+(vertSize/8))
						{
							compMalletVelo[1] = -1;
						}
						if(compMallet[0]==(goal[1]+vertHalfCoord)/2)
						{
							compMalletVelo[0]=0;
						}
						if(compMallet[1]==(3*vertSize/4)+(vertSize/8))
						{
							compMalletVelo[1]=0;
						}
					}
					//angle:quadrant 1
					if(puckVelo[0] > 0 && puckVelo[1] >= 0)
					{
						if(compMallet[0] < goal[1])
						{
							compMalletVelo[0] = 1;
						}
						if(compMallet[1] < (3*vertSize/4)+(vertSize/8))
						{
							compMalletVelo[1] = 1;
						}
						if(compMallet[0] > goal[1])
						{
							compMalletVelo[0] = -1;
						}
						if(compMallet[1] > (3*vertSize/4)+(vertSize/8))
						{
							compMalletVelo[1] = -1;
						}
						if(compMallet[0]==goal[1])
						{
							compMalletVelo[0]=0;
						}
						if(compMallet[1]==(3*vertSize/4)+(vertSize/8))
						{
							compMalletVelo[1]=0;
						}
					}
					//angle:quadrant 3
					if(puckVelo[0] >= 0 && puckVelo[1] < 0)
					{
						if(puckCoord[0] < compMallet[0])
						{
							compMalletVelo[0] = -1;
						}
						if(puckCoord[1] < compMallet[1])
						{
							compMalletVelo[1] = -1;
						}
						if(puckCoord[0] > compMallet[0])
						{
							compMalletVelo[0] = 1;
						}
						if(puckCoord[1] > compMallet[1])
						{
							compMalletVelo[1] = 1;
						}
						if(distance(compMallet,puckCoord)<=(malletSize+puckSize))
						{
							if(compMallet[0] < vertHalfCoord)
							{
								compMalletVelo[0] = 1;
							}
							if(compMallet[1] < (3*vertSize/4)+(vertSize/8))
							{
								compMalletVelo[1] = 1;
							}
							if(compMallet[0] > vertHalfCoord)
							{
								compMalletVelo[0] = -1;
							}
							
							if(compMallet[1] >= (3*vertSize/4)+(vertSize/8))
							{
								compMalletVelo[1] = 0;
							}
							if(compMallet[0]==vertHalfCoord)
							{
								compMalletVelo[0]=0;
							}
						}	
					}
					//angle:quadrant 4
					if(puckVelo[0] < 0 && puckVelo[1] <= 0)
					{
						if(puckCoord[0] < compMallet[0])
						{
							compMalletVelo[0] = -1;
						}
						if(puckCoord[1] < compMallet[1])
						{
							compMalletVelo[1] = -1;
						}
						if(puckCoord[0] > compMallet[0])
						{
							compMalletVelo[0] = 1;
						}
						if(puckCoord[1] > compMallet[1])
						{
							compMalletVelo[1] = 1;
						}
						if(distance(compMallet,puckCoord)<=(malletSize+puckSize))
						{
							if(compMallet[0] < vertHalfCoord)
							{
								compMalletVelo[0] = 2;
							}
							if(compMallet[1] < (3*vertSize/4)+(vertSize/8))
							{
								compMalletVelo[1] = 2;
							}
							if(compMallet[0] > vertHalfCoord)
							{
								compMalletVelo[0] = -2;
							}
							
							if(compMallet[1] >= (3*vertSize/4)+(vertSize/8))
							{
								compMalletVelo[1] = 0;
							}
							if(compMallet[0]==vertHalfCoord)
							{
								compMalletVelo[0]=0;
							}
						}	
					}
					
				}
				
				//Region 2
				if(puckCoord[0] >= goal[0] / 2 && puckCoord[0] < vertHalfCoord)
				{
					//angle:quadrant 2
					if(puckVelo[0] <= 0 && puckVelo[1] > 0)
					{
						if(compMallet[0] < (goal[0] + vertHalfCoord)/2)
						{
							compMalletVelo[0] = 1;
						}
						if(compMallet[1] < (3*vertSize/4)+(vertSize/8))
						{
							compMalletVelo[1] = 1;
						}
						if(compMallet[0] > (goal[0] + vertHalfCoord)/2)
						{
							compMalletVelo[0] = -1;
						}
						if(compMallet[1] > (3*vertSize/4)+(vertSize/8))
						{
							compMalletVelo[1] = -1;
						}
						if(compMallet[0]==(goal[0] + vertHalfCoord)/2)
						{
							compMalletVelo[0]=0;
						}
						if(compMallet[1]==(3*vertSize/4)+(vertSize/8))
						{
							compMalletVelo[1]=0;
						}
					}
					//angle:quadrant 1
					if(puckVelo[0] > 0 && puckVelo[1] >= 0)
					{
						if(puckCoord[0] < compMallet[0])
						{
							compMalletVelo[0] = -1;
						}
						if(puckCoord[1] < compMallet[1])
						{
							compMalletVelo[1] = -1;
						}
						if(puckCoord[0] > compMallet[0])
						{
							compMalletVelo[0] = 1;
						}
						if(puckCoord[1] > compMallet[1])
						{
							compMalletVelo[1] = 1;
						}
						if(distance(compMallet,puckCoord)<=(malletSize+puckSize))
						{
							if(compMallet[0] < vertHalfCoord)
							{
								compMalletVelo[0] = 1;
							}
							if(compMallet[1] < (3*vertSize/4)+(vertSize/8))
							{
								compMalletVelo[1] = 1;
							}
							if(compMallet[0] > vertHalfCoord)
							{
								compMalletVelo[0] = -1;
							}
							
							if(compMallet[1] >= (3*vertSize/4)+(vertSize/8))
							{
								compMalletVelo[1] = 0;
							}
							if(compMallet[0]==vertHalfCoord)
							{
								compMalletVelo[0]=0;
							}
						}	
					}
					//angle:quadrant 3
					if(puckVelo[0] < 0 && puckVelo[1] <= 0)
					{
						if(puckCoord[0] < compMallet[0])
						{
							compMalletVelo[0] = -1;
						}
						if(puckCoord[1] < compMallet[1])
						{
							compMalletVelo[1] = -1;
						}
						if(puckCoord[0] > compMallet[0])
						{
							compMalletVelo[0] = 1;
						}
						if(puckCoord[1] > compMallet[1])
						{
							compMalletVelo[1] = 1;
						}
						if(distance(compMallet,puckCoord)<=(malletSize+puckSize))
						{
							if(compMallet[0] < vertHalfCoord)
							{
								compMalletVelo[0] = 1;
							}
							if(compMallet[1] < (3*vertSize/4)+(vertSize/8))
							{
								compMalletVelo[1] = 1;
							}
							if(compMallet[0] > vertHalfCoord)
							{
								compMalletVelo[0] = -1;
							}
							
							if(compMallet[1] >= (3*vertSize/4)+(vertSize/8))
							{
								compMalletVelo[1] = 0;
							}
							if(compMallet[0]==vertHalfCoord)
							{
								compMalletVelo[0]=0;
							}
						}	
					}
					//angle:quadrant 4
					if(puckVelo[0] >= 0 && puckVelo[1] < 0)
					{
						if(puckCoord[0] < compMallet[0])
						{
							compMalletVelo[0] = -1;
						}
						if(puckCoord[1] < compMallet[1])
						{
							compMalletVelo[1] = -1;
						}
						if(puckCoord[0] > compMallet[0])
						{
							compMalletVelo[0] = 1;
						}
						if(puckCoord[1] > compMallet[1])
						{
							compMalletVelo[1] = 1;
						}
						if(distance(compMallet,puckCoord)<=(malletSize+puckSize))
						{
							if(compMallet[0] < vertHalfCoord)
							{
								compMalletVelo[0] = 1;
							}
							if(compMallet[1] < (3*vertSize/4)+(vertSize/8))
							{
								compMalletVelo[1] = 1;
							}
							if(compMallet[0] > vertHalfCoord)
							{
								compMalletVelo[0] = -1;
							}
							
							if(compMallet[1] >= (3*vertSize/4)+(vertSize/8))
							{
								compMalletVelo[1] = 0;
							}
							
							if(compMallet[0]==vertHalfCoord)
							{
								compMalletVelo[0]=0;
							}
						}	
					}
					
				}
				//Region 3
				if(puckCoord[0] >= vertHalfCoord && puckCoord[0] < (goal[1] + horiSize) / 2)
				{
					//angle:quadrant 1
					if(puckVelo[0] > 0 && puckVelo[1] >= 0)
					{
						if(compMallet[0] < (goal[0] + vertHalfCoord)/2)
						{
							compMalletVelo[0] = 1;
						}
						if(compMallet[1] < (3*vertSize/4)+(vertSize/8))
						{
							compMalletVelo[1] = 1;
						}
						if(compMallet[0] > (goal[0] + vertHalfCoord)/2)
						{
							compMalletVelo[0] = -1;
						}
						if(compMallet[1] > (3*vertSize/4)+(vertSize/8))
						{
							compMalletVelo[1] = -1;
						}
						if(compMallet[0]==(goal[1]+vertHalfCoord)/2)
						{
							compMalletVelo[0]=0;
						}
						if(compMallet[1]==(3*vertSize/4)+(vertSize/8))
						{
							compMalletVelo[1]=0;
						}
					}
					//angle:quadrant 2
					if(puckVelo[0] <= 0 && puckVelo[1] > 0)
					{
						if(puckCoord[0] < compMallet[0])
						{
							compMalletVelo[0] = -1;
						}
						if(puckCoord[1] < compMallet[1])
						{
							compMalletVelo[1] = -1;
						}
						if(puckCoord[0] > compMallet[0])
						{
							compMalletVelo[0] = 1;
						}
						if(puckCoord[1] > compMallet[1])
						{
							compMalletVelo[1] = 1;
						}
						if(distance(compMallet,puckCoord)<=(malletSize+puckSize))
						{
							if(compMallet[0] < vertHalfCoord)
							{
								compMalletVelo[0] = 1;
							}
							if(compMallet[1] < (3*vertSize/4)+(vertSize/8))
							{
								compMalletVelo[1] = 1;
							}
							if(compMallet[0] > vertHalfCoord)
							{
								compMalletVelo[0] = -1;
							}
							
							if(compMallet[1] >= (3*vertSize/4)+(vertSize/8))
							{
								compMalletVelo[1] = 0;
							}
							if(compMallet[0]==vertHalfCoord)
							{
								compMalletVelo[0]=0;
							}
						}	
					}
					//angle:quadrant 3
					if(puckVelo[0] < 0 && puckVelo[1] <= 0)
					{
						if(puckCoord[0] < compMallet[0])
						{
							compMalletVelo[0] = -1;
						}
						if(puckCoord[1] < compMallet[1])
						{
							compMalletVelo[1] = -1;
						}
						if(puckCoord[0] > compMallet[0])
						{
							compMalletVelo[0] = 1;
						}
						if(puckCoord[1] > compMallet[1])
						{
							compMalletVelo[1] = 1;
						}
						if(distance(compMallet,puckCoord)<=(malletSize+puckSize))
						{
							if(compMallet[0] < vertHalfCoord)
							{
								compMalletVelo[0] = 1;
							}
							if(compMallet[1] < (3*vertSize/4)+(vertSize/8))
							{
								compMalletVelo[1] = 1;
							}
							if(compMallet[0] > vertHalfCoord)
							{
								compMalletVelo[0] = -1;
							}
							
							if(compMallet[1] >= (3*vertSize/4)+(vertSize/8))
							{
								compMalletVelo[1] = 0;
							}
							
							if(compMallet[0]==vertHalfCoord)
							{
								compMalletVelo[0]=0;
							}
						}	
					}
					//angle:quadrant 4
					if(puckVelo[0] >= 0 && puckVelo[1] < 0)
					{
						if(puckCoord[0] < compMallet[0])
						{
							compMalletVelo[0] = -1;
						}
						if(puckCoord[1] < compMallet[1])
						{
							compMalletVelo[1] = -1;
						}
						if(puckCoord[0] > compMallet[0])
						{
							compMalletVelo[0] = 1;
						}
						if(puckCoord[1] > compMallet[1])
						{
							compMalletVelo[1] = 1;
						}
						if(distance(compMallet,puckCoord)<=(malletSize+puckSize))
						{
							if(compMallet[0] < vertHalfCoord)
							{
								compMalletVelo[0] = 1;
							}
							if(compMallet[1] < (3*vertSize/4)+(vertSize/8))
							{
								compMalletVelo[1] = 1;
							}
							if(compMallet[0] > vertHalfCoord)
							{
								compMalletVelo[0] = -1;
							}
							
							if(compMallet[1] >= (3*vertSize/4)+(vertSize/8))
							{
								compMalletVelo[1] = 0;
							}
							
							if(compMallet[0]==vertHalfCoord)
							{
								compMalletVelo[0]=0;
							}
						}	
					}
					
				}
				
				if(puckCoord[1] >= compMallet[1])
				{
					compMalletVelo[1] = 2;
				}
				if(puckCoord[1] >= initial)
				{
					compMalletVelo[0] = 0;
					compMalletVelo[1] = 1;
				}
			}
		}
	}
	if(compMallet[0] < malletSize)
	{
		compMalletVelo[0] = 1;
	}
	if(compMallet[0] > horiSize - malletSize)
	{
		compMalletVelo[0] = -1;
	}
	if(compMallet[1] < halfLineCoord)
	{
		compMalletVelo[1] = 1;
	}
	if(compMallet[1] > vertSize - malletSize)
	{
		compMalletVelo[1] = -1;
	}
	if(distance(compMallet,puckCoord)<=(malletSize+puckSize))
	{
		float temp = 2*compMalletVelo[0];
		compMalletVelo[0] -=temp;
		temp = 2*compMalletVelo[1];
		compMalletVelo[1] -=temp;
	}
	compMallet[0]+=compMalletVelo[0];
	compMallet[1]+=compMalletVelo[1];
	compMalletSpeed = (float)sqrt(pow(compMalletVelo[0],2)+pow(compMalletVelo[1],2));
	glutPostRedisplay();
}
void menu(int value)
{
	if(value == 0)
	{
		glutDestroyWindow(win);
		exit(0);
	}
	else
	{
		val=value;
	}
 
	// you would want to redraw now
	glutPostRedisplay();
}

void createMenu(void){
	//////////
	// MENU //
	//////////
 
	// Create a submenu, this has to be done first.
	animeringsmeny = glutCreateMenu(menu);
 
	// Add sub menu entry
	glutAddMenuEntry("På", 1);
	glutAddMenuEntry("Av", 2);
 
	springmeny = glutCreateMenu(menu);
 
	glutAddMenuEntry("Gå", 3);
	glutAddMenuEntry("Galopp", 4);
 
	// Create the menu, this menu becomes the current menu
	menyid = glutCreateMenu(menu);
 
	// Create an entry
	glutAddSubMenu("Animering", animeringsmeny);
	glutAddSubMenu("Löpstil", springmeny);
	// Create an entry
	glutAddMenuEntry("Avluta", 0);
 
	// Let the menu respond on the right mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
 
 
}

void display()
{
	int checkTime = glutGet(GLUT_ELAPSED_TIME);
	test++;
	float x1, y1;
	x1 = horiSize/2,y1=halfLineCoord;
    glClear (GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    drawSpots();
    drawCenter();
    drawGoalCircles();
	halfLine();
	drawgoals();
	int currTime = glutGet(GLUT_ELAPSED_TIME);
	int i = currTime / 100;
	elapsedTime = currTime - prevTime;
	glPushMatrix();
	if(level == 1)
	{
	   drawBitmapText("Level: 1", 35, 650, 0);
    }
    if(level == 2)
	{
	   drawBitmapText("Level: 2", 35, 650, 0);
    }
    if(level == 3)
	{
	   drawBitmapText("Level: 3", 35, 650, 0);
    }
	//drawText("AirHockey", 275, 340, 0.25, 0.25);
	plGoal();
	compGoal();
	
	//drawText(plScore, 487, 275, 0.25, 0.25);
	//drawText(compScore, 450, 400, 0.25, 0.25);
    glPopMatrix();
    
    drawPuck(puckCoord[0],puckCoord[1]);
	if(start==0)
    {
    	glutWarpPointer(vertHalfCoord, initial);
    	puckCoord[0]= horiSize / 2;
    	puckCoord[1]= vertSize / 2;
    	compMallet[0]= vertHalfCoord;
		compMallet[1]=(3*vertSize/4)+(vertSize/8);
		compMalletVelo[0]=0;
		compMalletVelo[1]=0;
		compMalletSpeed = 0;
    	playerMallet[0] = horiSize/2;
		playerMallet[1] = vertSize/4-(vertSize/8);
		touching = 0;	
		int k = 0;
		if(test==1)
	    pthread_create(&mouseThread, NULL, mouseFunction,(void*)&k);
	}
    drawMallet(compMallet[0], compMallet[1]);
    drawMallet(playerMallet[0], playerMallet[1]);
    if(paused==0)
    {
		puckTranslate();
	    compTranslate();
	}
	prevTime = currTime;
    glFlush();    
}

void handleKeypress(unsigned char key, //The key that was pressed                                                                                                           
                                        int x, int y) {    //The current mouse coordinates                                                                                  
        switch (key) {
                case 27: //Escape key                                                                                                                                       
                  exit(0); //Exit the program                                                                                                                               
                case 'p':
                  paused = 1 - paused;
                  break;
        }
}
int main(int argc, char* argv[])
{
	
    glutInit(&argc, argv);
    gotoxy(0, 0);
    initial= (3*vertSize/4)+(vertSize/8);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE| GLUT_DEPTH| GLUT_ALPHA);
    glutInitWindowSize(horiSize, vertSize);
    glutInitWindowPosition(300,0);
    glutCreateWindow("Air Hockey");
    init();
    //glutSetCursor(GLUT_CURSOR_FULL_CROSSHAIR);
    //glutFullScreen();
    createMenu();
    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeypress);
    
    glutMainLoop();
    return 0;
}
