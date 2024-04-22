//Ryan Badaloo - 620140940

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

int fuel = 100;

void handle_sigusr1(int sig)
{
	printf("\nBomber %d to base, bombs away!\n",getpid());
}

void handle_sigusr2(int sig)
{
	fuel=100;
	printf("\nPlane %d has been refueled\n",getpid());
}


void main()
{
	char command[10];
	int plID,prID,base;
	base=getpid();
	int planes=0;

	struct sigaction sa;
	sa.sa_handler = &handle_sigusr1;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa,NULL);

	struct sigaction sb;
	sb.sa_handler = &handle_sigusr2;
	sb.sa_flags = SA_RESTART;
	sigaction(SIGUSR2, &sb,NULL);



	printf("The list of valid commands are\nl = launch\nb = bomb\nr = refuel\nq = quit\n");

	while(1)
	{
		printf("Enter a command: ");
		scanf("%s",command);

		if(strcmp(command,"l")==0)
		{
			planes++;
			int i=fork();
			if(i==0)
			{
				prID=getpid();
				printf("Plane %d was launched\n",prID);
				while(fuel>0)
				{
					fflush(stdout);
					sleep(3);
					fuel-=15;
					printf("\nBomber %d to base, %d%% of fuel left\n",prID,fuel);
				}
				printf("\nSOS! Plane %d is going to crash\n",prID);
				exit(0);
			}
		}
		else if(strcmp(command,"b")==0)
		{
			if(planes>0)
			{
				printf("\nEnter the ID of the plane you wish to do the bombing: \n");
				scanf("%d",&plID);
				kill(plID,SIGUSR1);
			}
			else
			{
				printf("No planes are flying, can't drop a bomb!\n");
			}

		}
		else if(strcmp(command,"r")==0)
		{
			if(planes>0)
			{
				printf("\nWhich plane should refuel?: ");
				scanf("%d",&plID);
				kill(plID,SIGUSR2);
			}
			else
			{
				printf("No planes were launched, none to refuel\n");
			}
		}
		else if(strcmp(command,"q")==0)
		{
			kill(0,SIGTERM);
		}
		else
		{
			printf("%s is not a valid command\n",command);
		}
	}
}