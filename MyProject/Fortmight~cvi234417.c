#include "windows.h"
#include "toolbox.h"
#include <ansi_c.h>
#include <cvirte.h>
#include <userint.h>
#include "Fortmight.h"
#include "Math.h"
#include "bass.h"

#define MaxAsteroids 50
#define MaxProjectiles 10

#define CanvasSize 400 //Assuming Canvas is 450
#define M_PI 3.1415926535

static int panelHandle;
static int panelHandle2;
static int panelHandle3;
static int panelHandle4;

HSTREAM Scoresound;
HSTREAM Livelosssound; 
HSTREAM Projectilesound; 
HSTREAM Upgradesound; 


FILE *fp; 
 
 
//Todo:
// Spaceship Upgrade Weapon
// Smart Enemy
// Drift Spaceship
// Demo
// HELP





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
	int isEnemy;
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


typedef struct
{
	double x;
	double y;
	int isAlive;
} Enemy;


UserInputs userInputs;
Spaceship spaceship;

Projectile* projectiles [MaxProjectiles];
int projectileIndex;
int lastProjectileCreatedTime = 0;

Asteroid* asteroids [MaxAsteroids];

Enemy enemyShip;
int lastEnemyProjectileCreatedTime= 0;

int livesCount;
int score; 
int astroid_bitmap;
int hearts_bitmap;
int space_bitmap;











// **************** Keyboard ***************

//variables for keyboard monitoring
int Callback_Data;
int Posting_Handle;
int Keys_Down[256]= {0};

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

	livesCount = 5;
	score = 0;
	
	
	//seed rand number to time 0 and load files.
	srand(time (0));

	Connect_To_Windows_Events();

	//Initialize Spaceship
	spaceship.angle = 0;
	spaceship.x = 50;
	spaceship.y = 50;

	//Initalize Enemy
	enemyShip.isAlive = 0;
	enemyShip.x = 0;
	enemyShip.y = 0;

	projectileIndex = 0;
	for (i=0; i<MaxProjectiles; i++)
	{
		Projectile* p = malloc (sizeof(Projectile));
		p->x = 0;
		p->y = 0;
		p->angle= 0;
		p->isAlive = 0;
		p->isEnemy = 0;
		projectiles[i]=p;
	}



	for (i=0; i<MaxAsteroids; i++)
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
	
	
	GetBitmapFromFile ("images\\asteroid.png",&astroid_bitmap);
	GetBitmapFromFile ("images\\Hearts.png",&hearts_bitmap);
	GetBitmapFromFile ("images\\spacebackround.png",&space_bitmap);

	BASS_Init( -1,44100, 0,0,NULL);
    Scoresound = BASS_StreamCreateFile(FALSE,"Scoresound.mp3",0,0,0);
	Livelosssound = BASS_StreamCreateFile(FALSE,"Livelosssound.mp3",0,0,0); 
	Projectilesound = BASS_StreamCreateFile(FALSE,"Projectilesound.mp3",0,0,0); 
	Upgradesound = BASS_StreamCreateFile(FALSE,"Upgradesound.mp3",0,0,0);
	
}

void terminate()
{
	BASS_StreamFree(Scoresound);
	BASS_StreamFree(Livelosssound);
	BASS_StreamFree(Projectilesound);
	BASS_StreamFree(Upgradesound);
	DiscardBitmap (astroid_bitmap);
	DiscardBitmap (hearts_bitmap); 
	DiscardBitmap(space_bitmap);
}

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "Fortmight.uir", PANEL)) < 0)
		return -1;
	if ((panelHandle2 = LoadPanel (0, "Fortmight.uir", MENUPANEL)) < 0)
		return -1;
	if ((panelHandle3 = LoadPanel (0, "Fortmight.uir", LBPANEL)) < 0)
		return -1;
	if ((panelHandle4 = LoadPanel (0, "Fortmight.uir", ABOUTPANEL)) < 0)
		return -1;
	initilize();
	DisplayPanel (panelHandle2);
	RunUserInterface ();
	terminate();
	DiscardPanel (panelHandle);
	DiscardPanel (panelHandle2);
	DiscardPanel (panelHandle3);
	DiscardPanel (panelHandle4);
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



int CVICALLBACK Menufunc (int panel, int event, void *callbackData,
						  int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			HidePanel (panelHandle2);
			QuitUserInterface (0); 
			break;
	}
	return 0;
}


