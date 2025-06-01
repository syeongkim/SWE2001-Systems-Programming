#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 60

typedef struct Node {
	struct Node* parent;
	struct Node* left;
	struct Node* right;
	long long int reservationId;
	int movieId;
	char* startTime;
	char* date;
	int seatNumber;
	char color;
} Node;

typedef struct RBTree {
	struct Node* root;
	struct Node* NIL;
} RBTree;

typedef struct movieSchedule {
	int movieId;
	char* startTime;
	char* date;
	RBTree* reservationinfo;
} movieSchedule;

typedef struct queueNode {
	Node* item;
	struct queueNode* next;
} queueNode;

typedef struct queue {
	queueNode* front;
	queueNode* rear;
	int size;
} queue;

Node* createNode(long long int data) {
	Node* n = (Node*)malloc(sizeof(Node));

	n->parent = NULL;
	n->left = NULL;
	n->right = NULL;
	n->reservationId = data;
	n->movieId = 0;
	n->startTime = NULL;
	n->date = NULL;
	n->seatNumber = 0;
	n->color = 'R';

	return n;
}

RBTree* createRBTree() {
	RBTree* rbtree = (RBTree*)malloc(sizeof(RBTree));
	Node* nilnode = (Node*)malloc(sizeof(Node));

	nilnode->parent = NULL;
	nilnode->left = NULL;
	nilnode->right = NULL;
	nilnode->reservationId = -1; // key of Node
	nilnode->color = 'B';
	rbtree->NIL = nilnode;
	rbtree->root = rbtree->NIL;

	return rbtree;
}

movieSchedule* createMovieSchedule() {
	movieSchedule* n = (movieSchedule*)malloc(sizeof(movieSchedule));
	RBTree* rbtree = createRBTree();

	n->movieId = 0;
	n->startTime = NULL;
	n->date = NULL;
	n->reservationinfo = rbtree;

	return n;
}

queue* createQueue() {
	queue* n = (queue*)malloc(sizeof(queue));
	n->front = NULL;
	n->rear = NULL;
	n->size = 0;

	return n;
}

int isEmpty(queue* queue) {
	return queue->size == 0;
}

int getSize(queue* queue) {
	return queue->size;
}

void enqueue(queue* queue, Node* data) {
	queueNode* temp = (queueNode*)malloc(sizeof(queueNode));

	temp->item = data;
	temp->next = NULL;

	if (isEmpty(queue)) {
		queue->front = temp;
		queue->rear = temp;
	}
	else {
		queue->rear->next = temp;
		queue->rear = temp;
	}

	int currentSize = queue->size;
	currentSize = currentSize + 1;
	queue->size = currentSize;
}

Node* dequeue(queue* queue) {
	queueNode* temp = queue->front;
	Node* item = temp->item;

	queue->front = temp->next;
	free(temp);
	int currentSize = queue->size;
	currentSize = currentSize - 1;
	queue->size = currentSize;

	return item;
}

void leftRotation(RBTree* t, Node* x) {
	Node* y = x->right;
	x->right = y->left;
	if (y->left != t->NIL) {
		y->left->parent = x;
	}
	y->parent = x->parent;
	if (x->parent == t->NIL) {
		t->root = y;
	}
	else if (x == x->parent->left) {
		x->parent->left = y;
	}
	else {
		x->parent->right = y;
	}
	y->left = x;
	x->parent = y;
}

void rightRotation(RBTree* t, Node* y) {
	Node* x = y->left;
	y->left = x->right;
	if (x->right != t->NIL) {
		x->right->parent = y;
	}
	x->parent = y->parent;
	if (y->parent == t->NIL) {
		t->root = x;
	}
	else if (y == y->parent->right) {
		y->parent->right = x;
	}
	else {
		y->parent->left = x;
	}
	x->right = y;
	y->parent = x;
}

Node* Minimum(RBTree* t, Node* x) {
	while (x->left != t->NIL) {
		x = x->left;
	}
	return x;
}

