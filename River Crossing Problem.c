/* 
Name: Raghav Mathur
Reg. No: 201900434

Algorithm: 
Step 1:  Start
Step 2: define HACKER -> 0
		define SERF -> 1
Step 3: Inside typedef struct create the following variables :
		pthread_mutex_t mutex  
		pthread_barrier_t barrier  
		int numHackers   
		int numSerfs    
		int totalLimit      
		sem_t hackers_queue   
		sem_t serfs_queue 
Step 4: Create a structure pthread_board, which contains the following:
		Boat * boat
		int isCaptain
		int type
Step 5:  Inside void * board():
		Step 5.1: if(args->type == SERF)
			Step 5.1.1: if(args->boat->numSerfs == args->boat->totalLimit)
						pthread_mutex_lock(&args->boat->mutex)
						sem_wait(&args->boat->serfs_queue)
			Step 5.1.2: else if(args->boat->numHackers == args->boat->totalLimit)
						pthread_mutex_lock(&args->boat->mutex)
						sem_wait(&args->boat->hackers_queue)
		Step 5.2: Repeat the above for HACKER
Step 6:  Inside rowBoat():
		Step 6.1: Display "Row"
				Display "Unload"
		Step 6.2: for(i=0;i<totalLimit;i++) do
				sem_post(queue)
				End for
Step 7:  Inside void printBoat(): Display boat.numHackers, boat.numSerfs
Step 8:  Inside int main():
		Initialize: boat.totalLimit -> 4, boat.numHackers -> 0, boat.numSerfs -> 0
		Step 8.1: while(1) do:
				sleep(1)
				int person_type -> random() % 2
				args.type -> person_type;
				pthread_create(&person,NULL, board,(void *)&args)
				End while
Step 9:  Stop


Sample Output:
  	Hacker: 1, SERF: 0
	Hacker: 1, SERF: 1
	Hacker: 2, SERF: 1
	Hacker: 2, SERF: 2
	Boat is full and is now Rowing
	Boat is Unloaded

	Hacker: 1, SERF: 0
	Hacker: 1, SERF: 1
	Hacker: 1, SERF: 2
	Hacker: 2, SERF: 2
	Boat is full and is now Rowing
	Boat is Unloaded

	Hacker: 0, SERF: 1
	Hacker: 1, SERF: 1
	Hacker: 2, SERF: 1
	Hacker: 2, SERF: 2
	Boat is full and is now Rowing
	Boat is Unloaded

	Hacker: 1, SERF: 0
	Hacker: 2, SERF: 0
	Hacker: 2, SERF: 1
	Hacker: 2, SERF: 2
	Boat is full and is now Rowing
	Boat is Unloaded

	Hacker: 1, SERF: 0
	Hacker: 2, SERF: 0
	Hacker: 3, SERF: 0
	Hacker: 4, SERF: 0
	Boat is full and is now Rowing
	Boat is Unloaded
*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <unistd.h>

#define HACKER 0
#define SERF 1

//Creating Structure Boat with variables like number of hackers, number of serfs and the total limit
typedef struct{	 	  	 	    	 	 	   	 	
    pthread_mutex_t mutex;   
    pthread_barrier_t barrier;  
    int numHackers;   
    int numSerfs;     
    int totalLimit;         
    sem_t hackers_queue;   //semaphores
    sem_t serfs_queue;     //semaphores 
} Boat;

//Another structure which will be used to create the boat thread
struct pthread_board{
    Boat *boat;
    int isCaptain;
    int type;
};

//Function to initiate the boarding of the boat in the program
void *board(void *a){
    struct pthread_board * args = (struct pthread_board *)a;
    if(args->type == SERF){
        args->boat->numSerfs++;
        sem_post(&args->boat->serfs_queue);
		//Here we check if the total SERFS are equal to the totalLimit(that means all are SERFS) and if they are we can print the result
        if(args->boat->numSerfs == args->boat->totalLimit){
            printBoat(*args->boat);
            pthread_barrier_wait(&(args->boat->barrier));   
            args->isCaptain = 1;   
            pthread_mutex_lock(&args->boat->mutex);
            rowBoat(&args->boat->mutex, &args->isCaptain, &args->boat->serfs_queue, args->boat->totalLimit);
            args->boat->numSerfs = 0;
        }
		//Here we check if the total Hackers are equal 2 and the number of SERFs = 2 hence we can print the result
        else if (args->boat->numHackers == 2 && args->boat->numSerfs == 2){
            printBoat(*args->boat);
            pthread_barrier_wait(&(args->boat->barrier));    
            args->isCaptain = 1;   
            pthread_mutex_lock(&args->boat->mutex);
            rowBoat(&args->boat->mutex, &args->isCaptain, &args->boat->serfs_queue, args->boat->totalLimit);
            args->boat->numHackers = 0;
            args->boat->numSerfs = 0;
        }	 
		//If the above two statements are false then we can add another hacker and we have to decrease the number	  	 	    	 	 	   	 	
        else{
            if(args->boat->numHackers + args->boat->numSerfs >= args->boat->totalLimit){
                args->boat->numSerfs--;
            }
            else{
                printBoat(*args->boat);
                pthread_barrier_wait(&(args->boat->barrier));    
                pthread_mutex_unlock(&(args->boat->mutex));
            }
        }
        sem_wait(&args->boat->serfs_queue);           
    }
    else{
        args->boat->numHackers++;
        sem_post(&args->boat->hackers_queue);
		//Here we check if the total Hackers are equal to the totalLimit and if they are we can print the result
        if(args->boat->numHackers == args->boat->totalLimit){
            printBoat(*args->boat);
            pthread_barrier_wait(&(args->boat->barrier));    
            args->isCaptain = 1;   
            pthread_mutex_lock(&args->boat->mutex);
            rowBoat(&args->boat->mutex, &args->isCaptain, &args->boat->hackers_queue, args->boat->totalLimit);
            args->boat->numHackers = 0;
        }
		//Here we check if the total Hackers are equal 2 and the number of SERFs = 2 hence we can print the result
        else if (args->boat->numHackers == 2 && args->boat->numSerfs == 2){
            printBoat(*args->boat);
            pthread_barrier_wait(&(args->boat->barrier));   
            args->isCaptain = 1;   
            pthread_mutex_lock(&args->boat->mutex);
            rowBoat(&args->boat->mutex, &args->isCaptain,&args->boat->hackers_queue, args->boat->totalLimit);
            args->boat->numSerfs = 0;
            args->boat->numHackers = 0;
        }
		//If the above two statements are false then we can add another hacker and we have to decrease the number
        else{
            if(args->boat->numHackers + args->boat->numSerfs >= args->boat->totalLimit){
                args->boat->numHackers--;
            }	 	  	 	    	 	 	   	 	
            else{
                printBoat(*args->boat);
                pthread_barrier_wait(&(args->boat->barrier));   
                pthread_mutex_unlock(&(args->boat->mutex));
            }
        }
        sem_wait(&args->boat->hackers_queue);             
    }
}; 
//Function to print when the row has completed boarding and ready to row
void rowBoat(pthread_mutex_t * mutex, int * isCaptain, sem_t * queue, const int totalLimit){
    int i;
    printf("Boat is full and is now Rowing\n");             
    printf("Boat is Unloaded\n\n");               
    pthread_mutex_unlock(mutex);
    for(i=0;i<totalLimit;i++) sem_post(queue);         
    *isCaptain = 0;
};

//Function to Print the result of the boarding 
void printBoat(const Boat boat){
    printf("Hacker: %d, SERF: %d\n", boat.numHackers, boat.numSerfs);
}


int main(){
    system("clear");
    printf("Exit program by pressing the Ctrl + C\n");
    sleep(2);
    system("clear");
    sleep(1);
    srand(time(NULL)); 
    int isCaptain;
    Boat boat; // Creates an object of the boat structure
    pthread_t person;
    isCaptain = 0;
    boat.totalLimit = 4;
    boat.numHackers = 0;
    boat.numSerfs = 0;
    pthread_barrier_init(&(boat.barrier), NULL, 4);
    pthread_mutex_init(&(boat.mutex), NULL);
    pthread_mutex_lock(&(boat.mutex));
    sem_init(&boat.hackers_queue, 0, 0);        
    sem_init(&boat.serfs_queue, 0, 0);      
    struct pthread_board args;
    args.boat = &boat;
    args.isCaptain = isCaptain;
    // Creating threads for the program
    while(1){	 	  	 	    	 	 	   	 	
        sleep(1);
        int person_type = rand() % 2; // with random type (HACKER or SERF) as there is 50% chance
        args.type = person_type;
        pthread_create(&person,NULL, board,(void *)&args);
    }
    pthread_join(person, NULL);
    return 0;
}