int CVICALLBACK leaderBordpanel (int panel, int event, void *callbackData,
								 int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			HidePanel (panelHandle3); 
			break;
	}
	return 0;
}


   int CVICALLBACK Aboutpanel (int panel, int event, void *callbackData,
							int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			 HidePanel (panelHandle4);
			break;
	}
	return 0;
}




//***********************************************











// **************** User Inputs ***************
void getUserInput()
{
	userInputs.isKeyUpPressed = (Keys_Down[40] - Keys_Down[38]) == -1;
	userInputs.isKeyLeftPressed = (Keys_Down[39] - Keys_Down[37]) == -1;
	userInputs.isKeyRightPressed = (Keys_Down[39] - Keys_Down[37]) == 1;
	userInputs.isKeySpacePressed = Keys_Down[32] == 1;
}
//***********************************************





// **************** Update Game ***************
double toDegress(double radians)
{
	return radians * (M_PI * 180.0);	
}

double toRadians(double degrees)
{
	return degrees * (M_PI / 180.0);
}

void updateSpaceship()
{

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


	if (userInputs.isKeyUpPressed)
	{
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


void updateProjectiles()
{

	int i;
	int currentTime;

	if (userInputs.isKeySpacePressed)
	{
		currentTime = clock(); //number of milliseconds since the program started
		if (currentTime > lastProjectileCreatedTime+200)
		{
			lastProjectileCreatedTime = currentTime;
			projectiles[projectileIndex]->x = spaceship.x;
			projectiles[projectileIndex]->y = spaceship.y;
			projectiles[projectileIndex]->angle = spaceship.angle;
			projectiles[projectileIndex]->isAlive =1;
			projectiles[projectileIndex]->isEnemy = 0;
			projectileIndex = (projectileIndex + 1) % MaxProjectiles;
			BASS_ChannelPlay(Projectilesound,TRUE);

		}
	}


	for (i=0; i<MaxProjectiles; i++)
	{
		projectiles[i]->x = projectiles[i]->x + (15 * cos(toRadians(projectiles[i]->angle)));
		projectiles[i]->y = projectiles[i]->y + (15 * sin(toRadians(projectiles[i]->angle)));

		if (projectiles[i]->x > CanvasSize || projectiles[i]->x < 0)
			projectiles[i]->isAlive = 0;

		if (projectiles[i]->y > CanvasSize || projectiles[i]->y < 0)
			projectiles[i]->isAlive = 0;
	}

}



void generateAsteroid()
{

	int i;
	double randomNumber;
	int creationSide;
	int nextFreeSpace;
	int freeSpaceCount;
	
	
	freeSpaceCount = 0;
	for (i=0; i<MaxAsteroids; i++)
	{
		if (!asteroids[i]->isAlive)
		{
			freeSpaceCount++;
		}
	}
	
	if (freeSpaceCount < MaxAsteroids/2)
		return;
	
	//Find next free space
	nextFreeSpace=-1;
	for (i=0; i<MaxAsteroids; i++)
	{
		if (!asteroids[i]->isAlive)
		{
			nextFreeSpace = i;
		}
	}

	if (nextFreeSpace == -1)
		return;


	
	//Throttle
	randomNumber = ((double)rand()/RAND_MAX);
	if (randomNumber < 0.92)
		return;


	creationSide = rand()%4;

	if (creationSide == 0) //Top
	{
		asteroids[nextFreeSpace]->x = rand()%CanvasSize+50;
		asteroids[nextFreeSpace]->y = 0;
	}

	if (creationSide == 1) //Right
	{
		asteroids[nextFreeSpace]->x = CanvasSize;
		asteroids[nextFreeSpace]->y = rand()%CanvasSize+50;
	}

	if (creationSide == 2) //Bottom
	{
		asteroids[nextFreeSpace]->x = rand()%CanvasSize+50;
		asteroids[nextFreeSpace]->y = CanvasSize;
	}

	if (creationSide == 3) //Left
	{
		asteroids[nextFreeSpace]->x = 0;
		asteroids[nextFreeSpace]->y = rand()%CanvasSize+50;
	}


	asteroids[nextFreeSpace]->angle = rand()%360; //0-359
	asteroids[nextFreeSpace]->size = (rand()%3)+1;  //1-3
	asteroids[nextFreeSpace]->speed = (rand()%5) + 1; //1-5
	asteroids[nextFreeSpace]->isAlive =1;

}

void updateAsteriods()
{
	int i;

	//Generate New Asteroid
	generateAsteroid();

	
	//Update Existings
	for (i=0; i<MaxAsteroids; i++)
	{
		if (asteroids[i]->isAlive)
		{
			asteroids[i]->x = asteroids[i]->x + (asteroids[i]->speed * cos(toRadians(asteroids[i]->angle)));
			asteroids[i]->y = asteroids[i]->y + (asteroids[i]->speed * sin(toRadians(asteroids[i]->angle)));

			if (asteroids[i]->x > CanvasSize || asteroids[i]->x < 0)
				asteroids[i]->isAlive = 0;

			if (asteroids[i]->y > CanvasSize || asteroids[i]->y < 0)
				asteroids[i]->isAlive = 0;
		}
	}
}



int isAsteroidHitWithSpaceship(Asteroid* asteroid)
{
	int spaceshipHitboxSize = 20;
	if (asteroid->x >= spaceship.x-spaceshipHitboxSize && asteroid->x < spaceship.x+spaceshipHitboxSize && 
			asteroid->y >= spaceship.y-spaceshipHitboxSize && asteroid->y < spaceship.y+spaceshipHitboxSize)
	{
		return 1;
	}

	return 0;
}

int isAsteroidHitProjectile(Asteroid* asteroid, Projectile* projectile)
{
	int hitboxSize = asteroid->size*15;
	if (asteroid->x >= projectile->x-hitboxSize && asteroid->x < projectile->x+hitboxSize &&
			asteroid->y >= projectile->y-hitboxSize && asteroid->y < projectile->y+hitboxSize)
	{
		return 1;
	}
	return 0;
}



void splitAsteroidToAngle(Asteroid* asteroidToSplit, int angle)
{
	int nextFreeSpace;
	int i;
	
	//Find next free space
	nextFreeSpace=-1;
	for (i=0; i<MaxAsteroids; i++)
	{
		if (!asteroids[i]->isAlive)
		{
			nextFreeSpace = i;
		}
	}

	if (nextFreeSpace == -1)
		return;
	
	asteroids[nextFreeSpace]->isAlive = 1;
	asteroids[nextFreeSpace]->x = asteroidToSplit->x; 
	asteroids[nextFreeSpace]->y = asteroidToSplit->y;
	asteroids[nextFreeSpace]->size = asteroidToSplit->size - 1;
	asteroids[nextFreeSpace]->speed = asteroidToSplit->speed;
	asteroids[nextFreeSpace]->angle = angle;
	
}

void splitAsteroid(Asteroid* asteroidToSplit) 
{
	splitAsteroidToAngle(asteroidToSplit, (asteroidToSplit->angle+45)%360);
	splitAsteroidToAngle(asteroidToSplit, (asteroidToSplit->angle-45)%360);
}


int isProjectileHitSpaceship(Projectile* projectile)
{
	int spaceshipHitboxSize = 20;
	if (projectile->x >= spaceship.x-spaceshipHitboxSize && projectile->x < spaceship.x+spaceshipHitboxSize && 
			projectile->y >= spaceship.y-spaceshipHitboxSize && projectile->y < spaceship.y+spaceshipHitboxSize)
	{
		return 1;
	}

	return 0;	
}


int isProjectileHitEnemy(Projectile* projectile)
{
	int hitboxSize = 50;
	if (projectile->x >= enemyShip.x-hitboxSize && projectile->x < enemyShip.x+hitboxSize && 
			projectile->y >= enemyShip.y-hitboxSize && projectile->y < enemyShip.y+hitboxSize)
	{
		return 1;
	}

	return 0;	
}


void updateHits()
{

	int i;
	int j;

	//Asteriod In Spaceship
	for (i=0; i<MaxAsteroids; i++)
	{
		if (asteroids[i]->isAlive)
		{
			if (isAsteroidHitWithSpaceship(asteroids[i]) == 1) 
			{
				asteroids[i]->isAlive = 0;//Dead
				livesCount--;
				BASS_ChannelPlay(Livelosssound,TRUE);

			}
		}
	}


	//Projectile In Asteroid
	for (i=0; i<MaxAsteroids; i++)
	{
		if (asteroids[i]->isAlive)
		{
			for (j=0; j<MaxProjectiles; j++)
			{
				if (projectiles[j]->isAlive)
				{
					if (isAsteroidHitProjectile(asteroids[i], projectiles[j]) == 1)
					{
						if (asteroids[i]->size == 1)
						{
							asteroids[i]->isAlive = 0; //Dead
							projectiles[j]->isAlive = 0; //Dead
							score = score +20;
							BASS_ChannelPlay(Scoresound,TRUE);
						}
						else
						{
							splitAsteroid(asteroids[i]);
							projectiles[j]->isAlive = 0; //Dead  
							asteroids[i]->isAlive = 0; //Dead
							score = score +10; 
							BASS_ChannelPlay(Scoresound,TRUE); 
						}
					}
					
				}
			}
		}
	}

	//Projectile In Ship
	for (j=0; j<MaxProjectiles; j++)
	{
		if (projectiles[j]->isAlive && projectiles[j]->isEnemy == 1)
		{
			//If Projectile Hit Spaceship
			if (isProjectileHitSpaceship(projectiles[j]) == 1)
			{
				projectiles[j]->isAlive = 0; //Dead
				livesCount--; //Reduce Score
				BASS_ChannelPlay(Livelosssound,TRUE);
			}
		}
	}

	//Projectile in Enemy
	for (j=0; j<MaxProjectiles; j++)
	{
		if (projectiles[j]->isAlive && projectiles[j]->isEnemy == 0)
		{
			//If Projectile Hit Enemy
			if (isProjectileHitEnemy(projectiles[j]) == 1)
			{
				projectiles[j]->isAlive = 0; //Dead
				enemyShip.isAlive = 0;
				score = score +50;
				BASS_ChannelPlay(Scoresound,TRUE);
			}
		}
	}


}



void updateEnemy()
{
	double randomNumber;
	int currentTime;

	
	if (enemyShip.isAlive)
	{
		enemyShip.x = enemyShip.x + 2;
		
		if (enemyShip.x > CanvasSize)
		{
			enemyShip.isAlive = 0; 	
		}
		else
		{
			
			currentTime = clock();
			if (currentTime > lastEnemyProjectileCreatedTime+500)
			{
				lastEnemyProjectileCreatedTime = currentTime;
				projectiles[projectileIndex]->x = enemyShip.x;
				projectiles[projectileIndex]->y = enemyShip.y;
				projectiles[projectileIndex]->isAlive =1;
				projectiles[projectileIndex]->isEnemy =1;
				
				//projectiles[projectileIndex]->angle = 180 - toDegress(atan((spaceship.y - enemyShip.y)/(spaceship.x-enemyShip.x )));
				//deltaY = (spaceship.y - enemyShip.y);
    			//deltaX = (spaceship.x - enemyShip.x);
    			//tRes = toDegress(atan2(deltaY,deltaX)); 
				//projectiles[projectileIndex]->angle = 180-tRes;
				
				projectiles[projectileIndex]->angle = rand()%360;
				projectileIndex = (projectileIndex + 1) % MaxProjectiles;
			}
		}
	}
	else
	{
		//Throttle
		randomNumber = ((double)rand()/RAND_MAX);
		if (randomNumber < 0.99)
			return;
	
		enemyShip.isAlive = 1;
		enemyShip.x = 0;
		enemyShip.y = rand()%CanvasSize;
	}
	

	
	
}
  
void updateLeaderBord()
  { 
	  char nickname[20];
	  char recordname[20];
	  int i;
	  int recordscore;
	  int count=10;
	  
	  
	  
	  GetCtrlVal (panelHandle2, MENUPANEL_NAMESTRING,nickname);
	  GetTableCellVal (panelHandle3, LBPANEL_LEADERBORDTABLE, MakePoint(2,10), &recordscore);
	  if(score>recordscore)
	  {
		  SetTableCellVal (panelHandle3, LBPANEL_LEADERBORDTABLE, MakePoint (1,10), nickname);
		  SetTableCellVal (panelHandle3, LBPANEL_LEADERBORDTABLE, MakePoint (2,10), score);
	  }
	  for(i=1;i<10;i++)
	  {
		 GetTableCellVal (panelHandle3, LBPANEL_LEADERBORDTABLE, MakePoint(2,10-i), &recordscore);
		 GetTableCellVal (panelHandle3, LBPANEL_LEADERBORDTABLE, MakePoint(1,10-i), recordname);
		 if(recordscore<score)
		 {
		  SetTableCellVal (panelHandle3, LBPANEL_LEADERBORDTABLE, MakePoint (1,10-i), nickname);
		  SetTableCellVal (panelHandle3, LBPANEL_LEADERBORDTABLE, MakePoint (2,10-i), score);
		  SetTableCellVal (panelHandle3, LBPANEL_LEADERBORDTABLE, MakePoint (1,count), recordname);
		  SetTableCellVal (panelHandle3, LBPANEL_LEADERBORDTABLE, MakePoint (2,count), recordscore);
		  count--;
		 } 
		  
		  
	  }
	  
	  
	  
	   	SavePanelState (panelHandle3, "leaderbordpaneldata.txt", 0); 

	  
  }

void gameOver()
{	 
	updateLeaderBord(); 
	SetCtrlAttribute (panelHandle, PANEL_RETURNTOMENU, ATTR_VISIBLE, 1);
	SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 0);

	
	
}

void updateGameState()
{
	updateSpaceship();

	updateEnemy();
	
	updateProjectiles();

	updateAsteriods();

	updateHits();
}



//***********************************************








// **************** Drawing ***************

void printDebugInformation()
{

	char KeysTextBoxStr[255];
	char SpaceshipTextBoxStr[255];

	//Debug - Print User Inputs State
	sprintf(KeysTextBoxStr, "%d %d %d %d", userInputs.isKeyUpPressed, userInputs.isKeyLeftPressed, userInputs.isKeyRightPressed, userInputs.isKeySpacePressed);
	SetCtrlVal (panelHandle, PANEL_KeysTextBox, KeysTextBoxStr);


	//Debug - Print Spaceship State
	sprintf(SpaceshipTextBoxStr, "%f %f %d", spaceship.x, spaceship.y, spaceship.angle);
	SetCtrlVal (panelHandle, PANEL_SpaceshipTextBox, SpaceshipTextBoxStr);
	
}

void drawGameStatistics(){
	SetCtrlVal (panelHandle, PANEL_LivesCountControl, livesCount);
	CanvasDrawBitmap (panelHandle, PANEL_HEARTSCANVAS, hearts_bitmap, MakeRect ((5-livesCount)*42,0,46,225),VAL_ENTIRE_OBJECT);
	if(livesCount==0)
	{
		CanvasDrawText (panelHandle, PANEL_HEARTSCANVAS, "Game Over", VAL_APP_META_FONT, MakeRect (0,0,30,200), VAL_CENTER);
	}
	SetCtrlVal (panelHandle, PANEL_ScoreControl, score);
}

void drawAfterburn()
{
	Point enginePoint;
	Point afterburnerPoint;
		 //set PenColor and Dimentions
		SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_COLOR, VAL_YELLOW);
		SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_WIDTH, 6);
		
		//Draw afterburner
		enginePoint = MakePoint ((int)spaceship.x + (-15 * cos(toRadians(spaceship.angle))),(int)spaceship.y + (-15 * sin(toRadians(spaceship.angle))));
		afterburnerPoint = MakePoint ((int)spaceship.x + (-20 * cos(toRadians(spaceship.angle))),(int)spaceship.y + (-20 * sin(toRadians(spaceship.angle)))); 
		CanvasDrawLine (panelHandle, PANEL_CANVAS,enginePoint,afterburnerPoint);
	
}

