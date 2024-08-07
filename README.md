# 이중 연결 리스트 구현

운영체제 과목 과제로 Priority Queue를 이용한 CPU scheduling 알고리즘을 구현하였다.

하지만 그에 앞서, 포인터를 사용한 이중 연결 리스트를 먼저 구현한 다음 우선순위에 따라서 적절한 위치에 집어넣는 코드를 만들었다.

큐 레벨 1은 1에서 10<br/>
2는 11에서 20<br/>
3은 21에서 30의 Priority 값들을 저장한다.

결과

![image](https://github.com/user-attachments/assets/7c914d84-328b-40bc-8996-59b0173c5f98)

P는 Priority, CP는 Computing time이다.

# Priority & R-R 혼합 CPU Scheduling

단일 코어와 다중 코어에서의 효율적인 알고리즘이 다르지만, 이번 과제에선 단일 코어에서의 스케줄링 알고리즘을 구현하는 것이 목표였다.

우선순위 큐와 라운드 로빈을 섞은 방식을 사용했다. 처음에 우선순위대로 큐에 쌓이고, 시간 할당량을 설정. 할당된 시간이 끝나면 프로세스의 우선순위를 10 증가시키고 재배치 시킨다. 우선순위가 31보다 크면 31로 변경 후 4번 큐로 배치시킨다. 즉, 4번 큐에서는 우선순위가 같으므로 기존의 R-R방식으로 프로세스를 처리한다.


구조
![image](https://github.com/user-attachments/assets/f8105491-afb2-4cd5-a5b8-6cc042b070d1)

![image](https://github.com/user-attachments/assets/25a7b612-ee9b-43e2-96e9-17cadce6af1a)





결과

![image](https://github.com/user-attachments/assets/b70a7675-4bcf-4f0a-ac2f-cb05c3065f6d)