Node* Successor(RBTree* t, Node* x) {
	if (x->right != t->NIL) {
		return Minimum(t, x->right);
	}
	else {
		Node* y = x->parent;
		while ((y != t->NIL) && (x == y->right)) {
			x = y;
			y = y->parent;
		}
		return y;
	}
}

void RBInsertFixup(RBTree* t, Node* z) {
	while (z->parent->color == 'R') {
		if (z->parent == z->parent->parent->left) {
			Node* y = z->parent->parent->right;
			if (y->color == 'R') {
				z->parent->color = 'B';
				y->color = 'B';
				z->parent->parent->color = 'R';
				z = z->parent->parent;
			}
			else {
				if (z == z->parent->right) {
					z = z->parent;
					leftRotation(t, z);
				}
				z->parent->color = 'B';
				z->parent->parent->color = 'R';
				rightRotation(t, z->parent->parent);
			}
		}
		else {
			Node* y = z->parent->parent->left;
			if (y->color == 'R') {
				z->parent->color = 'B';
				y->color = 'B';
				z->parent->parent->color = 'R';
				z = z->parent->parent;
			}
			else {
				if (z == z->parent->left) {
					z = z->parent;
					rightRotation(t, z);
				}
				z->parent->color = 'B';
				z->parent->parent->color = 'R';
				leftRotation(t, z->parent->parent);
			}
		}
	}
	t->root->color = 'B';
}

void RBTreeInsert(RBTree* t, Node* z) {
	Node* y = t->NIL;
	Node* x = t->root;
	while (x != t->NIL) {
		y = x;
		if (z->reservationId < x->reservationId) {
			x = x->left;
		}
		else {
			x = x->right;
		}
	}
	z->parent = y;
	if (y == t->NIL) {
		t->root = z;
	}
	else if (z->reservationId < y->reservationId) {
		y->left = z;
	}
	else {
		y->right = z;
	}
	z->left = t->NIL;
	z->right = t->NIL;
	z->color = 'R';
	RBInsertFixup(t, z);
}

void RBTreeDeleteFixup(RBTree* t, Node* x) {
	Node* w;
	while ((x != t->root) && (x->color == 'B')) {
		if (x == x->parent->left) {
			w = x->parent->right;
			if (w->color == 'R') {
				w->color = 'B';
				x->parent->color = 'R';
				leftRotation(t, x->parent);
				w = x->parent->right;
			}

			if ((w->left->color == 'B') && (w->right->color == 'B')) {
				w->color = 'R';
				x = x->parent;
			}
			else {
				if (w->right->color == 'B') {
					w->left->color = 'B';
					w->color = 'R';
					rightRotation(t, w);
					w = x->parent->right;
				}
				w->color = x->parent->color;
				x->parent->color = 'B';
				w->right->color = 'B';
				leftRotation(t, x->parent);
				x = t->root;
			}
		}
		else {
			w = x->parent->left;
			if (w->color == 'R') {
				w->color = 'B';
				x->parent->color = 'R';
				rightRotation(t, x->parent);
				w = x->parent->left;
			}
			if ((w->left->color == 'B') && (w->right->color == 'B')) {
				w->color = 'R';
				x = x->parent;
			}
			else {
				if (w->left->color == 'B') {
					w->right->color = 'B';
					w->color = 'R';
					leftRotation(t, w);
					w = x->parent->left;
				}
				w->color = x->parent->color;
				x->parent->color = 'B';
				w->left->color = 'B';
				rightRotation(t, x->parent);
				x = t->root;
			}
		}
	}
	x->color = 'B';
}

void RBTreeChange(RBTree* t, Node* u, Node* v) {
	if (u->parent == t->NIL) {
		t->root = v;
	}
	else if (u == u->parent->left) {
		u->parent->left = v;
	}
	else {
		u->parent->right = v;
	}
	v->parent = u->parent;
}

