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
	int seatStatus[200];
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

typedef struct ReservedSeatInfo {
	int seatNumber;
	long long reservationId;
} ReservedSeatInfo;

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

	queue->size++;
}

Node* dequeue(queue* queue) {
	queueNode* temp = queue->front;
	Node* item = temp->item;

	queue->front = temp->next;
	free(temp);

	queue->size--;

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

Node* RBTreeDelete(RBTree* t, long long int key) {
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
	return target;
}

void printTree(Node* root) {
	queue* queue = createQueue();
	enqueue(queue, root);

	int level = 0;
	char lineBuffer[4096];

	while (!isEmpty(queue)) {
		int cnt = getSize(queue);

		int offset = 0;
		offset += sprintf(lineBuffer + offset, "LEVEL %d: ", level + 1);

		for (int i = 0; i < cnt; i++) {
			Node* current = dequeue(queue);

			if (current->reservationId != -1) {
				if (current->color == 'B') {
					offset += sprintf(lineBuffer + offset, "%2lld ", current->reservationId);
				}
				else {
					offset += sprintf(lineBuffer + offset, "\033[31m%2lld \033[0m", current->reservationId);
				}
			}

			if (current->left != NULL) {
				enqueue(queue, current->left);
			}

			if (current->left != NULL) {
				enqueue(queue, current->right);
			}
		}
		sprintf(lineBuffer + offset, "\n");
		printf("%s", lineBuffer);
		level++;
	}
}

int findNode(RBTree* t, int value) {
	Node* current = t->root;

	while (current != t->NIL) {
		int currentSeatNumber = current -> seatNumber;
		if (value == currentSeatNumber) {
			return 1;
		}
		else if (value < currentSeatNumber) {
			current = current->left;
		}
		else {
			current = current->right;
		}
	}

	return 0;
}

long long int findReservationId(RBTree* t, int value) {
	Node* current = t->root;

	while (current != t->NIL) {
		if (value == current->seatNumber) {
			return current->reservationId;
		}
		else if (value < current->seatNumber) {
			current = current->left;
		}
		else {
			current = current->right;
		}
	}

	return -1;
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

void printSeatLayout(movieSchedule* movie) {
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
			int seatIdx = i * 10 + j;
			// if the seat is occupied, print "X"
			if (movie->seatStatus[seatIdx]) {
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
			printSeatLayout(temp);
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
	movie->seatStatus[seatNumber-1] = 1;

	return reservationId;
}

void reservationCancellation(movieSchedule* movie, long long int reservationId) {
	// delete reservation node from rbtree
	Node* deletedNode = RBTreeDelete(movie->reservationinfo, reservationId);
	if (deletedNode != NULL) {
		int seatIdx = deletedNode->seatNumber - 1;
		movie->seatStatus[seatIdx] = 0;
		free(deletedNode);
	}
	// print reservation cancellation information and updates seat layout
	printf("***Your reservation is cancelled successfully!***\n");
	printf("Cancelled Reservation Id: %lld\n", reservationId);
	printTree(movie->reservationinfo->root);
	printSeatLayout(movie);
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

	printSeatLayout(movie);
}

void collectReservedSeats(Node* root, Node* NIL, ReservedSeatInfo* infos, int* count) {
	if (root == NIL) return;

	collectReservedSeats(root->left, NIL, infos, count);
	infos[*count].seatNumber = root->seatNumber;
	infos[*count].reservationId = root->reservationId;
	(*count)++;
	collectReservedSeats(root->right, NIL, infos, count);
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
			

			for (int j = 1; j <= 50; j++) {
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

				groupReservation(movie, j);

				ReservedSeatInfo reservedInfos[200];
				int reservedCount = 0;

				// 단체 예약 취소 (좌석 초기화)
				collectReservedSeats(movie->reservationinfo->root, movie->reservationinfo->NIL, reservedInfos, &reservedCount);

				for (int i = 0; i < reservedCount; i++) {
					reservationCancellation(movie, reservedInfos[i].reservationId);
				}
			}

			
		}
	}
	
    return 0;
}