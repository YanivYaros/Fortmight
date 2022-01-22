#include "windows.h"
#include "toolbox.h"
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "Fortmight.h"
#include "Math.h"  

#define MaxAsteroids 25
#define MaxProjectiles 10 

#define CanvasSize 400 //Assuming Canvas is 450
#define M_PI 3.1415926535

static int panelHandle;





typedef struct
{
	int isKeyUpPressed;
	int isKeyLeftPressed;
	int isKeyRightPressed;
	int isKeySpacePressed;
} UserInputs;


typedef struct
{
	double x;
	double y;
	int angle;
	int isAlive;
} Projectile;


typedef struct
{
	double x;
	double y;
	int angle;
} Spaceship;


typedef struct
{
	double x;
	double y;
	int size;
	int isAlive;
	int angle;
	int speed;
} Asteroid;


UserInputs userInputs;
Spaceship spaceship;

Projectile* projectiles [MaxProjectiles];
int projectileIndex;

Asteroid* asteroids [MaxAsteroids];












// **************** Keyboard ***************

//variables for keyboard monitoring
int Callback_Data;	
int Posting_Handle; 
int Keys_Down[256]={0}; 

//Receives information from windows regarding key presses
int CVICALLBACK Key_DownFunc(int panel, int message, unsigned int* wParam, unsigned int* lParam, void* callbackData)
{
	unsigned int Key = *wParam;
	//printf("%d\n", Key);
	Keys_Down[Key] = 1;
	return 0;
}

//Receives information from windows regarding key releases
int CVICALLBACK Key_UpFunc(int panel, int message, unsigned int* wParam, unsigned int* lParam, void* callbackData)
{
	unsigned int Key = *wParam;
	Keys_Down[Key] = 0;
	return 0;
}

//asking windows to send keyboard press and release events to our software 
//Specifing 'Key_DownFunc' and 'Key_UpFunc' as the functions to call
void Connect_To_Windows_Events(void)
{
	InstallWinMsgCallback(panelHandle,WM_KEYDOWN,Key_DownFunc,VAL_MODE_IN_QUEUE,&Callback_Data,&Posting_Handle);
	InstallWinMsgCallback(panelHandle,WM_KEYUP,Key_UpFunc,VAL_MODE_IN_QUEUE,&Callback_Data,&Posting_Handle);
}

//***********************************************







// **************** Init & Main *************** 
void initilize ()
{
	int i;
	
	//seed rand number to time 0 and load files.
	srand(time (0));  
	
	Connect_To_Windows_Events(); 
	
	//Initialize Spaceship
	spaceship.angle = 0;
	spaceship.x = 50;
	spaceship.y = 50;
	
	
	
	projectileIndex = 0;
	for (i=0;i<MaxProjectiles;i++)
	{
		Projectile* p = malloc (sizeof(Projectile));
		p->x = 0;
		p->y = 0;
		p->angle= 0;
		p->isAlive = 0;
		projectiles[i]=p;
	}
	
	
	
	for (i=0;i<MaxAsteroids;i++)
	{
		Asteroid* p = malloc (sizeof(Asteroid));
		p->x = 0;
		p->y = 0;
		p->angle= 0;
		p->isAlive = 0;
		p->size = 0;
		p->speed = 0;
		asteroids[i]=p;
	}
	
}

void terminate()
{
}

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "Fortmight.uir", PANEL)) < 0)
		return -1;
	initilize();
	DisplayPanel (panelHandle);
	RunUserInterface ();
	terminate();
	DiscardPanel (panelHandle);
	return 0;
}

int CVICALLBACK exitfunc (int panel, int event, void *callbackData,
						  int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface (0);
			break;
	}
	return 0;
}

//***********************************************   











// **************** User Inputs *************** 
void getUserInput(){
	userInputs.isKeyUpPressed = (Keys_Down[40] - Keys_Down[38]) == -1;
	userInputs.isKeyLeftPressed = (Keys_Down[39] - Keys_Down[37]) == -1;
	userInputs.isKeyRightPressed = (Keys_Down[39] - Keys_Down[37]) == 1;
	userInputs.isKeySpacePressed = Keys_Down[32] == 1;
}
//***********************************************  





// **************** Update Game *************** 
double toRadians(double degrees) {
    return degrees * (M_PI / 180.0);
}

