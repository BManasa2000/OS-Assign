#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/shm.h>
#include <time.h>


#define BUFFER_SIZE 30
#define READ_END 0
#define WRITE_END 1

#define FILE_C1 "C1_input.txt"
#define FILE_C2 "C2_input.txt"
#define FILE_C3 "C3_input.txt"

static int sleep_sch[3];
static int left;
double time_quant;

int task_done[3];

int q_empty(int *queue)
{
	if ((queue[0] == -1) && (queue[1] == -1) && (queue[2] == -1))
		return 1; // all processes have exited
	return 0;
}

struct c1_struct
{
	int n1;
	int *arr;
	int *fd1;
	char *write_msg1;
};

struct c2_struct
{
	int n2;
	int *fd2;
	char *write_msg2;
};

struct c3_struct
{
	int n3;
	int *fd3;
	char *write_msg3;
};

void* c1_task (void *arg)
{
		
	clock_t burst_begin1 = clock();
	
	struct c1_struct *actual_arg = arg;

	int n1 = actual_arg->n1;
	int *arr = actual_arg->arr;
	int *fd1 = actual_arg->fd1;
	char *write_msg1 = actual_arg->write_msg1;

	int i = 0;

	long long sum = 0;
	while (1)
	{
		if (sleep_sch[0])
		{
			sleep((double)time_quant);
		}
		else if(!sleep_sch[0]){
		if (i != n1)
		{
			sum += arr[i];
			i++;
		}
		else if (i == n1)
			break;}
	}

	task_done[0] = 1;
	
	close (fd1[READ_END]);
	sprintf (write_msg1, "%lld", sum); // converting integer to string and storing in msg_write1
	write (fd1[WRITE_END], write_msg1, strlen(write_msg1) + 1);
	close(fd1[WRITE_END]);
	
	clock_t burst_end1 = clock();
	printf("BURST FOR PROCESS 1 = %lf\n", (double)(burst_end1 - burst_begin1));
	
}

void* c1_monitor (void *arg)
{
	int shmid;
	int *shmptr;

	if ((shmid = shmget(2041, 10*sizeof(int), 0)) == -1)
	{
		perror("Error in shmget() in child 1\n");
		exit(1);
	}

	shmptr = (int *)shmat(shmid, 0, 0);

	if (shmptr == (int *) - 1)
	{
		perror ("Error in shmat() in child 1\n");
		exit(2);
	}
	while (1)
	{
		sleep_sch[0] = shmptr[0];

		if (task_done[0] == 1)
		{
			shmptr[3] = 2;
			break;
		}
	}
	
	shmdt(shmptr);
}

void* c2_task (void *arg)
{
	clock_t begin2 = clock();
	double io_time = 0.0;
	
	struct c2_struct *actual_arg = arg;
	int n2 = actual_arg->n2;
	int *fd2 = actual_arg->fd2;
	char *write_msg2 = actual_arg->write_msg2;

	FILE *fp2;
	fp2 = fopen (FILE_C2, "r+");

	char buf[255];
	int i = 0;
	while (1)
	{
		if (sleep_sch[1] == 1)
		{
			sleep((double)time_quant);
		}
		else if (i != n2)
		{
			clock_t io_begin, io_end;
			io_begin = clock();
			fscanf(fp2, "%s", buf);
			printf("%s ", buf);
			io_end = clock();
			io_time += (double)(io_end - io_begin);
			i++;
		}
		else if (i == n2)
			break;
	}

	task_done[1] = 1;
	
	close (fd2[READ_END]);
	write (fd2[WRITE_END], write_msg2, strlen(write_msg2)+1);
	close (fd2[WRITE_END]);
	fclose(fp2);

	clock_t end2 = clock();
	
	printf("\n BURST FOR PROCESS 2 = %lf\n", (double)(end2 - begin2));
}

void* c2_monitor (void *arg)
{
	int shmid;
	int *shmptr;

	if ((shmid = shmget(2041, 10*sizeof(int), 0)) == -1)
	{
		perror("Error in shmget() in child 2\n");
		exit(1);
	}

	shmptr = (int *)shmat(shmid, 0, 0);

	if (shmptr == (int *) - 1)
	{
		perror ("Error in shmat() in child 2\n");
		exit(2);
	}

	while (1)
	{
		sleep_sch[1] = shmptr[1];
		if (task_done[1] == 1)
		{
			shmptr[4] = 2;
			break;
		}
	}
	
	shmdt(shmptr);
}

