
/* inclusion of header files */
# include <stdio.h>
# include <stdlib.h>
# include <time.h>


/* declaration of struct event */
struct event{
	int event_day;	// contains event_day
	int event_type;	// contains type: 1 for recovery, -1 for transmission
	int node_index;	// contains the index os associated node
};

/* declaration of struct node */
struct node{
	int key;	// key stores the no. of person
	struct node* next;	// points to the next node
	struct node* previous;	// points to the previous node
};

/* declaration of List */
struct List{
	struct node* first;	// pointer to the first element
};

/* declaration of vertex */
struct vertex{
	int Node;	// contains index for person
	int** Node_ptr;	// a list of pointers pointing to different presons
};

/* declaration of Graph */
struct vertex* Graph;

/* MAX_EDGES is defined */
# define MAX_EDGES 3000

/* MAX_VERTICES is defined */
# define MAX_VERTICES 10000

/* MAX_DAYS is defined */
# define MAX_DAYS 300

/* tau is defined */
# define tau 50

/* gamma is defined */
# define gamma 20

/* a global variable to contain max no. of edges */
int maxNumberOfEdges;

/* declaratuin of Susceptible List*/
struct List* Susceptible;

/* declaration of Infected List */
struct List* Infected;

/* declaration of Recovered List */
struct List* Recovered;

/* Events array: represents priority queue*/
struct event Events[10002];

/* Susceptible count at the begining */
int susceptible_count=MAX_VERTICES;

/* infected count at the begining */
int infected_count=0;

/* recovered count atthe begining */
int recovered_count=0;

/* declaration of day count */
int day_count=1;

/* declaration of event_count */
int event_count=0;


/* declaration of arrays to help plotting */
double X_days[302];
double Y_infected[302];

/* function prototype of process_trans_SIR */
void process_trans_SIR(int concerned_node);

/* function prototype of process_rect_SIR  */
void process_rec_SIR(void);

/* function prototype for print_status */
void print_status(void);

/* function prototype for print_events */
void print_events(void);

/* function prototype for delete_first_event */
void delete_first_event(void);

/* function prototype for sort_events */
void sort_events(void);

/* function prototype for min_heapify */
void min_heapify(int index);

/* function prototype for exchange */
void exchange(int index1,int index2);

/* function prototype of calculate_transmission_day */
int calculate_trasmission_day(void);

/* function prototype for calculate recovery_day */
int calculate_recovery_day(void);

/* function prototype for susceptible_to_infected */
void susceptible_to_infected(int key_no);

/* function prototype for infected_to_recovery */
void infected_to_recovered(int key_no);

/* function protype for is_susceptible */
int is_susceptible (int key_num);

/* function prototype for createGraph */
void create_graph(void);

/* function prototype for storedata */
void store_data(void);

/* function prototype for create_plot */
void create_plot(void);

/* function prototype for printList */
void printList(void);

/* function prototype for printGraph */
void printGraph(void);