void updateSpaceship(){
	
	if (userInputs.isKeyLeftPressed)
	{
		if (spaceship.angle == 0)
			spaceship.angle = 360;
		
		spaceship.angle = (spaceship.angle - 10) % 360;
	}
	
	if (userInputs.isKeyRightPressed)
	{
		spaceship.angle = (spaceship.angle + 10) % 360;
	}
	
	
	if (userInputs.isKeyUpPressed){
		spaceship.x = spaceship.x + (10 * cos(toRadians(spaceship.angle)));
		spaceship.y = spaceship.y + (10 * sin(toRadians(spaceship.angle)));
	}
	
	
	if (spaceship.x < 0)
		spaceship.x = CanvasSize;
	
	if (spaceship.x > CanvasSize)
		spaceship.x = 0;
	
	if (spaceship.y < 0)
		spaceship.y = CanvasSize;
	
	if (spaceship.y > CanvasSize)
		spaceship.y = 0;
}


void updateProjectiles(){
	
	int i;
	
	if (userInputs.isKeySpacePressed){
		//Todo: Not Allow so many shots so quickly
		projectiles[projectileIndex]->x = spaceship.x;
		projectiles[projectileIndex]->y = spaceship.y;
		projectiles[projectileIndex]->angle = spaceship.angle;
		projectiles[projectileIndex]->isAlive =1;
		projectileIndex = (projectileIndex + 1) % MaxProjectiles;
	}
	
	
	for (i=0;i<MaxProjectiles;i++){
		projectiles[i]->x = projectiles[i]->x + (10 * cos(toRadians(projectiles[i]->angle)));
		projectiles[i]->y = projectiles[i]->y + (10 * sin(toRadians(projectiles[i]->angle)));
		
		if (projectiles[i]->x > CanvasSize || projectiles[i]->x < 0)
			projectiles[i]->isAlive = 0;
		
		if (projectiles[i]->y > CanvasSize || projectiles[i]->y < 0)
			projectiles[i]->isAlive = 0;
	}
	
}


void updateAsteriods(){
	
	int i;
	double randomNumber;
	
	int nextFreeSpace=-1;
	for (i=0;i<MaxAsteroids;i++){
		if (!asteroids[i]->isAlive){
			nextFreeSpace = i;
		}
	}
	
	
	randomNumber = ((double)rand()/RAND_MAX);
	
	
	
	if (randomNumber > 0.92 && nextFreeSpace != -1){
		
		randomNumber = rand()%4;
		
		if (randomNumber == 0){
			asteroids[nextFreeSpace]->x = rand()%CanvasSize+50;
			asteroids[nextFreeSpace]->y = 0;
		}
		
		if (randomNumber == 1)
		{
			asteroids[nextFreeSpace]->x = CanvasSize;
			asteroids[nextFreeSpace]->y = rand()%CanvasSize+50;
		}
		
		if (randomNumber == 2)
		{
			asteroids[nextFreeSpace]->x = rand()%CanvasSize+50;
			asteroids[nextFreeSpace]->y = CanvasSize;
		}
		
		if (randomNumber == 3)
		{
			asteroids[nextFreeSpace]->x = 0;
			asteroids[nextFreeSpace]->y = rand()%CanvasSize+50;
		}
		
		
		asteroids[nextFreeSpace]->angle = rand()%360;
		asteroids[nextFreeSpace]->size = (rand()%3)+1;
		asteroids[nextFreeSpace]->speed = (rand()%5) + 1;
		asteroids[nextFreeSpace]->isAlive =1;
	}
	
	//Update Existings
	for (i=0;i<MaxAsteroids;i++){
		if (asteroids[i]->isAlive){
			
			asteroids[i]->x = asteroids[i]->x + (asteroids[i]->speed * cos(toRadians(asteroids[i]->angle)));
			asteroids[i]->y = asteroids[i]->y + (asteroids[i]->speed * sin(toRadians(asteroids[i]->angle)));
		
			if (asteroids[i]->x > CanvasSize || asteroids[i]->x < 0)
				asteroids[i]->isAlive = 0;
		
			if (asteroids[i]->y > CanvasSize || asteroids[i]->y < 0)
				asteroids[i]->isAlive = 0;
		}
	}
}

