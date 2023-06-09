/* circularQ.c
 *
 *  Data Structures, Homework #5
 *  School of Computer Science at Chungbuk National University
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4 // 큐의 최대 사이즈는 4이다.

typedef char element; // char형을 가지는 element 자료형을 만든다.
                      // 큰 의미는 없지만, 자료형만으로 큐의 원소임을 알 수 있다.
typedef struct
{
    element queue[MAX_QUEUE_SIZE]; // 4개의 element(char)형 배열을 선언한다.
                                   // 이 공간이 큐가 된다.
    int front, rear;               // front는 맨 앞, rear은 맨 끝을 의미한다.
} QueueType;

QueueType *createQueue();                   // 큐 만들기
int freeQueue(QueueType *cQ);               // 큐 종료하기
int isEmpty(QueueType *cQ);                 // 큐가 비었는지 확인
int isFull(QueueType *cQ);                  // 큐가 가득 찼는지 확인
void enQueue(QueueType *cQ, element item);  // 큐에 원소 더하기
void deQueue(QueueType *cQ, element *item); // 큐에 원소 빼기
void printQ(QueueType *cQ);                 // 큐 출력하기
void debugQ(QueueType *cQ);                 // 큐 상태 보기
element getElement();                       // 원소 외부에서 입력받기

int main(void)
{
    QueueType *cQ = createQueue(); // 원소가 4개인 큐를 생성한다.
    element data;                  // 입력받을(출력될) 데이터를 임시 저장하는 변수 data를 선언한다.
    char command;                  // command 변수를 통해 실행할 함수, 종료 여부를 판단한다.

    do
    {
        printf("[----- [hyaejun mun] [2019015035] -----]");
        printf("\n-----------------------------------------------------\n");
        printf("                     Circular Q                   \n");
        printf("------------------------------------------------------\n");
        printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
        printf("------------------------------------------------------\n");

        printf("Command = ");
        scanf(" %c", &command); // 어떤 함수를 실행할지 or 프로그램을 종료할지를 판단하는 변수
                                // command를 입력받는다.

        switch (command)
        {
        case 'i': // i이거나 I이면,
        case 'I':
            data = getElement(); // 문자를 입력받아서
            enQueue(cQ, data);   // 그것을 큐에 추가한다.
            break;
        case 'd': // d이거나 D이면,
        case 'D':
            deQueue(cQ, &data); // 큐의 가장 앞에 있는 원소를 제거하고 그 값을 data에 추가한다.
            break;
        case 'p': // p이거나 P이면,
        case 'P':
            printQ(cQ); // 큐를 출력한다.
            break;
        case 'b': // b이거나 B이면,
        case 'B':
            debugQ(cQ); // 큐 상태를 출력한다.
            break;
        case 'q': // q이거나 Q이면,
        case 'Q':
            freeQueue(cQ); // 큐를 동적할당 해제해 준다.
            break;
        default: // 이외의 것들이면 실행하지 않는다.
            printf("\n       >>>>>   Concentration!!   <<<<<     \n");
            break;
        }

    } while (command != 'q' && command != 'Q'); // q또는 Q를 받지 않을 경우만 반복한다.

    return 1;
}

QueueType *createQueue()
{
    QueueType *cQ;
    cQ = (QueueType *)malloc(sizeof(QueueType)); // 큐를 동적할당하여 생성한다.
    cQ->front = 0;                               // 큐의 시작점을 0이라 한다.
    cQ->rear = 0;                                // 큐의 종료점을 0이라 한다.
    return cQ;                                   // 큐를 외부에서 사용할 수 있게 큐의 주소를 리턴한다.
}

int freeQueue(QueueType *cQ)
{
    if (cQ == NULL) // 큐가 만들어지지 않았다면
        return 1;   // 그냥 종료한다.
    free(cQ);       // 큐가 생성되었으면 동적할당 종료해 준다.
    return 1;
}

element getElement()
{
    element item;
    printf("Input element = ");
    scanf(" %c", &item); // element를 입력받고,
    return item;         // 그 값을 리턴한다.
}

/* complete the function */
int isEmpty(QueueType *cQ) // 큐가 비었는지 확인하려면, front와 rear가 같은지 확인하면 된다.
{
    if (cQ->front == cQ->rear)       // cQ는 큐의 주소이므로, .가 아닌 ->로 써야 한다.
    {                                // 혹은 (*cQ).front도 가능하다.
        printf("Queue is empty.\n"); // 같다면, 비어 있다다고 말해주고
        return 1;                    // 1(true)을 리턴한다.
    }
    else
        return 0; // 같지 않다면 0(false)를 리턴한다.
}