/* main starts here */
int main (void)
{
	/* seed for rand() */
	srand (time(NULL));

	/* Graph is created */
	create_graph();

	/* memory acclocation for the Lists */ 
	Susceptible=(struct List*)malloc(sizeof(struct List));
	
	Infected=(struct List*)malloc(sizeof(struct List));
	
	Recovered=(struct List*)malloc(sizeof(struct List));

	
	/* first node of each list is assigned NULL */
	Susceptible->first=NULL;
	
	Infected->first=NULL;
	
	Recovered->first=NULL;

	/* index1, index2, value are declared */
	int index1;
	
	int index2;
	
	int value;

	/* create of susceptible List */


	/* head and temp are declared */
	struct node* head;
	
	struct node* temp;
	
	/* first node of Susceptibe list is created */
	head=(struct node*)malloc(sizeof(struct node));
	
	/* head->key,head->next,head->previous are assigned */
	head->key=1;
	
	head->next=NULL;
	
	head->previous=NULL;
	
	/* Susceptible head is assigned */
	Susceptible->first=head;
	
	/* this loop adds node one by one and make the susceptible list */ 
	for (index1=2;index1<=MAX_VERTICES;++index1)
	{
		/* memory allocation for temp */
		temp=(struct node*)malloc(sizeof(struct node));

		/* temp->key, temp->next, temp->previous is set */
		temp->key=index1;
		
		head->next=temp;
		
		temp->previous=head;
		
		/* head gets modefied */
		head=temp;	
	}

	/* head->next is NULL */
	head->next=NULL;

	/* creation of susceptible List completed */


	/* first evnent of transmission */
	++event_count;

	/* event_day,event_node,event_type is assigned */
	Events[event_count].event_day=1;
	
	Events[event_count].event_type=-1;
	
	Events[event_count].node_index=1+rand()%MAX_VERTICES;


	/* prints the first infected node */
	printf("First infected node: %d\n",Events[event_count].node_index);

	/* transmit subroutine is invoked */
	process_trans_SIR(Events[event_count].node_index);


	/* if the next event is not on same day, status is printed */
	if (Events[1].event_day>1)
	{
		while(1)
		{
			/* if day_count matches with first event day, the loop breaks */
			if (day_count==Events[1].event_day)
				break;
			
			/* status is printed */
			print_status();

			/* data is stored to create plot */
			store_data();

			/* day_count increases */
			++day_count;
		}
	}
	

	/* This loop completes the tasks in the queue according to their priorities */
	while(1)
	{
		/* if no event is left or day_count is beyond limit the loop breaks */
		if (event_count==0 || day_count>MAX_DAYS)
		{
			/* if no event is left, status is printed */
			if (event_count==0)
				print_status();
			
			/* the loop breaks */			
			break;
		}

		/* if the event is transmission, process_trans_SIR is invoked */
		if (Events[1].event_type==-1)
		{
			/* value stores the node temporarily */
			value=Events[1].node_index;
			
			/* if the node is susceptible */
			if (is_susceptible(value)==1)
			{
				/* the transmit subroutine is invoked */
				process_trans_SIR(value);
			}
		}
		
		/* if event is recovery type, process_rec_SIR is invoked */
		if (Events[1].event_type==1)
		{
			process_rec_SIR();
		}
		
		/* if next event_day does not match day_count, status is printed */
		if (Events[1].event_day!=day_count)
		{
			while(1)
			{
				/* if event_day matches with day_count, the loop breaks */
				if (Events[1].event_day==day_count || day_count >MAX_DAYS)
					break;

				/* status is printed */
				print_status();
				
				/* data is stored to create plot later */
				store_data();

				/* day_count increases */
				++day_count;
			}
		}
	}
	
	/* prints status st the end of simulation */
	printf("At the end of process:\n");
		print_status();

	/* if recovery day is more than 300, first event in queue is printed */
	if (day_count>300)
	{
		/* prints the string */
		printf("\nFirst event in queue->event_day :\t%d\n\n",Events[1].event_day);
	}

	/* plot is created using gnuplot */
	create_plot();

	/* returns , and the execution concludes */
	return 0;
}
/* main ends here */


/* This function is called to store data-points of plot */
void store_data()
{
	/* x: data points represents days */
	X_days[day_count]=day_count;

	/* y: data points represents the proportion of infected persons */
	Y_infected[day_count]=infected_count/10000.00;
}
/* function store_data ends here */


/* This function creates plot using gnuplot */
void create_plot()
{
	/* code for creating first plot starts here */

	/* file pointer is declared */
	FILE* fptr_plot1=popen("gnuplot -persistent","w");
	
	/* index is declared */
	int index;
	
	/* commands for gnuplot */
	char* commands[]={
		"set title\"infection curve\"",
		"set xlabel \" t(days)\" ",
		"set ylabel \" Proportion infected (at a particular day)\"",
		"plot '-'"};

	/* the commands are passed */
	for (index=0;index<4;++index)
		fprintf(fptr_plot1,"%s\n",commands[index]);

	/* the data points are passed */
	for (index=1;index<day_count;++index)
		fprintf(fptr_plot1,"%lf %lf\n",X_days[index],Y_infected[index]);

	/* the plot is printed */
	fprintf(fptr_plot1,"e");

	/* code for creating first plot ends here */

	/* code for creating second plot starts here */

	/* file pointer is declared */
	FILE* fptr_plot2=popen("gnuplot -persistent","w");

	/* declartion of point to store the index temprarily */
	int point;

	/* declaration of shift_day */
	int shift_day=0;

	/* the day when 0.1 portion of total population is infected is found*/
	for (index=1;index<day_count;++index)
	{
		if (Y_infected[index]>=0.1)
			break;
	}

	/* if index is less than day_count */
	if (index<day_count)
	{	
		/* index is saved */
		point=index;

		/* the day is shifted: negative values for earlier days */
		for (index=point; index>=1;-- index,--shift_day)
			X_days[index]=shift_day;

		/* shift_day is assigned value */
		shift_day=0;

		/* the day is shifted: positive values for later days */
		for (index=point;index<day_count;++index,++shift_day)
			X_days[index]=shift_day;

		/* x label is changed for the second plot */
		commands[1]="set xlabel \"shifted t (0 is assigned when 1% of population is infected)\" ";

		/* commands are given */
		for (index=0;index<4;++index)
			fprintf(fptr_plot2,"%s\n",commands[index]);

		/* data points are passed one-by-one */
		for (index=1;index<day_count;++index)
			fprintf (fptr_plot2,"%lf %lf \n",X_days[index],Y_infected[index]);

		/* plotting completes */
		fprintf(fptr_plot2,"e");

	}
	/* code for creating second plot ends here */

	/* closes the first pointer */
	fclose (fptr_plot1);

	/* closes the second pointer */
	fclose (fptr_plot2);		
}
/* function create_plot ends here */


