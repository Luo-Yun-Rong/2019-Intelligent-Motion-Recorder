//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include "LTexture.h"
///////use for socket
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <sys/time.h>

using namespace std;

//extern TTF_Font *gFont;
//extern SDL_Renderer* gRenderer;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//use for socket
void socket_client();
int recvtimeout(int s, char *buf, int len, int timeout);

//check if mouse in rectangle
bool checkmousepos(SDL_Rect &Rect);

//Loads individual image
SDL_Surface* loadSurface( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
	
//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//Begin image
SDL_Surface* gSurface = NULL;

//Start button image
SDL_Surface* gbuttonSurface = NULL;

//Running image
SDL_Surface* grunningSurface = NULL;
SDL_Surface* grunningSurface2 = NULL;

//sitting image
SDL_Surface* gsittingSurface = NULL;
SDL_Surface* gsittingSurface2 = NULL;

//lying image
SDL_Surface* glyingSurface = NULL;
SDL_Surface* glyingSurface2 = NULL;

//stand image
SDL_Surface* gstandSurface = NULL;
SDL_Surface* gstandSurface2 = NULL;

//walk image
SDL_Surface* gwalkSurface = NULL;
SDL_Surface* gwalkSurface2 = NULL;

SDL_Surface* gbikingSurface = NULL;
SDL_Surface* gbikingSurface2 = NULL;

SDL_Surface* gnoSurface = NULL;
SDL_Surface* gnoSurface2 = NULL;

SDL_Surface* gjogSurface = NULL;
SDL_Surface* gjogSurface2 = NULL;

SDL_Surface* gstationSurface = NULL;
SDL_Surface* gstationSurface2 = NULL;

SDL_Surface* gTurnOverSurface = NULL;
SDL_Surface* gsleepSurface = NULL;


//exit image
SDL_Surface* gexitSurface = NULL;

//Rendered texture
LTexture gTextTexture;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }

		//Create window
		gWindow = SDL_CreateWindow( "Smart Cusion", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

			if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }

                 //Initialize SDL_ttf
                if( TTF_Init() == -1 )
                {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                    success = false;
                }
            }

			//Get window surface
			gScreenSurface = SDL_GetWindowSurface( gWindow );
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load stretching surface
	gSurface = loadSurface( "scene.bmp" );
	if( gSurface == NULL )
	{
		printf( "Failed to load smart_cushion image!\n" );
		success = false;
	}
	gbuttonSurface = loadSurface( "start_icon.bmp" );
	if( gbuttonSurface == NULL )
	{
		printf( "Failed to load start_icon image!\n" );
		success = false;
	}
	gTurnOverSurface = loadSurface( "turn_over.bmp" );
	if( gTurnOverSurface == NULL )
	{
		printf( "Failed to turn over image!\n" );
		success = false;
	}
	gsleepSurface = loadSurface( "sleep.bmp" );
	if( gsleepSurface == NULL )
	{
		printf( "Failed to load sleep image!\n" );
		success = false;
	}
	gstationSurface = loadSurface( "station.bmp" );
	if( gstationSurface == NULL )
	{
		printf( "Failed to load station image!\n" );
		success = false;
	}
	grunningSurface = loadSurface( "run.bmp" );
	if( grunningSurface == NULL )
	{
		printf( "Failed to load running image!\n" );
		success = false;
	}
	gnoSurface = loadSurface( "no.bmp" );
	if( gnoSurface == NULL )
	{
		printf( "Failed to load running image!\n" );
		success = false;
	}
	gsittingSurface = loadSurface( "sit.bmp" );
	if( gsittingSurface == NULL )
	{
		printf( "Failed to load sitting_long image!\n" );
		success = false;
	}
	glyingSurface = loadSurface( "lying.bmp" );
	if( glyingSurface == NULL )
	{
		printf( "Failed to load lying image!\n" );
		success = false;
	}
	gstandSurface = loadSurface( "stand.bmp" );
	if( gstandSurface == NULL )
	{
		printf( "Failed to load stand image!\n" );
		success = false;
	}
	gwalkSurface = loadSurface( "walk.bmp" );
	if( gwalkSurface == NULL )
	{
		printf( "Failed to load walk image!\n" );
		success = false;
	}
	gbikingSurface = loadSurface( "biking.bmp" );
	if( gbikingSurface == NULL )
	{
		printf( "Failed to load biking image!\n" );
		success = false;
	}
	gbikingSurface2 = loadSurface( "no_sleep/biking.bmp" );
	if( gbikingSurface2 == NULL )
	{
		printf( "Failed to load biking image!\n" );
		success = false;
	}
	gstationSurface2 = loadSurface( "no_sleep/station.bmp" );
	if( gstationSurface2 == NULL )
	{
		printf( "Failed to load station image!\n" );
		success = false;
	}
	grunningSurface2 = loadSurface( "no_sleep/run.bmp" );
	if( grunningSurface2 == NULL )
	{
		printf( "Failed to load running image!\n" );
		success = false;
	}
	gnoSurface2 = loadSurface( "no_sleep/no.bmp" );
	if( gnoSurface2 == NULL )
	{
		printf( "Failed to load running image!\n" );
		success = false;
	}
	gsittingSurface2 = loadSurface( "no_sleep/sit.bmp" );
	if( gsittingSurface2 == NULL )
	{
		printf( "Failed to load sitting_long image!\n" );
		success = false;
	}
	glyingSurface2 = loadSurface( "no_sleep/lying.bmp" );
	if( glyingSurface2 == NULL )
	{
		printf( "Failed to load lying image!\n" );
		success = false;
	}
	gstandSurface2 = loadSurface( "no_sleep/stand.bmp" );
	if( gstandSurface2 == NULL )
	{
		printf( "Failed to load stand image!\n" );
		success = false;
	}
	gwalkSurface2 = loadSurface( "no_sleep/walk.bmp" );
	if( gwalkSurface2 == NULL )
	{
		printf( "Failed to load walk image!\n" );
		success = false;
	}
	gexitSurface = loadSurface( "exit.bmp" );
	if( gexitSurface == NULL )
	{
		printf( "Failed to load exit image!\n" );
		success = false;
	}

	/*//open font
	gFont = TTF_OpenFont( "lazy.ttf", 28 );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		//Render text
		SDL_Color textColor = { 0, 0, 0 };
		if( !gTextTexture.loadFromRenderedText( "The quick brown fox jumps over the lazy dog", textColor ) )
		{
			printf( "Failed to render text texture!\n" );
			success = false;
		}
	}*/
	return success;
}