void* c3_task (void *arg)
{
	clock_t begin3 = clock();
	
	struct c3_struct *actual_arg = arg;
	int n3 = actual_arg->n3;
	int *fd3 = actual_arg->fd3;
	char *write_msg3 = actual_arg->write_msg3;

	FILE *fp3;
	fp3 = fopen (FILE_C3, "r+");

	char buf[255];
	long long sum = 0;
	int n;
	int i = 0;
	while (1)
	{
		if (sleep_sch[2] == 1)
		{
			sleep((double)time_quant);
		}
		else if (i != n3)
		{
			fscanf(fp3, "%s", buf);
			n = atoi(buf);
			sum += n;
			i++;
		}
		else if (i == n3)
			break;
	}
	
	task_done[2] = 1;

	close (fd3[READ_END]);
	sprintf (write_msg3, "%lld", sum); // converting integer to string and storing in msg_write1
	write (fd3[WRITE_END], write_msg3, strlen(write_msg3) + 1);
	close(fd3[WRITE_END]);
	fclose(fp3);
	
	clock_t end3 = clock();
	printf("BURST FOR PROCESS 3 = %lf\n", (double)(end3 - begin3));
}

void* c3_monitor (void *arg)
{
	int shmid;
	int *shmptr;
	
	if ((shmid = shmget(2041, 10*sizeof(int), 0)) == -1)
	{
		perror("Error in shmget() in child 3\n");
		exit(1);
	}

	shmptr = (int *)shmat(shmid, 0, 0);

	if (shmptr == (int *) - 1)
	{
		perror ("Error in shmat() in child 3\n");
		exit(2);
	}

	while (1)
	{
		sleep_sch[2] = shmptr[2];

		if (task_done[2] == 1)
		{
			shmptr[5] = 2;
			break;
		}
	}	
	shmdt(shmptr);
}