/* This loop is called when the event is transmission */
void process_trans_SIR(int concerned_node)
{
	/* deletes the first event*/
	delete_first_event();

	/* the node is shifted from susceptible to infected */
	susceptible_to_infected(concerned_node);

	/* index, index2, value are declared */
	int index;
	
	int index2;
	
	int value;

	/* vertex_index is declared */
	int vertrx_index=concerned_node-1;


	/* this loop creates events for transmission to neighbours*/
	for (index=0;index<maxNumberOfEdges;++index)
	{
		/* Graph is used to check neighbours */ 
		if (Graph[vertrx_index].Node_ptr[index]!=NULL)
		{	
			/* neighbour nodes are located */
			value=*(Graph[vertrx_index].Node_ptr[index]);
			
			/* if the node is susceptible, a transimission event entered */
			if (is_susceptible(value)==1)
			{
				/* look for an already existing event */
				for (index2=1;index2<=event_count;++index2)
				{
					/* if an event already exists */
					if (Events[index2].node_index==value)
					{
						/* if the event is transmission */
						if (Events[index2].event_type==-1)
						{	
							/* transmission_day is declared */
							int transmission_day;

							/* calculates new transmission day */
							transmission_day=day_count+calculate_trasmission_day();

							/* if new transmission comes earlier */
							if (Events[index2].event_day>transmission_day)
								/* it is accepted */
								Events[index2].event_day=transmission_day;
						}

						/* the loop breaks */
						break;
					}
				}

				/* if no event is found for the node, a new event has been created */
				if (index2>event_count)
				{
					/* this is for a new transmission event, event_count is increased */
					++event_count;

					/* event_node, event_type and event_day is assigned */
					Events[event_count].event_day=day_count+calculate_trasmission_day();
					
					Events[event_count].event_type=-1;
					
					Events[event_count].node_index=value;
				}
			}
		}
	}


	/* creating event for the recovery of the infected node*/
	++event_count;

	/* event_day,event_node,event_type is assigned */
	Events[event_count].event_day=day_count+calculate_recovery_day();
	
	Events[event_count].event_type=1;
	
	Events[event_count].node_index=concerned_node;

	/* sorts all the events after all the insertions are done*/ 
	sort_events();
}
/* function process_trans_SIR ends here */


/* This function checks whether a node is present in susceptible list */
int is_susceptible(int key_no)
{
	/* struct node* head is declared */
	struct node* head;

	/* head points to the first node is Susceptible list */
	head=Susceptible->first;

	/* this loop checks nodes in sesceptible list to match the key_no. */
	while(head!=NULL)
	{
		/* if head->key is matched, 1 is returned */
		if (head->key==key_no)
			return(1);

		/* head moves to nest node */
		head=head->next;
	}

	/* if head is NULL, 0 is returned  */
	if (head==NULL)
		return (0);
}
/* function is_susceptible ends here */