void drawSpaceship()
{

	Point tipPoint;
	

	Point one_end;
	Point two_end;
	
	Point one_mid;
	Point two_mid;
	




	if (userInputs.isKeyUpPressed)
	{
		drawAfterburn();
	}
	
	SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_COLOR, MakeColor(192,192,192)); //silvercolor
	SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_WIDTH, 4);
	
	//draw spaceshiptip
	tipPoint = MakePoint((int)spaceship.x,(int)spaceship.y);

	//draw spaceship wings
	one_end = MakePoint((int)spaceship.x + (20 * cos(toRadians(spaceship.angle-140))),(int)spaceship.y + (20 * sin(toRadians(spaceship.angle-140))));
	CanvasDrawLine(panelHandle, PANEL_CANVAS,tipPoint,one_end);

	two_end = MakePoint((int)spaceship.x + (20 * cos(toRadians(spaceship.angle+140))),(int)spaceship.y + (20 * sin(toRadians(spaceship.angle+140))));
	CanvasDrawLine(panelHandle, PANEL_CANVAS,tipPoint,two_end);
	
	//draw spaceship bottom line
	one_mid = MakePoint((int)spaceship.x + (15 * cos(toRadians(spaceship.angle-140))),(int)spaceship.y + (15 * sin(toRadians(spaceship.angle-140))));
	
    two_mid = MakePoint((int)spaceship.x + (15 * cos(toRadians(spaceship.angle+140))),(int)spaceship.y + (15 * sin(toRadians(spaceship.angle+140))));
	CanvasDrawLine(panelHandle, PANEL_CANVAS,one_mid,two_mid); 

	
}

 


