#include <stdio.h>
#include <stdlib.h>
#define tankcapacity_X 9 //suc chua trong binh X
#define tankcapacity_Y 4 // suc chua trong binh Y
#define empty 0
#define goal 6 //muc tieu luong nuoc can dong nuoc
#define Maxlength 100 //su dung cai dat Ngan Xep (Stack)



//khai bao cau truc trang thai
typedef struct{
	int x; //luong nuoc trong binh x
	int y; //luong nuoc trong binh y
}State;

void makeNullState(State *state){  //khoi tao giai doan ban dau
	state->x = 0;
	state->y = 0;
}

void print_State(State state){  //in luong nuoc co trong binh x va binh y
	printf("\n  X: %d  ---  Y: %d",state.x,state.y);
}

int goalCheck(State state){ //kiem tra trang thai goal
	return (state.x == goal || state.y == goal);
}

int max(int a, int b){
	return a>b?a:b;
}

int min(int a, int b){
	return a<b?a:b;
}

int pourWaterFullX(State cur_state, State *result){ //lam day binh X
	if(cur_state.x < tankcapacity_X){
		result->x = tankcapacity_X;
		result->y = cur_state.y;
		return 1; //thuc hien thanh cong
	}
	return 0; //thuc hien that bai
}

int pourWaterFullY(State cur_state, State *result){ //lam day binh Y
	if(cur_state.y < tankcapacity_Y){
		result->x = cur_state.x;
		result->y = tankcapacity_Y;
		return 1; //thuc hien thanh cong
	}
	return 0; //thuc hien that bai
}

int pourWaterEmptyX(State cur_state, State *result){ //lam rong binh X
	if(cur_state.x > 0){
		result->x = empty;
		result->y = cur_state.y;
		return 1; //thuc hien thanh cong
	}
	return 0; //thuc hien that bai
}

int pourWaterEmptyY(State cur_state, State *result){ //lam rong binh Y
	if(cur_state.y > 0){
		result->x = cur_state.x;
		result->y = empty;
		return 1; //thuc hien thanh cong
	}
	return 0; //thuc hien that bai
}

int pourWaterXY(State cur_state, State *result){ //do nuoc tu X sang Y
	if((cur_state.y < tankcapacity_Y) && (cur_state.x > 0)){
		result->x = max(cur_state.x - (tankcapacity_Y - cur_state.y),empty);
		result->y = min((cur_state.x + cur_state.y),tankcapacity_Y);
		return 1; //thuc hien thanh cong
	}
	return 0; //thuc hien that bai
}

int pourWaterYX(State cur_state, State *result){ //do nuoc tu Y sang X
	if((cur_state.y > 0) && (cur_state.x < tankcapacity_X)){
		result->x = min((cur_state.x + cur_state.y),tankcapacity_X);
		result->y = max((cur_state.y - (tankcapacity_X - cur_state.x)),empty);
		return 1; //thuc hien thanh cong
	}
	return 0; //thuc hien that bai
}

//kiem tra 2 trang thai co bang nhau khong
int compareStates(State state1, State state2){
	return (state1.x == state2.x && state1.y == state2.y);
}

//goi cac phep toan trang thai (cac hanh dong)
int call_operator(State cur_state, State *result, int option){
	switch(option){
		case 1: return pourWaterFullX(cur_state,result);
		case 2: return pourWaterFullY(cur_state,result);
		case 3: return pourWaterEmptyX(cur_state,result);
		case 4: return pourWaterEmptyY(cur_state,result);
		case 5: return pourWaterXY(cur_state,result);
		case 6: return pourWaterYX(cur_state,result);
		default: printf("Error calls operators");
			return 0;
	}
}

const char* action[] = {"First State", "Pour Water Full X", "Pour Water Full Y", "Pour Water Empty X", "Pour Water Empty Y", "Pour Water X To Y", "Pour Water Y To X"};

//khai bao cau truc node
typedef struct Node{
	State state;
	struct Node *Parent;
	int no_function;
}Node;

//khai bao cau truc queue
typedef struct {
	Node *Elements[Maxlength];
	int front, rear;
}Queue;