/* This function shifts a node from susceptible list to infected list */
void susceptible_to_infected(int key_no)
{
	/* struct node* head,temp1 and temp2 are declared */
	struct node* head;

	struct node* temp1;

	struct node* temp2;
	
	/* heads points to the first node in Susceptible list */
	head=Susceptible->first;

	/* printing Caution */
	if (head==NULL)
		printf("Caution1!\n");
	
	/* if Susceptible list is non-empty */
	if (head!=NULL)
	{
		/* key no. is compared with node->key */
		while(head!=NULL)
		{
			/* if match is found, the loop breaks */
			if (head->key==key_no)
				break;

			/* head moves to next node */
			head=head->next;
		}

		/* if a match is found */
		if (head!=NULL)
		{
			/* temp1 points to head->previous */
			temp1=head->previous;

			/* temp1 points to head->next */
			temp2=head->next;
			
			/* matched node is the first node */
			if (temp1==NULL)
			{
				/* Susceptible ->first is modefied */
				Susceptible->first=temp2;

				/* matched node is last node in susceotible */
				if (temp2!=NULL)
					temp2->previous=NULL;
			}

			/* matched node is not the first node */
			else
			{
				/* temp1 and temp2 is connected */
				temp1->next=temp2;

				/* iftemp2 is  not the last node */
				if (temp2!=NULL)
					temp2->previous=temp1;
			}

			/* head->previous and head->next is set to NULL */
			head->previous=NULL;
			
			head->next=NULL;
			
			/* var is declared */
			struct node* var;

			/* var points to first node in infected list */
			var=Infected->first;

			/* if there is no node in Infected list*/
			if (var==NULL)
			{
				/* Infected first is set */
				Infected->first=head;
			}

			/* if infected list is non empty */
			else
			{
				/* this loop modefies var to pint to last node */
				while(1)
				{
					/* if is the last element, the loop breaks */
					if (var->next==NULL)
						break;

					/* var moves to next element */
					var=var->next;
				}

				/* var->next is set */
				var->next=head;

				/* head->previous is set */
				head->previous=var;
			}

			/* infected count increases */
			++infected_count;

			/* susceptible count decreases */
			--susceptible_count;
		}
	}

}
/* function susceptible_to_infected ends here */


/* This function counts transmission day */
int calculate_trasmission_day()
{
	/* transmission_day is declared */
	int transmission_day=1;

	/* this loop tosses a coin unless we get a heads */
	while(1)
	{
		/* probability 0.5 */
		if (rand()%100<tau)
			break;

		/* probability 0.5 */
		else
			++transmission_day;
	}
	
	/* transmission day is returned */
	return (transmission_day);
}
/* function calculate_transmission_day ends here */


/* This function calculates the recovery_day */
int calculate_recovery_day()
{
	/* recover_day and n are declared */
	int recovery_day=0;

	/* this loop counts the days unless it's a head */
	while(1)
	{
		/* probability 0.2 */
		if (rand()%100<gamma)
			break;
		/* probability 0.8 */
		else
			++recovery_day;
	}
	
	/* recovery_day is returned */
	return (recovery_day);
}
/* function calculate_recovery_day ends here */


/* This functions is called when a node is to be recovered */
void process_rec_SIR(void)
{
	/* concerned_node stores the first event node index */
	int concerned_node=Events[1].node_index;

	/* removing the node from infected list, adding it in recovered list */
	infected_to_recovered(concerned_node);

	/* first event is deleted */
	delete_first_event();
}
/* function precess_rec_SIR ends here */