void RBTreeDelete(RBTree* t, long long int key) {
	Node* target = t->root;
	Node* temp = NULL;
	Node* x = NULL;
	char color;

	while (key != target->reservationId) {
		if (target->reservationId > key) {
			target = target->left;
		}
		else {
			target = target->right;
		}
	}
	color = target->color;

	if (target->left == t->NIL) {
		x = target->right;
		RBTreeChange(t, target, target->right);
	}
	else if (target->right == t->NIL) {
		x = target->left;
		RBTreeChange(t, target, target->left);
	}
	else {
		temp = target->right;
		while (temp->left != t->NIL) {
			temp = temp->left;
		}
		color = temp->color;
		x = temp->right;
		RBTreeChange(t, temp, temp->right);
		temp->right = target->right;
		temp->right->parent = temp;
		RBTreeChange(t, target, temp);
		temp->left = target->left;
		temp->left->parent = temp;
		temp->color = target->color;
	}
	if (color == 'B') {
		RBTreeDeleteFixup(t, x);
	}
	free(target);
}

void printTree(Node* root) {
	queue* queue = createQueue();
	enqueue(queue, root);

	int level = 0;

	while (!isEmpty(queue)) {
		int cnt = getSize(queue);

		printf("LEVEL %d: ", level + 1);
		for (int i = 0; i < cnt; i++) {
			Node* current = dequeue(queue);

			if (current->reservationId != -1) {
				if (current->color == 'B') {
					printf("%2lld ", current->reservationId);
				}
				else {
					printf("\033[31m%2lld \033[0m", current->reservationId);
				}
			}

			if (current->left != NULL) {
				enqueue(queue, current->left);
			}

			if (current->left != NULL) {
				enqueue(queue, current->right);
			}
		}
		level++;
		printf("\n");
	}
}

int findNode(RBTree* t, int value) {
	Node* current = t->root;

	while (current != t->NIL) {
		if (value == current->seatNumber) {
			return 1;
		}
		else if (value < current->seatNumber) {
			current = current->left;
		}
		else {
			current = current->right;
		}
	}

	return 0;
}

movieSchedule* findMovieId(movieSchedule*** matrix, int movieId, char* startTime, char* date) {
	for (int i = 0; i < 5; i++) {
		for (int d = 0; d < 7; d++) {
			movieSchedule* temp = matrix[i][d];
			// compare movie Id
			if (temp->movieId == movieId) { 
				// compare start time
				if (strcmp(temp->startTime, startTime) == 0) { 
					// compare date
					if (strcmp(temp->date, date) == 0) {
						return temp;
					}
				}
			}
		}
	}
	return NULL;
}

int movieArr[5] = { 100, 200, 300, 400, 500 };
char* dateArr[7] = { "12/03", "12/04", "12/05", "12/06", "12/07", "12/08", "12/09" };
char* timeArr[5] = { "10:00", "12:00", "15:00", "18:00", "21:00" };

movieSchedule*** makeMovieSchedule(int num, int day) {
	movieSchedule*** movieList = (movieSchedule***)malloc(sizeof(movieSchedule**) * num);

	for (int i = 0; i < num; i++) {
		movieList[i] = (movieSchedule**)malloc(sizeof(movieSchedule*) * day);

		// generate movie schedule randomly
		for (int d = 0; d < day; d++) {
			movieSchedule* n = createMovieSchedule();
			// allocate movie id randomly
			int idx = rand() % 5;
			n->movieId = movieArr[idx];
			n->startTime = strdup(timeArr[i]);
			n->date = strdup(dateArr[d]);
			RBTree* reservationTree = createRBTree();
			n->reservationinfo = reservationTree;
			movieList[i][d] = n;
		}
	}
	return movieList;
}

void printSeatLayout(RBTree* t) {
	printf("---------------SEAT LAYOUT---------------\n");
	for (int i = 0; i < 20; i++) {
		printf("|");
		for (int j = 0; j < 10; j++) {
			printf("%3d", i * 10 + j + 1);
			printf("|");
		}
		printf("\n");
		printf("|");
		for (int j = 0; j < 10; j++) {
			// if the seat is occupied, print "X"
			if (findNode(t, i * 10 + j + 1) == 1) {
				printf(" X ");
				printf("|");
			}
			// if the seat is vacant, print blank.
			else {
				printf("   ");
				printf("|");
			}
		}
		printf("\n");
		printf("-----------------------------------------\n");
	}
}