void drawProjectiles()
{
	SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_WIDTH, 5);

	int i;
	for (i=0; i<MaxProjectiles; i++)
	{
		if (projectiles[i]->isAlive)
		{
			if (projectiles[i]->isEnemy)
			{
				SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_COLOR, VAL_RED);
			}
			else
			{
				SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_COLOR, MakeColor(192,192,192));//silver color
			}
			
			CanvasDrawPoint (panelHandle, PANEL_CANVAS, MakePoint((int)projectiles[i]->x,(int)projectiles[i]->y));
		}
	}
}


void drawAsteroid(Asteroid* asteroid)
{

	
	SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_COLOR, VAL_BLUE);
	SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_WIDTH, 1);
	
	//draw asteroids as rectengels
	//CanvasDrawRect (panelHandle, PANEL_CANVAS, MakeRect((int)asteroid->y, (int)asteroid->x, asteroid->size * 20, asteroid->size * 20), VAL_DRAW_FRAME);
	
	//Bitmap asteroid Drawing
	
	CanvasDrawBitmap (panelHandle, PANEL_CANVAS, astroid_bitmap, MakeRect(256,256,256,256), MakeRect((int)asteroid->y, (int)asteroid->x, asteroid->size * 20, asteroid->size * 20));
	
 }

void drawAsteroids()
{
	int i;
	for (i=0; i<MaxAsteroids; i++)
	{
		if (asteroids[i]->isAlive)
		{
			drawAsteroid(asteroids[i]);
		}
	}
}