//khoi tao hang doi rong
void makeNull_Queue(Queue *Q){
	Q->front = -1;
	Q->rear = -1;
}

//kiem tra xem hang doi co rong khong
int empty_Queue(Queue Q){
	return Q.front==-1;
}

//kiem tra hang doi co day khong
int full_Queue(Queue Q){
	return ((Q.rear-Q.front+1)%Maxlength)==0;
}

//tra ve dia chi phan tu dau hang doi
Node *get_Front(Queue Q){
	if(empty_Queue(Q)){
		printf("Queue is empty");
	}else
		return Q.Elements[Q.front];
}

//xoa bo mot phan tu khoi hang doi
void del_Queue(Queue *Q){
	if(!empty_Queue(*Q)){
		if(Q->front == Q->rear){
			makeNull_Queue(Q);
		}else Q->front = (Q->front+1) % Maxlength;
	}
	else printf("Error, Delete");
}

void push_Queue(Queue *Q, Node *x){
	if(!full_Queue(*Q)){
		if(empty_Queue(*Q))
			Q->front = 0;
		Q->rear = (Q->rear + 1)%Maxlength;
		Q->Elements[Q->rear] = x;	
	}else printf("Error, Push");
}

//tim trang thai trong Queue Open/Close:
//muc tieu la kiem tra xem State moi sinh ra co ton tai trong Open/Close Queue hay ch!!
int find_State(State state, Queue openQueue){
	while(!empty_Queue(openQueue)){
		if(compareStates(get_Front(openQueue)->state,state))
			return 1;
		del_Queue(&openQueue);
	}
	return 0;
}

Node *BFS_Algorithm(State state){
	//khai bao 2 Queue Open va Close
	Queue Open_BFS;
	Queue Close_BFS;
	makeNull_Queue(&Open_BFS);
	makeNull_Queue(&Close_BFS);
	//tao nut trang thai cha (nut goc)
	Node *root = (Node*)malloc(sizeof(Node)); //cap phat cho bien root 1 vung nho
	root->state = state;
	root->Parent = NULL;
	root->no_function = 0;
	
	//them nut root(goc) vao Open_BFS
	push_Queue(&Open_BFS,root);
	while(!empty_Queue(Open_BFS)){ //neu Queue Open khac rong thi thuc hien duyet 
		//lay mot nut trong Queue Open ra duyet
		Node *node = get_Front(Open_BFS);
		del_Queue(&Open_BFS); //xoa nut khoi Open_Queue
		push_Queue(&Close_BFS,node); //them nut vua lay ra dem vao Close_Queue
		//kiem tra nut vua lay ra co phai trang thai GOAL chua
		if(goalCheck(node->state))
			return node;  //neu no la GOAL thi tra ve dia chi cua nut GOAL
		int opt;
		//goi cac hanh dong tren trang thai
		for(opt=1;opt<=6;opt++){
			State new_state; //trang thai moi
			makeNullState(&new_state); //khoi tao trang thai moi
			if(call_operator(node->state,&new_state,opt)){
				//neu trang thai moi sinh ra(new_state) da ton tai trong Open/Close Queue thi bo qua
				if(find_State(new_state,Open_BFS) || find_State(new_state,Close_BFS)){
					continue;
				}
				//neu trang thai moi sinh ra(new_state) khong ton tai trong
				//Open/Close Queue thi them (new_state) nay vao Open Queue
				Node *newNode = (Node*)malloc(sizeof(Node));
				newNode->state = new_state;
				newNode->Parent = node;
				newNode->no_function = opt;
				push_Queue(&Open_BFS,newNode);
			}
		}
	}
	return NULL;
}

int act = 0;
void print_Ways(Node *node){
	if(node==NULL){
		return;
	}
	else{
		print_Ways(node->Parent);
		printf("\nAction %d: %s",act, action[node->no_function]);
		print_State(node->state);
		act++;
	} 
}

int main (){
	State cur_state = {0,0};
	Node *p;
	p = BFS_Algorithm(cur_state);
	print_Ways(p);
	return 0;
}