void printMovieSchedule(movieSchedule*** matrix, int num, int day) {
	printf("-------------------Movie Schedule---------------------\n");
	printf("|   DATE   ");
	for (int d = 0; d < day; d++) {
		printf("|");
		printf("%s", ((movieSchedule*)matrix[0][d])->date);
	}
	printf("|\n");
	printf("------------------------------------------------------\n");
	// print movie id and start time for each time of movie
	for (int i = 0; i < num; i++) {
		printf("|");
		printf(" Movie Id ");
		printf("|");
		for (int d = 0; d < day; d++) {
			printf(" %d ", ((movieSchedule*)matrix[i][d])->movieId);
			printf("|");
		}
		printf("\n");
		printf("|");
		printf("Start Time");
		printf("|");
		for (int d = 0; d < day; d++) {
			printf("%s", ((movieSchedule*)matrix[i][d])->startTime);
			printf("|");
		}
		printf("\n");
		printf("------------------------------------------------------\n");
	}
	// print seat layout of each time of movie
	for (int i = 0; i < num; i++) {
		for (int d = 0; d < day; d++) {
			movieSchedule* temp = ((movieSchedule*)matrix[i][d]);
			printf("Day: %s\n", temp->date);
			printf("Start Time: %s\n", temp->startTime);
			printf("Movie ID: %d\n", temp->movieId);
			printSeatLayout(temp->reservationinfo);
		}
	}
}

long long int reservation(movieSchedule* movie, int movieId, char startTime[], char day[], int seatNumber) {

	long long int reservationId = 0;
	// make reservation id ({month}{day}{start time}{movie id}{seat number})
	int monthInt, dayInt;
	if (sscanf(day, "%d/%d", &monthInt, &dayInt) == 2) {
		reservationId = reservationId + monthInt * 10000000000 + dayInt * 100000000;
	}

	int startTimeInt;
	char* startTimeStr;
	char* nextToken = NULL;
	startTimeStr = strtok_r(startTime, ":", &nextToken);
	if (startTimeStr != NULL) {
		startTimeInt = atoi(startTimeStr);
		reservationId = reservationId + startTimeInt * 1000000;
	}

	reservationId = reservationId + movieId * 1000 + seatNumber;


	// store reservation information in node
	Node* new = createNode(reservationId);
	new->movieId = movieId;
	new->startTime = startTime;
	new->date = day;
	new->seatNumber = seatNumber;

	// insert new reservation information to rbtree
	RBTreeInsert(movie->reservationinfo, new);

	return reservationId;
}

void reservationInput(movieSchedule*** matrix) {
	// user enters reservation information
	int movieId, seatNumber, cnt;
	char startTime[10], day[10];
	printf("Enter Movie Id: ");
	scanf("%d", &movieId);
	printf("Enter Day(MM/DD): ");
	scanf("%9s", day);
	printf("Enter Start Time(HH:MM): ");
	scanf("%9s", startTime);


	// print message if the movie which user finds doesn't exist
	movieSchedule* movie = findMovieId(matrix, movieId, startTime, day);
	if (movie == NULL) {
		printf("Movie doesn't exist");
	}
	// show seat layout and user enter seat number if the movie which user finds exists
	else {
		RBTree* reservationInfo = movie->reservationinfo;
		printTree(reservationInfo->root);
		printSeatLayout(reservationInfo);
		printf("Enter Seat Number: ");
		scanf("%d", &seatNumber);

		if (findNode(movie->reservationinfo, seatNumber) == 1) {
			printf("The seat is already reserved. Choose the menu again.\n");
		}
		else {
			// make a reservation
			long long int reservationId = reservation(movie, movieId, startTime, day, seatNumber);
			printTree(reservationInfo->root);
			// print reservation information and updated seat layout
			printf("***Your reservation is completed successfully!***\n");
			printf("Reservation Id: %lld\n", reservationId);
			printf("Movie Id: %d\n", movieId);
			printf("Day: %s\n", day);
			printf("Start Time: %s\n", startTime);
			printf("Seat Number: %d\n", seatNumber);
			printSeatLayout(movie->reservationinfo);
		}
	}
}