void drawEnemy()
{
	if (enemyShip.isAlive)
	{
		SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_COLOR, VAL_RED);
		SetCtrlAttribute (panelHandle, PANEL_CANVAS, ATTR_PEN_WIDTH, 2);
		//CanvasDrawOval (panelHandle, PANEL_CANVAS, MakeRect(enemyShip.y, enemyShip.x, 50, 50), VAL_DRAW_FRAME);
		CanvasDrawArc (panelHandle, PANEL_CANVAS, MakeRect(enemyShip.y, enemyShip.x, 30, 30), 0, 1800, VAL_DRAW_FRAME);
		CanvasDrawLine (panelHandle, PANEL_CANVAS, MakePoint(enemyShip.x-10, enemyShip.y+15), MakePoint(enemyShip.x+40, enemyShip.y+15));
	}
}

void draw ()
{
	printDebugInformation();

	drawGameStatistics();

	//Open Canvas
	CanvasStartBatchDraw (panelHandle, PANEL_CANVAS);
	//CanvasClear (panelHandle, PANEL_CANVAS, VAL_ENTIRE_OBJECT);
	CanvasDrawBitmap (panelHandle, PANEL_CANVAS, space_bitmap, VAL_ENTIRE_OBJECT, VAL_ENTIRE_OBJECT);

	//Draw On Canvas
	drawSpaceship();
	drawProjectiles();
	drawAsteroids();
	drawEnemy();
	
	//Close Canvas
	CanvasEndBatchDraw (panelHandle, PANEL_CANVAS);
}