/* This function shifts a node from infected list to recovered list */
void infected_to_recovered(int key_no)
{
	/* struct node* head,temp1 and temp2 is declared */
	struct node* head;
	
	struct node* temp1;
	
	struct node* temp2;

	/* head is assigned the first node in Infected list */
	head=Infected->first;
	
	/* printing Caution */
	if (head==NULL)
		printf("Caution2!\n");

	/* if list is non-empty */
	if (head!=NULL)
	{
		/* this loop matches the key_no with every node */
		while (head!=NULL)
		{
			/* if match is found the loop breaks */
			if (head->key==key_no)
				break;

			/* head moves to next element */
			head=head->next;
		}

		/* in case match is found */
		if (head!=NULL)
		{
			/* temp1 points to head previous */
			temp1=head->previous;

			/* temp2 points to head next */
			temp2=head->next;

			/* if first node is matched */
			if (temp1==NULL)
			{
				/* temp2 then becomes the new first */
				Infected->first=temp2;
				
				/* if the only node present matches */
				if (temp2!=NULL)
					temp2->previous=NULL;
			}

			/* if the matched node is not the first */
			else
			{
				/* temp1->next points to temp2 */
				temp1->next=temp2;
				
				/* if last node is matched */
				if (temp2!=NULL)
					temp2->previous=temp1;
			}

			/* head->next, head->previous are set to NULL */
			head->next=NULL;
			
			head->previous=NULL;

			/* var is declared */
			struct node* var;

			/* var points to Recovered->first */
			var=Recovered->first;

			/* if no node is present in recovered list */
			if (var==NULL)
			{
				/* Recovered first is set */
				Recovered->first=head;
			}

			/* this loop modefies var to point to the last element in list */
			else
			{
				/* this loop modefies var */
				while (1)
				{
					/* when the last element is reached, the loop breaks */
					if (var->next==NULL)
						break;

					/* var moves to next element */
					var=var->next;
				}

				/* var->next is set */
				var->next=head;

				/* head->previous is set */
				head->previous=var;
			}
			
			/* infected_count increases */
			-- infected_count;

			/* recovered_count increases */
			++recovered_count;
		}
	}
}
/* infected to recovered ends here */


/* This function deletes the first event from the queue */
void delete_first_event()
{
	/* first and last event is exchanged */
	exchange(1,event_count);
	
	/* event_count decreases */
	--event_count;

	/* min_heapify is invoked */
	min_heapify(1);
}
/* function delete_first_event ends here */


/* This function sorts the queue according to priorities */ 
void sort_events()
{
	/* index is declared */
	int index;

	/* calls min_heapify for each node that has a child */
	for (index=event_count/2;index>=1;--index)
		/* invokes min_heapify */
		min_heapify(index);
}
/* function sort_events starts here */


/* This function modefies the array according to priorities */
void min_heapify(int index)
{
	/* left index is found */
	int left=2*index;

	/* right index is found */
	int right=2*index+1;

	/* min is declared */
	int min;

	/* left is compared with the node, minimun is set as min */
	if (left<=event_count && Events[left].event_day< Events[index].event_day)
		min=left;
	else
		min=index;

	/* min is compared with right and min is modefied if necessary */
	if (right<=event_count && Events[right].event_day<Events[min].event_day)
		min=right;

	/* if min different than index */
	if (min!=index)
	{
		/* min and index is exchanged */
		exchange(min,index);

		/* min_heapify is called */
		min_heapify(min);
	}
}
/* function min_headpify end here */


/* This function exchanges the contents of two events present in queue */
void exchange(int index1,int index2)
{
	/* declaration of swap_day,swap_type,swap_node_index */
	int swap_day;
	
	int swap_type;
	
	int swap_node_index;

	/* swap_day stores a value temporarily */
	swap_day=Events[index1].event_day;
	
	/* swap_events stores a value temporarily */
	swap_type=Events[index1].event_type;

	/* swap_node_index stores a value temporarily */
	swap_node_index=Events[index1].node_index;

	
	/* Events[index] stores the values of Events[index2] */
	Events[index1].event_day=Events[index2].event_day;
	
	Events[index1].event_type=Events[index2].event_type;
	
	Events[index1].node_index=Events[index2].node_index;

	
	/* values stored in all the swap variables are assigned to Events[index2] */
	Events[index2].event_day=swap_day;
	
	Events[index2].event_type=swap_type;
	
	Events[index2].node_index=swap_node_index;
}
/* function exchange ends here */


/* This function prints the events present in priority queue */ 
void print_events()
{
	/* index is declared */
	int index;

	/* even_count is printed */
	printf("event_count=%d\n",event_count);

	/* this loop prints all the events present in the priority queue */
	for (index=1;index<=event_count;++index)
		printf("%d %d %d\n",Events[index].event_day,Events[index].event_type,Events[index].node_index);
}
/* function print_events ends here */


/* This function prints the status of the population after each day */
void print_status()
{
	/* The string is printed */
	printf("\ncounts at the end of day %d:\n",day_count);
	
	/* susceptible_count, infected_count and recovered_count are printed */
	printf("S:%d, I:%d, R:%d\n\n",susceptible_count,infected_count,recovered_count);
}
/* function print_status ends here */ 


