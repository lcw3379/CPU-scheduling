#include <stdio.h>
#include <malloc.h>
#define Queue_level 4
#define priority_up 10
#define Max_priority 31
int time_quantum = 20;
typedef struct priority_queue* queue_pointer;

struct priority_queue {
	int process_id;
	int queue_id;
	int priority;
	int computing_time;
	int arrival_time;
	int ta_time;
	int remain_time;
	queue_pointer left_link, right_link;
};

queue_pointer head[Queue_level];


void initialize_queue(void);
int insert_queue(int type, int process_id, int priority, int computing_time);
int delete_queue(int priority);
void print_queue(void);
int level_check(int);
void scheduling(void);
int move_node(queue_pointer cur);
int ostime = 0;
int insertcount = 0;

float ntt_sum = 0;


int main(void) {

	initialize_queue();

	printf("process id	queue id	priority	computing time	turn around time	OStime	arrival time	Ntt\n");

	insert_queue(0, 1, 25, 80);
	insert_queue(0, 2, 15, 40);
	insert_queue(0, 3, 8, 30);
	insert_queue(1, 0, 0, 0); //insert_queue 입력타입이 1일때 스케줄링 하는걸로.
	insert_queue(0, 4, 12, 10);
	insert_queue(0, 5, 22, 30);
	insert_queue(0, 6, 28, 50);
	insert_queue(1, 0, 0, 0);
	insert_queue(0, 7, 5, 20);
	insert_queue(0, 8, 3, 40);
	insert_queue(0, 9, 13, 60);
	insert_queue(1, 0, 0, 0);
	insert_queue(0, 10, 24, 45);
	insert_queue(-1, 0, 0, 0); //insert_queue입력타입이 -1일때, 레디큐가 전부 빌 때까지 스케줄링 실행.


	float Antt = ntt_sum / (float)10;
	printf("Average Nomalized Turn Around Time : %f\n", Antt);
}


void initialize_queue() {
	int i;

	//priority 층이 4개 -> 각 레디큐들의 헤드가 4개 존재한다는 뜻.
	for (i = 0; i < Queue_level; i++) {
		head[i] = (queue_pointer)malloc(sizeof(struct priority_queue));
		head[i]->left_link = NULL;
		head[i]->right_link = NULL;
		head[i]->priority = 0;
		head[i]->computing_time = 0;
		head[i]->process_id = 0;
		head[i]->ta_time = 0;
		head[i]->arrival_time = 0;
		head[i]->remain_time = 0;
	}
}

int insert_queue(int type, int process_id, int priority, int computing_time) {

	if (type == 1) {
		// 타입1이면 ostime이 time_quantum만큼 증가.
		scheduling();
		while (ostime % time_quantum != 0) { // 만약 입력과정중 timequantum 이내에 끝나는 프로세스가 있다면 실행된 time만큼 time_quantum을 줄이고 다시 실행하기.
			time_quantum = ostime % time_quantum;
			scheduling();
			time_quantum = 20;
		}
		insertcount++;
		return 0;
	}
	else if (type == -1) {
		while (1) {
			scheduling();
			if ((head[0]->right_link == NULL) && (head[1]->right_link == NULL) && (head[2]->right_link == NULL) && (head[3]->right_link == NULL)) return 0; //4개의 레디큐에 아무것도 없으면 종료.
		}
	}

	//새로운 노드 생성
	queue_pointer QP = malloc(sizeof(struct priority_queue));
	QP->priority = priority;
	QP->computing_time = computing_time;
	QP->process_id = process_id;
	QP->left_link = NULL;
	QP->right_link = NULL;
	QP->arrival_time = time_quantum * (insertcount);
	QP->ta_time = 0;
	QP->remain_time = computing_time;

	int qlevel = level_check(priority);
	QP->queue_id = qlevel + 1;


	if (head[qlevel]->right_link == NULL) { //큐에 아무것도 없고 이 노드가 최초의 입력이면 발생.
		QP->left_link = head[qlevel];
		QP->right_link = NULL;

		head[qlevel]->ta_time = 0;
		head[qlevel]->right_link = QP;
		head[qlevel]->computing_time = 0;
		head[qlevel]->priority = 0; //헤드는 레디큐 연결하는 포인터 말고 사용 안함. queue_head 사용 안했음
		return 0;
	}

	else { //큐 안에 기존 노트가 있으면 발생.
		int i = 0;
		queue_pointer cur = head[qlevel];
		while (cur->right_link != NULL) {  //현재 노드의 오른쪽 연결이 없을때까지 실행!
			cur = cur->right_link;
			if (cur->priority < QP->priority) { //현재 노드의 priority가 인서트할 노드의 priority보다 작으면 다음 노드 검색

			}
			else if (cur->priority > QP->priority) {
				queue_pointer prev = cur->left_link;  //현재 검색 노드의 이전 노드 포인터 설정.

				QP->right_link = prev->right_link;
				QP->left_link = prev;
				prev->right_link = QP;
				cur->left_link = QP;
				return 0;
			}

		}
		if (cur->right_link == NULL) { //맨뒤에 인서트해야 하는 경우에 사용.
			if (cur->priority <= QP->priority) {
				cur->right_link = QP;
				QP->left_link = cur;
				QP->right_link = NULL;
				return 0;
			}
		}
	}

	return 0;
}