void updateHits(){
	
	
	int i;
	int j;
	
	//Asteriod In Spaceship
	SetCtrlVal (panelHandle, PANEL_SpaceshipHit, 0);
	for (i=0;i<MaxAsteroids;i++){
		if (asteroids[i]->isAlive){
			
			if (	asteroids[i]->x >= spaceship.x-20 && 
					asteroids[i]->x < spaceship.x+20 &&
					asteroids[i]->y >= spaceship.y-20 &&
					asteroids[i]->y < spaceship.y+20
			   )
			{
				//Todo:Game Over Reduce Score
				SetCtrlVal (panelHandle, PANEL_SpaceshipHit, 1);
			}
			
		}
	}
	
	
	//Projectile In Asteroid
	for (i=0;i<MaxAsteroids;i++)
	{
		if (asteroids[i]->isAlive)
		{
			for (j=0;j<MaxProjectiles;j++)
			{
				if (projectiles[j]->isAlive)
				{
					if (asteroids[i]->x >= projectiles[j]->x-20 && 
						asteroids[i]->x < projectiles[j]->x+20 &&
						asteroids[i]->y >= projectiles[j]->y-20 &&
						asteroids[i]->y < projectiles[j]->y+20)
						{
							if (asteroids[i]->size == 1){
								asteroids[i]->isAlive = 0;
							}
							
							if (asteroids[i]->size == 2){
								//Todo: Split to 2 Size Of 1
							}
							
							if (asteroids[i]->size == 3){
								//Todo: Split to 2 Size Of 2
							}
						}
				}
			}	
		}
	}
	
	
	
	
	
}

void updateGameState()
{
	updateSpaceship();
	
	updateProjectiles();
	
	updateAsteriods();
	
	updateHits();
}

//*********************************************** 








// **************** Drawing *************** 

void printDebugInformation(){
	
	char KeysTextBoxStr[255];
	char SpaceshipTextBoxStr[255];
	
	//Debug - Print User Inputs State 
   	sprintf(KeysTextBoxStr, "%d %d %d %d", userInputs.isKeyUpPressed, userInputs.isKeyLeftPressed, userInputs.isKeyRightPressed, userInputs.isKeySpacePressed);
	SetCtrlVal (panelHandle, PANEL_KeysTextBox, KeysTextBoxStr);
	
	
	//Debug - Print Spaceship State
   	sprintf(SpaceshipTextBoxStr, "%f %f %d", spaceship.x, spaceship.y, spaceship.angle);
	SetCtrlVal (panelHandle, PANEL_SpaceshipTextBox, SpaceshipTextBoxStr);
}

void drawSpaceship(){
	
	Point tipPoint;
	
	Point one_end;
	Point two_end;
	
	
	SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_COLOR, VAL_BLACK);
	SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_WIDTH, 4);
	
	
	
	if (userInputs.isKeyUpPressed){
		SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_COLOR, VAL_YELLOW);
		SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_WIDTH, 4);
	}
	
	
	tipPoint = MakePoint((int)spaceship.x,(int)spaceship.y);
	
	one_end = MakePoint((int)spaceship.x + (20 * cos(toRadians(spaceship.angle-140))),(int)spaceship.y + (20 * sin(toRadians(spaceship.angle-140))));
	CanvasDrawLine(panelHandle, PANEL_CANVAS,tipPoint,one_end);
	
	two_end = MakePoint((int)spaceship.x + (20 * cos(toRadians(spaceship.angle+140))),(int)spaceship.y + (20 * sin(toRadians(spaceship.angle+140))));
	CanvasDrawLine(panelHandle, PANEL_CANVAS,tipPoint,two_end);
}


void drawProjectiles(){
	
	SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_COLOR, VAL_RED);
	SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_WIDTH, 4);
	
	int i;
	for (i=0;i<MaxProjectiles;i++){
		if (projectiles[i]->isAlive){
			CanvasDrawPoint (panelHandle, PANEL_CANVAS, MakePoint((int)projectiles[i]->x,(int)projectiles[i]->y));
		}
	}
}


void drawAsteroids(){
	
	SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_COLOR, VAL_BLUE);
	
	
	int i;
	for (i=0;i<MaxAsteroids;i++){
		if (asteroids[i]->isAlive){
			SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_WIDTH, asteroids[i]->size*10);
			CanvasDrawPoint (panelHandle, PANEL_CANVAS, MakePoint((int)asteroids[i]->x,(int)asteroids[i]->y));
		}
	}
}


void draw ()
{
	printDebugInformation();
	
	
	//Open Canvas
	CanvasStartBatchDraw (panelHandle, PANEL_CANVAS);
	CanvasClear (panelHandle, PANEL_CANVAS, VAL_ENTIRE_OBJECT);
	
	//Draw On Canvas
	drawSpaceship();
	drawProjectiles();
	drawAsteroids();
	
	//Close Canvas
	CanvasEndBatchDraw (panelHandle, PANEL_CANVAS);
}

//*********************************************** 








// **************** Game Loop & Timer *************** 
void gameLoop()
{
	getUserInput();
	updateGameState();
	draw();	
}

int CVICALLBACK Timerfunc (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:

			gameLoop();
			break;
	}
	return 0;
}

//*********************************************** 
