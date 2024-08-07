#include <stdio.h>
#include <malloc.h>
#define Queue_level 3


typedef struct priority_queue* queue_pointer;

struct priority_queue {
	int priority;
	int computing_time;
	queue_pointer left_link, right_link;
};

queue_pointer head[Queue_level];

typedef struct queue_head* head_pointer;

struct queue_head {
	head_pointer left_link;
	head_pointer right_link;
};
void initialize_queue(void);
int insert_queue(int priority, int computing_time);
int delete_queue(int priority);
void print_queue(void);
int level_check(int);

int main(void) {

	initialize_queue();

	insert_queue(1, 10);
	insert_queue(12, 20);
	insert_queue(23, 10);
	insert_queue(8, 20);
	insert_queue(27, 30);
	insert_queue(9, 30);
	insert_queue(25, 20);
	insert_queue(17, 30);
	print_queue();

}


void initialize_queue() {
	int i;
	for (i = 0; i < Queue_level; i++) {
		head[i] = (queue_pointer)malloc(sizeof(struct priority_queue));
		head[i]->left_link = NULL;	
		head[i]->right_link = NULL;
		head[i]->priority = 0;
		head[i]->computing_time = 0;	
		printf("%d번째 quelevel 생성완료\n", i + 1);
	}
}

int insert_queue(int priority, int computing_time) {
	//새로운 노드 생성
	queue_pointer QP = malloc(sizeof(struct priority_queue));
	QP->priority = priority;
	QP->computing_time = computing_time;
	QP->left_link = NULL;
	QP->right_link = NULL;

	int qlevel = level_check(priority);

	if (head[qlevel]->right_link == NULL) { //큐에 아무것도 없고 이 노드가 최초의 입력이면 발생.
		printf("priority %d층의 첫 노드 입력.\n", qlevel+1);
		QP->left_link = head[qlevel];
		QP->right_link = NULL;

		head[qlevel]->right_link = QP;
		head[qlevel]->computing_time = 0;
		head[qlevel]->priority = 0; //헤드는 레디큐 연결하는 포인터 말고 사용 안함.
		printf("value added. priority : %d, CP : %d\n", QP->priority,QP->computing_time);
		return 0;
	}

	else { //큐 안에 기존 노트가 있으면 발생.
		int i = 0;
		queue_pointer cur = head[qlevel];
		while (cur->right_link != NULL) {  //현재 노드의 오른쪽 연결이 없을때까지 실행!
			cur = cur->right_link;
			if (cur->priority < QP->priority) { //현재 노드의 priority가 인서트할 노드의 priority보다 작으면 다음 노드 검색

			}
			else if(cur->priority > QP->priority){
				queue_pointer prev = cur->left_link;  //현재 검색 노드의 이전 노드 포인터 설정.

				QP->right_link = prev->right_link;
				QP->left_link = prev;
				prev->right_link = QP;
				cur->left_link = QP;
				/*
				QP->right_link = head[qlevel]->right_link;
				QP->left_link = head[qlevel];
				head[qlevel]->right_link->left_link = QP;
				head[qlevel]->right_link = QP;*/
				printf("value added. priority : %d, CP : %d\n", QP->priority, QP->computing_time);
				return 0;
			}
		}

		if (cur->right_link == NULL) { //맨뒤에 인서트해야 하는 경우에 사용.
			if (cur->priority < QP->priority) {
				cur->right_link = QP;
				QP->left_link = cur;
				QP->right_link = NULL;
				printf("value added at last. priority : %d, CP : %d\n", QP->priority, QP->computing_time);
			}
		}
	}

	return 0;
}

int level_check(int priority) {
	if (priority >= 1 && priority <= 10) return 0; //priority값따라 0,1,2로 나뉨
	else if (priority >= 11 && priority <= 20) return 1;
	else if (priority >= 21 && priority <= 30) return 2;
	else {
		printf("priority value error\n");
		return -1;
	}
}


void print_queue() {
	printf("출력 실행\n");

	for (int i = 0; i < Queue_level; i++) {
		queue_pointer cur = head[i];
		printf("Queue Level = %d\n", i+1);
		while (head[i]->right_link != NULL) {
			cur = cur->right_link;
			if (cur->right_link == NULL) {
				printf("[P:%2d, CP:%2d] ", cur->priority,cur->computing_time);
				break;
			}
			else {
				printf("[P:%2d, CP:%2d], ", cur->priority, cur->computing_time);
			}
		}
		printf("\n");
	}
}