int main(int argc, char** argv)
{
	printf("1 Round Robin \n2 FCFS\n");
	int choice;
	scanf("%d",&choice);
	if(choice==1){
		printf("Enter time quantum\n");
		scanf("%lf", &time_quant);
	}
	sleep_sch[0] = sleep_sch[1] = sleep_sch[2] = 1;
	task_done[0] = task_done[1] = task_done[2] = 0;
	pid_t pid1, pid2, pid3;
	int fd1[2], fd2[2], fd3[2];
	int *shmptr;
	int shmid;
	
	FILE *fp1;
	fp1 = fopen (FILE_C1, "r+");
	char buf[255];
	
	int n1 = atoi (argv[1]);
	int arr[n1];
	for(int i = 0; i < n1; i++)
	{
		fscanf(fp1, "%s", buf);
		int n = atoi(buf);
		arr[i] = n;
	}
	
	fclose(fp1);

	// creating the pipes
	if (pipe(fd1) == -1 || pipe(fd2) == -1 || pipe(fd3) == -1) 
	{
		perror("Error in the file system call\n");
		exit(1);
	}

	char write_msg1[BUFFER_SIZE], read_msg1[BUFFER_SIZE];
	char write_msg2[BUFFER_SIZE] = "Done Printing", read_msg2[BUFFER_SIZE];
	char write_msg3[BUFFER_SIZE], read_msg3[BUFFER_SIZE];


	// THREAD DECLARATIONS

	pthread_t tid1_mon; // thread identifier for monitor thread of child process 1
	pthread_attr_t attr1_mon; // set of thread attributes for monitor thread of child 1
	pthread_t tid1_task; // thread identifier for task thread of child process 1
	pthread_attr_t attr1_task; // set of thread attributes for task thread of child process 1

	pthread_t tid2_mon; // thread identifier for monitor thread of child process 2
	pthread_attr_t attr2_mon; // set of thread attributes for monitor thread of child 2
	pthread_t tid2_task; // thread identifier for task thread of child process 2
	pthread_attr_t attr2_task; // set of thread attributes for task thread of child process 2
	
	pthread_t tid3_mon; // thread identifier for monitor thread of child process 2
	pthread_attr_t attr3_mon; // set of thread attributes for monitor thread of child 2
	pthread_t tid3_task; // thread identifier for task thread of child process 2
	pthread_attr_t attr3_task; // set of thread attributes for task thread of child process 2

	
	clock_t arrival1, tat_end1;
	clock_t arrival2, tat_end2;
	clock_t arrival3, tat_end3;
	
	double waiting1, waiting2, waiting3;
	waiting1 = waiting2 = waiting3 = 0;
	
	arrival1 = clock();
	
	struct timespec start1, end1;
	struct timespec start2, end2;
	struct timespec start3, end3;
	
	
	clock_gettime(CLOCK_MONOTONIC_RAW, &start1);
	pid1 = fork(); // fork a child process
	if (pid1 < 0) // error occurred
	{
		fprintf(stderr, "Fork 1 Failed");
		return 1;
	}
	else if (pid1 == 0) // child process 1
	{
		struct c1_struct *arg1 = malloc(sizeof *arg1);
		arg1->n1 = n1;
		arg1->arr = &arr;
		arg1->fd1 = &fd1; 
		arg1->write_msg1 = &write_msg1;

		// MONITOR THREAD FOR CHILD PROCESS 1
		pthread_attr_init(&attr1_mon); // set the default attributes
		pthread_create(&tid1_mon, &attr1_mon, c1_monitor, NULL); // create the monitor thread
		

		// END OF MONITOR THREAD CODE

		// TASK THREAD FOR CHILD PROCESS 1
		pthread_attr_init(&attr1_task); // set the default attributes
		pthread_create(&tid1_task, &attr1_task, c1_task, (void *)arg1); // create the task thread
		// END OF TASK THREAD CODE

		pthread_join(tid1_task, NULL);  // wait for the task thread to exit
		pthread_join(tid1_mon, NULL);  // wait for the monitor thread to exit
	}
	else // parent process 
	{
		
		arrival2 = clock();
		clock_gettime(CLOCK_MONOTONIC_RAW, &start2);
		pid2 = fork();
		if (pid2 < 0)
		{
			fprintf(stderr, "Fork 2 Failed");
			return 1;
		}
		else if (pid2 == 0) // child process 2
		{
			
			int n2 = atoi(argv[2]);

			struct c2_struct *arg2 = malloc(sizeof *arg2);
			arg2->n2 = n2;
			arg2->fd2 = &fd2;
			arg2->write_msg2 = &write_msg2;

			// MONITOR THREAD FOR CHILD PROCESS 2
			pthread_attr_init(&attr2_mon); // set the default attributes
			pthread_create(&tid2_mon, &attr2_mon, c2_monitor, NULL); // create the task thread

			// END OF MONITOR THREAD CODE

			// TASK THREAD FOR CHILD PROCESS 2
			pthread_attr_init(&attr2_task); // set the default attributes
			pthread_create(&tid2_task, &attr2_task, c2_task, (void *)arg2); // create the task thread

			// END OF TASK THREAD CODE

			pthread_join(tid2_task, NULL);  // wait for the thread to exit
			pthread_join(tid2_mon, NULL);  // wait for the monitor thread to exit

		}
		else // parent process
		{
			arrival3 = clock();
			clock_gettime(CLOCK_MONOTONIC_RAW, &start3);
			pid3 = fork();
			if (pid3 < 0)
			{
				fprintf(stderr, "Fork 3 failed\n");
				return 1;
			}
			else if (pid3 == 0) // child process 3
			{
				int n3 = atoi(argv[3]);

				struct c3_struct *arg3 = malloc(sizeof *arg3);
				arg3->n3 = n3;
				arg3->fd3 = &fd3;
				arg3->write_msg3 = &write_msg3;

				// MONITOR THREAD FOR CHILD PROCESS 3
				pthread_attr_init(&attr3_mon); // set the default attributes
				pthread_create(&tid3_mon, &attr3_mon, c3_monitor, NULL); // create the monitor thread

				// END OF MONITOR THREAD CODE

				// TASK THREAD FOR CHILD PROCESS 3
				pthread_attr_init(&attr3_task); // set the default attributes
				pthread_create(&tid3_task, &attr3_task, c3_task, (void *)arg3); // create the task thread

				// END OF TASK THREAD CODE

				pthread_join(tid3_task, NULL);  // wait for the thread to exit
				pthread_join(tid3_mon, NULL);  // wait for the monitor thread to exit

			}
			else // parent process
			{

				// for shared memory
				if ((shmid = shmget(2041, 10*sizeof(int), 8644 | IPC_CREAT)) == -1)
				{
					perror ("Error in shmget() in parent\n");
					exit(3);
				}

				shmptr = (int *)shmat(shmid, 0, 0);
				shmptr[0]=shmptr[1]=shmptr[2]=shmptr[3]=shmptr[4]=shmptr[5]=1;
				
				if (shmptr == (int *) - 1)
				{
					perror ("Error in shmat() in parent\n");
					exit(4);
				}
				
				int queue[3] = {1, 2, 3};
				int top = 0;
				clock_t current_time;
				
				u_int64_t tatt1, tatt2, tatt3;
				while (q_empty(&queue[0]) == 0)
				{
					int p_top;
					while (queue[top] == -1)
						top = (top + 1)%3;
					p_top = queue[top];
					if (p_top == 1)
					{
						
						if (shmptr[0] == 1) // child 1 is sleeping
						{
							shmptr[0] = 0;
							shmptr[1] = 1;
							shmptr[2] = 1;
							waiting2++;
							waiting3++;
							current_time=clock();
							printf("Child 1 starts at %lf clock ticks\n",(double)current_time);
							if(choice == 1){ // round robin
								sleep((double)time_quant);
								shmptr[0] = 1;
							}
							else{ // fcfs
								while(shmptr[3]!=2){
									sleep((double)0.000001);
								}
							}
						}
						if (shmptr[3] == 2)
						{
							queue[top] = -1;
							close(fd1[WRITE_END]);
							read (fd1[READ_END], read_msg1, BUFFER_SIZE);
							printf("Msg from C1: %s\n", read_msg1);
							close (fd1[READ_END]);
							clock_gettime(CLOCK_MONOTONIC_RAW, &end1);
							tatt1 = (end1.tv_sec - start1.tv_sec)*1000000 + (end1.tv_nsec - start1.tv_nsec)/1000;
							
						}
						
					}
					else if (p_top == 2)
					{
						
						if (shmptr[1] == 1) // child 1 is sleeping
						{
							shmptr[1] = 0;
							shmptr[0] = 1;
							shmptr[2] = 1;
							waiting1++;
							waiting3++;
							current_time=clock();
							printf("Child 2 starts at %lf clock ticks\n",(double)current_time);
							if(choice == 1){
								sleep((double)time_quant);
								shmptr[1] = 1;
							}
							else{
								while(shmptr[4]!=2){
									sleep((double)0.000001);
								}
							}
						}
						if (shmptr[4] == 2)
						{
							queue[top] = -1;
							close (fd2[WRITE_END]);
							read (fd2[READ_END], read_msg2, BUFFER_SIZE);
							printf ("Msg from C2: %s\n", read_msg2);
							close (fd2[READ_END]);
							clock_gettime(CLOCK_MONOTONIC_RAW, &end2);
							tatt2 = (end2.tv_sec - start2.tv_sec)*1000000 + (end2.tv_nsec - start2.tv_nsec)/1000;

						}
						
					}
					else if (p_top == 3)
					{
						
						if (shmptr[2] == 1) // child 1 is sleeping
						{
							shmptr[2] = 0;
							shmptr[1] = 1;
							shmptr[0] = 1;
							waiting1++;
							waiting2++;
							current_time=clock();
							printf("Child 3 starts at %lf clock ticks\n",(double)current_time);
							if(choice == 1){
								sleep((double)time_quant);
								shmptr[2] = 1;
							}
							else{
								while(shmptr[5]!=2){
									sleep((double)0.000001);
								}
							}
						}
						if (shmptr[5] == 2)
						{
							queue[top] = -1;
							close(fd3[WRITE_END]);
							read (fd3[READ_END], read_msg3, BUFFER_SIZE);
							printf("Msg from C3: %s\n", read_msg3);
							close (fd3[READ_END]);
							clock_gettime(CLOCK_MONOTONIC_RAW, &end3);
							tatt3 = (end3.tv_sec - start3.tv_sec)*1000000 + (end3.tv_nsec - start3.tv_nsec)/1000;
							
						}
						
					}
					top = (top + 1)%3;
				}
				
				printf("Turn Around Time for C1 = %ld\n\n", tatt1);
				printf("Turn Around Time for C2 = %ld\n\n", tatt2);
				printf("Turn Around Time for C3 = %ld\n\n", tatt3);
				
				wait(NULL);
				wait(NULL);
				wait(NULL);
				
				
				if(shmctl(shmid, IPC_RMID, NULL)==-1){
					perror("ERROR");
					exit(5);
				}
				
				shmdt(shmptr);
			}
		}
	}
	
	return 0;
}