void reservationConfirmation(movieSchedule*** matrix) {
	long long int reservationId;
	int monthInt, dayInt, startTime, movieId, seatNumber;

	// user enters reservation id
	printf("Enter your reservation Id: ");
	scanf("%lld", &reservationId);

	// find detailed information about reservation by reservation id
	seatNumber = reservationId % 1000;
	movieId = (reservationId % 1000000) / 1000;
	startTime = (reservationId % 100000000) / 1000000;
	dayInt = (reservationId % 10000000000) / 100000000;
	monthInt = reservationId / 10000000000;


	char startTimeStr[10], dayStr[10];
	sprintf(startTimeStr, "%d:00", startTime);
	sprintf(dayStr, "%d/%02d", monthInt, dayInt);

	// find movie which user made a reservation
	movieSchedule* movie = findMovieId(matrix, movieId, startTimeStr, dayStr);

	// if reservation id is valid, print reservation information
	if (findNode(movie->reservationinfo, seatNumber) == 1) {
		printf("***Your Reservation Information***\n");
		printf("Reservation Id: %lld\n", reservationId);
		printf("Movie Id: %d\n", movieId);
		printf("Day: %s\n", dayStr);
		printf("Start Time: %s\n", startTimeStr);
		printf("Seat Number: %d\n", seatNumber);
	}
	// if reservation id is invalid, print message
	else {
		printf("Your reservation Id doesn't exist.\n");
	}
}

void reservationCancellation(movieSchedule* movie, long long int reservationId) {
	// delete reservation node from rbtree
	RBTreeDelete(movie->reservationinfo, reservationId);
	// print reservation cancellation information and updates seat layout
	printf("***Your reservation is cancelled successfully!***\n");
	printf("Cancelled Reservation Id: %lld\n", reservationId);
	printTree(movie->reservationinfo->root);
	printSeatLayout(movie->reservationinfo);
}

void reservationCancellationInput(movieSchedule*** matrix) {
	long long int reservationId;
	int monthInt, dayInt, startTime, movieId, seatNumber;

	// user enters reservation id
	printf("Enter your reservation Id: ");
	scanf("%lld", &reservationId);

	// find detailed information about reservation by reservation id
	seatNumber = reservationId % 1000;
	movieId = (reservationId % 1000000) / 1000;
	startTime = (reservationId % 100000000) / 1000000;
	dayInt = (reservationId % 10000000000) / 100000000;
	monthInt = reservationId / 10000000000;


	char startTimeStr[10], dayStr[10];
	sprintf(startTimeStr, "%d:00", startTime);
	sprintf(dayStr, "%d/%02d", monthInt, dayInt);

	// find movie which user made a reservation
	movieSchedule* movie = findMovieId(matrix, movieId, startTimeStr, dayStr);

	printTree(movie->reservationinfo->root);

	// if reservation id is valid, cancel the reservation
	if (findNode(movie->reservationinfo, seatNumber) == 1) {
		reservationCancellation(movie, reservationId);
	}
	// if reservation id is invalid, print the message
	else {
		printf("Your reservation Id doesn't exist. Choose the menu again.\n");
	}
}