/* This a a graph generator that follows the method as mentioned in the document */
void create_graph()
{

	/* the no. of verices is assigned */
	int numberOfVertices = MAX_VERTICES; 
	
	/* maximum no. of edges is determined */
	maxNumberOfEdges = rand() % MAX_EDGES;

    /*graphs is 2 dimensional array of pointers*/
	if(maxNumberOfEdges==0)
    	maxNumberOfEdges=1;

    /*generate a dynamic array of random size*/
	Graph=(struct vertex*)malloc(sizeof(struct vertex)* MAX_VERTICES);
     
    /*generate space for edges*/
    int vertexCounter = 0;

    /*generate space for vertices*/
	int edgeCounter = 0;
	
	/* this loop assignes each Node a no. and alloctes memory for Node_ptr */	
	for (vertexCounter = 0; vertexCounter < numberOfVertices; vertexCounter++)
    {
		Graph[vertexCounter].Node=vertexCounter+1;
		Graph[vertexCounter].Node_ptr=(int**)malloc(sizeof(int*)*maxNumberOfEdges);
	}

	/* vertex counter and edge counter is assigned to 0 */
    vertexCounter = 0;
    edgeCounter = 0;

    /* this loop creates the graph by setting the pointer to point to Nodes */
    for (vertexCounter = 0; vertexCounter < numberOfVertices; vertexCounter++)
    {
		for (edgeCounter=0; edgeCounter < maxNumberOfEdges; edgeCounter++)
        {
			if (rand()%2 == 1)	/*link the vertices*/
			{ 
				int linkedVertex = rand()%numberOfVertices;
				Graph[vertexCounter].Node_ptr[edgeCounter] = &(Graph[linkedVertex].Node);

            }
            else	/*make the link NULL*/
            { 
				Graph[vertexCounter].Node_ptr[edgeCounter] = NULL;
			}
		}
	}
}
/* function create graph ends */


/* This function prints the graph if the user invoekes it */
void printGraph()
{
	/* vertexCounter and edgeCounter are declared */
	int vertexCounter;
	
	int edgeCounter;
	

	/* This loop prints the Graph */
	for (vertexCounter=0;vertexCounter<MAX_VERTICES;++vertexCounter)
	{
		/* Base Node is printed */
		printf("%d:\t",Graph[vertexCounter].Node);

		/* This loop prints the Nodes that the edges points to */
		for (edgeCounter=0;edgeCounter<maxNumberOfEdges;++edgeCounter)
		{
			/* if the pointer is not NULL, the Node it points to is printed */
			if (Graph[vertexCounter].Node_ptr[edgeCounter]!=NULL)
			printf("%d ",*(Graph[vertexCounter].Node_ptr[edgeCounter]));
		}

		/* new line is printed */
		printf("\n");
	}
}
/* function printGraph ends here */


/* This function prints the Susceptible, Infected and Recovered list */
void printList()
{
	/* struct node* head is declared */
	struct node* head;

	/* head points to the first node in Susceptible list */
	head=Susceptible->first;

	/* prints the string */
	printf("Susceptible: ");

	/* if list is empty */
	if (head==NULL)
	{
		/* NULL is printed */
		printf("NULL");	
	}

	/* if list is non-empty */
	else
	{
		/* every node->key is printed */
		while(head!=NULL)
		{
			printf("%d ",head->key);

			/* head moves to next node */
			head=head->next;
		}
	}

	/* new line is printed */
	printf("\n");

	/* head points to the first node of Infected list */
	head=Infected->first;

	/* prints the string */
	printf("Infected: ");

	/* if list is empty */
	if (head==NULL)
	{
		/* prints the string */
		printf("NULL");
	}

	/* if list is non-empty */
	else
	{
		/* every node->key is printed */
		while(head!=NULL)
		{
			printf("%d ",head->key);

			/* head moves to next node */
			head=head->next;
		}
	}

	/* prints new line */
	printf("\n");


	/* head points to first node in Recovered list */
	head=Recovered->first;

	/* the srting is printed */
	printf("Recovered: ");
	
	/* is list is empty */
	if (head==NULL)
	{
		/* string is printed */
		printf("NULL");
	}

	/* if the string is non-empty */
	else
	{
		/* every node->key is printed */
		while(head!=NULL)
		{
			printf("%d ",head->key);
			
			/* head moves to next node */
			head=head->next;
		}
	}

	/* prints new line */
	printf("\n");
}
/* function printList ends here */