//***********************************************








// **************** Game Loop & Timer ***************
void gameLoop()
{
	if (livesCount == 0)
	{
		gameOver();
	}	
	
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



//******************MAIN MENU********************

int CVICALLBACK StartGame (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			DisplayPanel (panelHandle);
			HidePanel (panelHandle2);
			SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED,1);
			
			
			break;
	}
	return 0;
}

int CVICALLBACK OpenLeaderBord (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			 
			 RecallPanelState (panelHandle3, "leaderbordpaneldata.txt", 0);
			 DisplayPanel (panelHandle3);
			 
			break;
	}
	return 0;
}

int CVICALLBACK OpenAboutMe(int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			DisplayPanel (panelHandle4);
			break;
	}
	return 0;
}

int CVICALLBACK ReturnToMenu (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

				 DisplayPanel (panelHandle2);
				 HidePanel(panelHandle);
				 SetCtrlAttribute (panelHandle, PANEL_RETURNTOMENU, ATTR_VISIBLE, 0);
				 score=0;
				 livesCount=5;
			
			
			break;
	}
	return 0;
}

void CVICALLBACK OpenTutorialDoc (int menuBar, int menuItem, void *callbackData,
								  int panel)
{
	//opendoc
}

void CVICALLBACK OpenDemoVideo (int menuBar, int menuItem, void *callbackData,
								int panel)
{
	//opendemovideo
}