void groupReservation(movieSchedule* movie, int groupSize) {
	int seats[200] = {0};

    // 현재 예약된 좌석 체크
    for (int i = 1; i <= 200; i++) {
        if (findNode(movie->reservationinfo, i)) {
            seats[i - 1] = 1;  // 1이면 예약됨
        }
    }

    int found = 0;
    // 한 줄(10석)씩 연속된 groupSize 찾기
    for (int row = 0; row < 20; row++) {
        int consecutive = 0;
        for (int col = 0; col < 10; col++) {
            int seatNum = row * 10 + col + 1;
            if (seats[seatNum - 1] == 0) {
                consecutive++;
                if (consecutive == groupSize) {
                    // 연속된 자리 발견, 예약
                    for (int k = seatNum - groupSize + 1; k <= seatNum; k++) {
                        long long int reservationId = reservation(
                            movie, movie->movieId, movie->startTime, movie->date, k
                        );
                        printf("Group Reservation - Seat %d, Reservation ID: %lld\n", k, reservationId);
                    }
                    found = 1;
                    break;
                }
            } else {
                consecutive = 0;
            }
        }
        if (found) break;
    }

    // 연속된 자리 없으면, 빈 자리에서 아무거나 groupSize만큼 예약
    if (!found) {
        int reserved = 0;
        for (int i = 1; i <= 200 && reserved < groupSize; i++) {
            if (seats[i - 1] == 0) {
                long long int reservationId = reservation(
                    movie, movie->movieId, movie->startTime, movie->date, i
                );
                printf("Group Reservation (Non-contiguous) - Seat %d, Reservation ID: %lld\n", i, reservationId);
                reserved++;
            }
        }
    }
}

void menuselect(movieSchedule*** matrix) {
	int menu;

	// show the menu
	printf("-----Select Menu-----\n");
	printf("1. Check Movie Schedule\n");
	printf("2. Reservation\n");
	printf("3. Reservation Cancellation\n");
	printf("4. Reservation Confirmation\n");
	printf("5. Finish\n");
	printf("choose the menu: ");

	// user enters the menu
	scanf("%d", &menu);


	switch (menu) {
	case 1: // print movie schedule and seat layouts of each movie to user
		printMovieSchedule(matrix, 5, 7);
		menuselect(matrix);
		break;
	case 2: // user enters reservation information and make a reservation
		printMovieSchedule(matrix, 5, 7);
		reservationInput(matrix);
		menuselect(matrix);
		break;
	case 3: // user enters reservation id and cancel the reservation
		reservationCancellationInput(matrix);
		menuselect(matrix);
		break;
	case 4: // user enters reservation id and show reservation information
		reservationConfirmation(matrix);
		menuselect(matrix);
		break;
	case 5: // finish the program
		printf("Movie Reservation System is finished!");
		break;
	default: // user selects menu again
		printf("Wrong input. Choose menu again.");
		menuselect(matrix);
	}
}

int main() {
    movieSchedule*** MovieSchedule = makeMovieSchedule(5, 7);

	
	srand((int)time(NULL));
	
	for (int i = 0; i < 5; i++) {
		for (int d = 0; d < 7; d++) {
			// make movie schedule randomly
			movieSchedule* movie = MovieSchedule[i][d];
			int movieId = movie->movieId;
			char startTime[10];
			strcpy(startTime, movie->startTime);
			char day[10];
			strcpy(day, movie->date);
			
			// randomly generated seats of 30% of entire reservation
			int A[MAX];
			for (int n = 0; n < MAX; n++) {
				A[n] = rand() % 200 + 1;
				for (int j = 0; j < n; j++) {
					if (A[n] == A[j]) {
						n--;
						break;
					}
				}
			}

			// make a reservation of 30% of entire reservation
			for (int n = 0; n < MAX; n++) {
				int seatNumber = A[n];
				//printf("%d", seatNumber);
				long long int reservationId = reservation(movie, movieId, startTime, day, seatNumber);
			}
		}
	}
	
	// show menu to user
    // menuselect(MovieSchedule);
	
	for (int i = 0; i < 5; i++) {
        for (int d = 0; d < 7; d++) {
            movieSchedule* movie = MovieSchedule[i][d];

            // 30명 단체 예약
            groupReservation(movie, 30);

            // 단체 예약 취소 (좌석 초기화)
            for (int seatNum = 1; seatNum <= 200; seatNum++) {
                if (findNode(movie->reservationinfo, seatNum)) {
                    long long int reservationId = reservation(movie, movie->movieId, movie->startTime, movie->date, seatNum);
                    reservationCancellation(movie, reservationId);
                }
            }
        }
	}
    return 0;
}