int level_check(int priority) {
	if (priority >= 1 && priority <= 10) return 0; //priority값따라 0,1,2로 나뉨
	else if (priority >= 11 && priority <= 20) return 1;
	else if (priority >= 21 && priority <= 30) return 2;
	else if (priority == 31) return 3;
	else {
		printf("priority value error\n");
		return -1;
	}
}

void scheduling() {
	queue_pointer cur;
	int i = 0;
	for (i = 0; i < Queue_level; i++) {
		cur = head[i];
		while (head[i]->right_link != NULL) {
			cur = cur->right_link;

			if (cur->right_link == NULL) {  // 현재 노드를 오른쪽으로 반복하여 옮긴 후 오른쪽 링크에 아무것도 없을때 실행. 반복문 break.
				if (cur->remain_time <= time_quantum) { //타임퀀텀 내에 수행 끝나면 실행
					ostime += cur->remain_time;
					cur->ta_time = ostime - cur->arrival_time;// 반환시간 계산.
					cur->left_link->right_link = NULL;  //이중연결리스트에서 해당 노드 삭제. 큐의 마지막 노드이므로 왼쪽연결들만 삭제한다.
					cur->left_link = NULL;	
					float ntt = (float)cur->ta_time / (float)cur->computing_time;
					ntt_sum += ntt;
					printf("%d		%d		%d		%d		%d			%d	%d		%f\n", cur->process_id, cur->queue_id, cur->priority, cur->computing_time, cur->ta_time, ostime, cur->arrival_time,ntt);
					free(cur);
					return;

				}
				else { //타임퀀텀 내에 끝내지 못하면 실행
					ostime += time_quantum;
					cur->remain_time -= time_quantum;
					if (Max_priority - cur->priority <= priority_up) cur->priority = 31; // priority 값을 더할때 31을 넘으면 31로 고정.
					else cur->priority += priority_up;
					move_node(cur);
					return;
				}
			}
			else { 
				if (cur->remain_time <= time_quantum) { //타임퀀텀 내에 수행 끝나면 실행
					ostime += cur->remain_time;
					cur->ta_time = ostime - cur->arrival_time;// 반환시간 계산.
					cur->left_link->right_link = cur->right_link;  //이중연결리스트에서 해당 노드 삭제.
					cur->right_link->left_link = cur->left_link;
					float ntt = (float)cur->ta_time / (float)cur->computing_time ;
					ntt_sum += ntt;
					printf("%d		%d		%d		%d		%d			%d	%d		%f\n", cur->process_id, cur->queue_id, cur->priority, cur->computing_time, cur->ta_time, ostime, cur->arrival_time, ntt);
					free(cur);
					return;

				}
				else { //타임퀀텀 내에 끝내지 못하면 실행
					ostime += time_quantum;
					cur->remain_time -= time_quantum;
					if (Max_priority - cur->priority <= priority_up) cur->priority = 31; // priority 값을 더할때 31을 넘으면 31로 고정.
					else cur->priority += priority_up;
					move_node(cur);
					return;
				}
			}
		}

	}
}

int move_node(queue_pointer QP) {
	if (QP->right_link == NULL) {  //이중연결리스트에서 해당 노드 삭제.
		QP->left_link->right_link = NULL;
		QP->left_link = NULL;
	}
	else {
		QP->left_link->right_link = QP->right_link;  
		QP->right_link->left_link = QP->left_link;
	}

	int qlevel = level_check(QP->priority);
	QP->queue_id = qlevel + 1;

	if (head[qlevel]->right_link == NULL) { //큐에 아무것도 없고 이 노드가 최초의 입력이면 발생.
		QP->left_link = head[qlevel];
		QP->right_link = NULL;

		head[qlevel]->ta_time = 0;
		head[qlevel]->right_link = QP;
		head[qlevel]->computing_time = 0;
		head[qlevel]->priority = 0; //헤드는 레디큐 연결하는 포인터 말고 사용 안함. queue_head 사용 안했음
		return 0;
	}

	else { //큐 안에 기존 노트가 있으면 발생.
		int i = 0;
		queue_pointer cur = head[qlevel];
		while (cur->right_link != NULL) {  //현재 노드의 오른쪽 연결이 없을때까지 실행!
			cur = cur->right_link;
			if (cur->priority < QP->priority) { //현재 노드의 priority가 인서트할 노드의 priority보다 작으면 다음 노드 검색

			}
			else if (cur->priority > QP->priority) {
				queue_pointer prev = cur->left_link;  //현재 검색 노드의 이전 노드 포인터 설정.

				QP->right_link = prev->right_link;
				QP->left_link = prev;
				prev->right_link = QP;
				cur->left_link = QP;
				return 0;
			}

		}
		if (cur->right_link == NULL) { //맨뒤에 인서트해야 하는 경우에 사용.
			if (cur->priority <= QP->priority) {
				cur->right_link = QP;
				QP->left_link = cur;
				QP->right_link = NULL;
				return 0;
			}
		}
	}

}