/* complete the function */
int isFull(QueueType *cQ) // 큐가 다 찼는지 확인하려면, rear가 front 바로 뒤인지 확인하면 된다.
{
    if (cQ->front == ((cQ->rear + 1) % MAX_QUEUE_SIZE)) // 순환 큐이므로, 나머지 연산을 통해 rear가 큐 범위를 넘어가면
    {                                                   // 다시 0이 되도록 해준다.
        printf("Queue is full.\n");                     // 같으면, 다 찼다고 말해주고
        return 1;                                       // 1(true)를 리턴한다.
    }
    else
        return 0; // 같지 않다면 0(false)를 리턴한다.
}

/* complete the function */
void enQueue(QueueType *cQ, element item) // 큐에 원소 추가하기.(맨 뒤)(push)
{
    if (isFull(cQ))
        return; // 이미 다 찼다면, 실행하지 않는다.
    else
    {
        cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE; // rear를 1 더하여(최대 사이즈를 넘어가면 다시 0이 되고)
                                                    // 들어올 원소의 자리를 마련한다.
        cQ->queue[cQ->rear] = item;                 // 들어올 원소를 입력한다.
    }
}

/* complete the function */
void deQueue(QueueType *cQ, element *item) // 큐에 원소 제거하기.(맨 앞)(pop)
{
    if (isEmpty(cQ))
        return; // 큐가 이미 비어 있다면, 실행하지 않는다.
    else
    {
        cQ->front = (cQ->front + 1) % MAX_QUEUE_SIZE; // front를 1 더하여(최대 사이즈를 넘어가면 다시 0이 되고)
                                                      // front가 가리키는 원소 다음부터 원소가 들어오게 한다.
        *item = cQ->queue[cQ->front];                 // front가 가리키고 있는(원래 맨 앞이었던)원소를 item으로 POP한다.
    }
}

void printQ(QueueType *cQ) // 큐 출력하기
{
    int i, first, last;                       // 큐 출력에 필요한 자료형 3개.
                                              // 시작점, 종료점, 출력하는 지점.
    first = (cQ->front + 1) % MAX_QUEUE_SIZE; // 시작점은 front의 다음 원소부터.
    last = (cQ->rear + 1) % MAX_QUEUE_SIZE;   // 종료점은 rear을 넘어가면 종료.

    printf("Circular Queue : [");

    i = first;        // first부터 시작.
    while (i != last) // last와 같아지면 종료한다.
    {
        printf("%3c", cQ->queue[i]);  // 하나씩 출력한다.
        i = (i + 1) % MAX_QUEUE_SIZE; // i를 하나씩 늘려준다(너무 커지면 0으로 돌아가게).
    }
    printf(" ]\n"); // 끝나면 닫아준다.
}

void debugQ(QueueType *cQ) // 큐 상태 보기
{

    printf("\n---DEBUG\n");
    for (int i = 0; i < MAX_QUEUE_SIZE; i++) // 0부터 3까지, 4개 확인함.
    {
        if (i == cQ->front) // front 지점이면,
        {
            printf("  [%d] = front\n", i); // 이곳이 front라고 하고 계속 진행.
            continue;
        }
        printf("  [%d] = %c\n", i, cQ->queue[i]); // 그렇지 않으면, 그냥 원소 출력해줌.
    }
    printf("front = %d, rear = %d\n", cQ->front, cQ->rear); // 큐의 확인이 끝나면, front와 rear의 상태를 출력함.
}