void close()
{
	//Free loaded image
	SDL_FreeSurface( gSurface );
	gSurface = NULL;

	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

SDL_Surface* loadSurface( string path )
{
	//The final optimized image
	SDL_Surface* optimizedSurface = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
	}
	else
	{
		//Convert surface to screen format
		optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, 0 );
		if( optimizedSurface == NULL )
		{
			printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return optimizedSurface;
}

int recvtimeout(int sockfd, char *buf, int len, int timeout)
{
	fd_set fds;	
	int n;
	struct timeval tv;

	// file descriptor set
	FD_ZERO(&fds);
	FD_SET(sockfd, &fds);

	// set timeout struct timeval
	tv.tv_sec = timeout;
	tv.tv_usec = 0;

	// wait until timeout or recieve data
	n = select(sockfd+1, &fds, NULL, NULL, &tv);
	// time out
	if (n == 0) 
	{
		cout<<"time out"<<endl;
		return -2; 
	}
	// error
	if (n == -1) return -1;

	return recv(sockfd, buf, len, 0);
}

void socket_client()
{	
	//
	SDL_Rect stretchRect;
	stretchRect.x = 0;
	stretchRect.y = 0;
	stretchRect.w = SCREEN_WIDTH;
	stretchRect.h = SCREEN_HEIGHT;

	SDL_Rect exitRect;
	exitRect.x = SCREEN_WIDTH*5/6;
	exitRect.y = SCREEN_HEIGHT*3/4;
	exitRect.w = SCREEN_WIDTH/6;
	exitRect.h = SCREEN_HEIGHT/4;
	SDL_BlitScaled( grunningSurface, NULL, gScreenSurface, &stretchRect );
							

	SDL_BlitScaled( gexitSurface, NULL, gScreenSurface, &exitRect );
	//Update the surface
	SDL_UpdateWindowSurface( gWindow );

	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr;
	char sendBuff[1024];
	char recvBuff[1024];
	time_t ticks;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(sendBuff, '0', sizeof(sendBuff));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(65431);
	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	listen(listenfd, 10);

	int result=0;
	string strtime;
	strcpy(sendBuff, "Hello");


	bool exit = false;
    //state: 0: healthy, 1:too right, 2:too left, 3:sit too long, 4: leave
    while(!exit)
    {
		SDL_Event e_2;
		bool mouse = false;
	//	int sendState = send(fd,message,sizeof(message),MSG_DONTWAIT);

		//cout << "sendState = " <<receiveMessage[0] <<endl;
        //int recvState = recv(sockfd,state,1,0);
		//int recvState = recvtimeout(sockfd, receiveMessage, sizeof(receiveMessage), 10);
		//socket server is closed
		/*if(recvState == 0)
		{
			cout<<"close Socket"<<endl;
    		close(sockfd);
			return;
		}
		//error
		else if(recvState == -1)
		{
			cout<<"error";
		}*/
			/*
		else if(receiveMessage == "fast walking")
		{
			SDL_BlitScaled( grunningSurface, NULL, gScreenSurface, &stretchRect );
		}
		else if(receiveMessage == "")
		{
			SDL_BlitScaled( grightSurface, NULL, gScreenSurface, &stretchRect );
		}
		else if(receiveMessage[0] == '2')
		{
			SDL_BlitScaled( gleftSurface, NULL, gScreenSurface, &stretchRect );
		}
		else if(receiveMessage[0] == '3') 
		{
			SDL_BlitScaled( gsittingSurface, NULL, gScreenSurface, &stretchRect );
		}

		SDL_BlitScaled( gexitSurface, NULL, gScreenSurface, &exitRect );
        //recv(sockfd,receiveMessage,sizeof(receiveMessage),0);*/
		string msg="";
		bool IsTurnOver=false;
		while (1) {
			connfd=accept(listenfd, (struct sockaddr*)NULL, NULL);
			result=0;

			while(1) {
				ticks=time(NULL);
				if(0< (result=read(connfd,recvBuff, sizeof(recvBuff)-1))) {
					strtime=ctime(&ticks);
					strtime.erase(strtime.size()-1);
					cout<<strtime<<": ";
					recvBuff[result]='\0';
					cout<<recvBuff<<endl;
					
					msg=recvBuff;
					
					if(msg == "no activity")
						SDL_BlitScaled( gnoSurface, NULL, gScreenSurface, &stretchRect );
					else if(msg == "stationary") {
						cout<<"mom Im here\n";
						SDL_BlitScaled( gstationSurface, NULL, gScreenSurface, &stretchRect );
					}
					else if(msg == "standing")
						SDL_BlitScaled( gstandSurface, NULL, gScreenSurface, &stretchRect );
					else if(msg == "sitting")
						SDL_BlitScaled( gsittingSurface, NULL, gScreenSurface, &stretchRect );
					else if(msg == "lying")
						SDL_BlitScaled( glyingSurface, NULL, gScreenSurface, &stretchRect );
					else if(msg == "walking")
						SDL_BlitScaled( gwalkSurface, NULL, gScreenSurface, &stretchRect );
					else if(msg == "fast walking")
						SDL_BlitScaled( grunningSurface, NULL, gScreenSurface, &stretchRect );
					else if(msg == "jogging")
						SDL_BlitScaled( gjogSurface, NULL, gScreenSurface, &stretchRect );
					else if(msg == "biking")
						SDL_BlitScaled( gbikingSurface, NULL, gScreenSurface, &stretchRect );

					else if(msg == "turn over"){
						SDL_BlitScaled( gTurnOverSurface, NULL, gScreenSurface, &stretchRect );
						IsTurnOver=true;
					}

					else if(msg == "sleeping")
						SDL_BlitScaled( gsleepSurface, NULL, gScreenSurface, &stretchRect );

					else if(msg == "no sleeping, no activity")
						SDL_BlitScaled( gnoSurface2, NULL, gScreenSurface, &stretchRect );
					else if(msg == "no sleeping, stationary") {
						SDL_BlitScaled( gstationSurface2, NULL, gScreenSurface, &stretchRect );
					}
					else if(msg == "no sleeping, standing")
						SDL_BlitScaled( gstandSurface2, NULL, gScreenSurface, &stretchRect );
					else if(msg == "no sleeping, sitting")
						SDL_BlitScaled( gsittingSurface2, NULL, gScreenSurface, &stretchRect );
					else if(msg == "no sleeping, lying")
						SDL_BlitScaled( glyingSurface2, NULL, gScreenSurface, &stretchRect );
					else if(msg == "no sleeping, walking")
						SDL_BlitScaled( gwalkSurface2, NULL, gScreenSurface, &stretchRect );
					else if(msg == "no sleeping, fast walking")
						SDL_BlitScaled( grunningSurface2, NULL, gScreenSurface, &stretchRect );
					else if(msg == "no sleeping, jogging")
						SDL_BlitScaled( gjogSurface2, NULL, gScreenSurface, &stretchRect );
					else if(msg == "no sleeping, biking")
						SDL_BlitScaled( gbikingSurface2, NULL, gScreenSurface, &stretchRect );
//					SDL_BlitScaled( gexitSurface, NULL, gScreenSurface, &exitRect );
					SDL_UpdateWindowSurface( gWindow );
					if (IsTurnOver){
						sleep(1);
						IsTurnOver=false;
					}
					// write to screen
					gFont = TTF_OpenFont( "lazy.ttf", 28 );
					if( gFont == NULL )
					{	
						printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
					}
					else
					{
						//Render text
						SDL_Color textColor = { 0, 0, 0 };
						if( !gTextTexture.loadFromRenderedText( recvBuff , textColor ) )
						{
							printf( "Failed to render text texture!\n" );
						}
					}

					write(connfd, sendBuff, strlen(sendBuff));
				}
			}
		}
		//Update the surface
		SDL_UpdateWindowSurface( gWindow );
		while(SDL_PollEvent( &e_2 ) != 0)
		{	
			if( e_2.type == SDL_MOUSEBUTTONDOWN )
			{
				mouse = true;
			}
			else if( e_2.type == SDL_MOUSEBUTTONUP && mouse )
			{
				exit = checkmousepos(exitRect);
			}
		}
    }

    //cout<<receiveMessage<<endl;
    cout<<"close Socket"<<endl;
    close(connfd);
    return ;
}

//set the start view
void reset_screen(SDL_Rect &buttonRect)
{
	//Apply the image stretched
	SDL_Rect stretchRect;
	stretchRect.x = 0;
	stretchRect.y = 0;
	stretchRect.w = SCREEN_WIDTH;
	stretchRect.h = SCREEN_HEIGHT;
	SDL_BlitScaled( gSurface, NULL, gScreenSurface, &stretchRect );

	SDL_BlitScaled( gbuttonSurface, NULL, gScreenSurface, &buttonRect );

	//Update the surface
	SDL_UpdateWindowSurface( gWindow );
}

bool checkmousepos(SDL_Rect &Rect)
{
	cout<<"checkmouse"<<endl;
	int x,y;
	SDL_GetMouseState( &x, &y );
	cout<<"x = "<<x<<",y ="<<y<<endl;
	if( x>Rect.x && x<Rect.x+Rect.w && y>Rect.y && y<Rect.y+Rect.h )
		return false;
	else return true;
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;
			bool mouse = false;

			//Event handler
			SDL_Event e;

			//Start button view point
			SDL_Rect buttonRect;
			buttonRect.x = SCREEN_WIDTH*2/5;
			buttonRect.y = SCREEN_HEIGHT*2/3;
			buttonRect.w = SCREEN_WIDTH/5;
			buttonRect.h = SCREEN_HEIGHT/6;

			reset_screen(buttonRect);

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					else if( e.type == SDL_MOUSEBUTTONDOWN )
					{
						mouse = true;
					}
					else if( e.type == SDL_MOUSEBUTTONUP && mouse )
					{
						if( !checkmousepos(buttonRect))//x>buttonRect.x && x<buttonRect.x+buttonRect.w && y>buttonRect.y && y<buttonRect.y+buttonRect.h )
						{
							//Apply the image stretched
							/*
							SDL_Rect runningRect;
							runningRect.x = 0;
							runningRect.y = 0;
							runningRect.w = SCREEN_WIDTH;
							runningRect.h = SCREEN_HEIGHT;
							SDL_BlitScaled( grunningSurface, NULL, gScreenSurface, &runningRect );
							//Update the surface
							SDL_UpdateWindowSurface( gWindow );
							*/
							socket_client();
							reset_screen(buttonRect);
						}
						mouse = false;
					}
				}
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
