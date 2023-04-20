/* postfix.c
 *
 * Data Structures, Homework #5
 * School of Computer Science at Chungbuk National University
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20
/* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum
{
    lparen = 0, /* ( 왼쪽 괄호 */
    rparen = 9, /* ) 오른쪽 괄호*/
    times = 7,  /* * 곱셈 */
    divide = 6, /* / 나눗셈 */
    plus = 5,   /* + 덧셈 */
    minus = 4,  /* - 뺄셈 */
    operand = 1 /* 피연산자 */
} precedence;
char infixExp[MAX_EXPRESSION_SIZE];   /* infix expression을 저장하
  는 배열 */
                                      // infix: 중위 표기법을 의미한다.
char postfixExp[MAX_EXPRESSION_SIZE]; /* postfix로 변경된 문자열을 저
장하는 배열 */
                                      // postfix: 후위 표기법을 의미한다.
char postfixStack[MAX_STACK_SIZE];    /* postfix로 변환을 위해 필요한 스택
                                       */
int evalStack[MAX_STACK_SIZE];        /* 계산을 위해 필요한 스택 */
int postfixStackTop = -1;             /* postfixStack용 top */
int evalStackTop = -1;                /* evalStack용 top */
int evalResult = 0;                   /* 계산 결과를 저장 */
void postfixPush(char x);
char postfixPop();
void evalPush(int x);
int evalPop();
void getInfix();
precedence getToken(char symbol);
precedence getPriority(char x);
void charCat(char *c);
void toPostfix();
void debug();
void reset();
void evaluation();
int main()
{
    char command;
    do
    {
        printf("----------------------------------------------------------------\n");
        printf(" Infix to Postfix, then Evaluation\n");
        printf("----------------------------------------------------------------\n");
        printf(" Infix=i, Postfix=p, Eval=e, Debug=d,Reset=r, Quit=q \n");
        printf("----------------------------------------------------------------\n");
        printf("Command = ");
        scanf(" %c", &command);
        switch (command)
        {
        case 'i':
        case 'I':
            getInfix();
            break;
        case 'p':
        case 'P':
            toPostfix();
            break;
        case 'e':
        case 'E':
            evaluation();
            break;
        case 'd':
        case 'D':
            debug();
            break;
        case 'r':
        case 'R':
            reset();
            break;
        case 'q':
        case 'Q':
            break;
        default:
            printf("\n >>>>> Concentration!! <<<<<\n");
            break;
        }
    } while (command != 'q' && command != 'Q');
    return 1;
}
void postfixPush(char x) // postfix 스택에 x를 추가하는 명령어이다.
{
    postfixStack[++postfixStackTop] = x; // 스택의 top을 1 증가시키고, 맨 위에 멤버변수를 넣는다.
}
char postfixPop() // postfix 스택의 원소를 빼내는 명령어이다.
{
    char x;
    if (postfixStackTop == -1) // top이 -1이면(스택이 비어 있으면),
        return '\0';           // 실행하지 않고 종료한다.
    else
    {
        x = postfixStack[postfixStackTop--]; // 아니면, 맨 위의 값을 저장하고 top을 1 줄인 다음,
    }                                        // 이렇게 되면 스택의 크기가 1 줄어든다.
    return x;                                // 저장한 값을 출력한다.
}
void evalPush(int x) // eval 스택에 x를 추가하는 명령어이다.
{                    // postfix의 스택에 x를 추가하는 과정과 같다.
    evalStack[++evalStackTop] = x;
}
int evalPop() // eval 스택에서 원소을 빼내는 명령어이다.
{             // postfix의 스택에서 원소를 빼내는 과정과 같다.
    if (evalStackTop == -1)
        return -1;
    else
        return evalStack[evalStackTop--];
}
/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix() // infix 표기법을 입력받는 함수이다.
{
    printf("Type the expression >>> ");
    scanf("%s", infixExp); // scanf 함수로 입력받는다.
}
precedence getToken(char symbol) // 입력받은 심볼에 따라 그 심볼의 우선순위를 리턴한다.
{
    switch (symbol)
    {
    case '(': // 연산자들이면, 그에 맞는 우선순위를 가진다.
        return lparen;
    case ')':
        return rparen;
    case '+':
        return plus;
    case '-':
        return minus;
    case '/':
        return divide;
    case '*':
        return times;
    default:
        return operand; // 피연산자는 가장 우선순위가 높으므로, 1을 리턴한다.
    }
}
precedence getPriority(char x) // getToken 함수를 실행시키는 함수이다.
{
    return getToken(x);
}
/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char *c) // 문자 하나를 전달받아서 postfix 문자열에 추가한다.
{
    if (postfixExp == '\0')        // 스택이 비어 있으면,
        strncpy(postfixExp, c, 1); // 그 문자가 그대로 스택에 들어간다.
    else                           // 스택이 어느 정도 차 있으면,
        strncat(postfixExp, c, 1); // 그 문자를 스택 맨 뒤에 추가한다.
}
/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()
{
    /* infixExp의 문자 하나씩을 읽기위한 포인터 */
    char *exp = infixExp;
    char x; /* 문자하나를 임시로 저장하기 위한 변수 */
    /* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
    while (*exp != '\0')
    {
        if (getPriority(*exp) == operand) // 현재 문자가 피연산자이면,
        {
            x = *exp;    // 그 수를 임시 저장하고,
            charCat(&x); // 그 수를 postfix 문자열에 추가한다.
        }
        else if (getPriority(*exp) == lparen) // 현재 문자가 (이면,
        {
            postfixPush(*exp); // (을 무조건 postfix 스택에 추가한다.
                               // 스택에는 연산자들만 저장된다.
        }
        else if (getPriority(*exp) == rparen) // 현재 문자가 )이면,
        {
            while ((x = postfixPop()) != '(') // (가 올 때까지 pop을 반복하여면서,
            {
                charCat(&x); // 나온 값들을 postfix 문자열에 추가한다.
            }
        }
        else // (, ) 가 아닌 연산자인 경우,
        {
            while (getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp))
            {                     // 연산자들을 모아놓은 스택의 맨 위에(우선순위가 가장 높은) 연산자보다
                                  // 현재 가져온 연산자가 우선순위가 낮은 경우,
                x = postfixPop(); // 이 연산자는 바로 스택에서 뽑아내어
                charCat(&x);      // 문자열에 더해준다.
            }                     // 이를 스택의 연산자들의 우선순위가
                                  // 현재 가져온 연산자의 우선순위보다 낮아질 때까지 반복한다.
                                  // 현재 가져온 연산자가 가장 우선순위가 높아진 경우,
            postfixPush(*exp);    // 그 연산자를 스택에 추가한다.
        }
        exp++;
    }
    while (postfixStackTop != -1) // postfix 스택의 top이 -1이 될때까지 반복한다.
    {                             // -> 스택을 다 비울 때까지 반복한다.
        x = postfixPop();         // 남은 연산자들을 뒤에서부터 하나씩 꺼내서,
        charCat(&x);              // 그 값을 문자열에 추가해 준다.
    }
}
void debug() // 상태를 표시하는 함수이다.
{
    printf("\n---DEBUG\n");
    printf("infixExp = %s\n", infixExp);      // 중위 표기법으로 표현한다.
    printf("postExp = %s\n", postfixExp);     // 후위 표기법으로 표현한다.
    printf("eval result = %d\n", evalResult); // 계산 결과를 표시한다.
    printf("postfixStack : ");                // 연산자들을 모아서 표기한다.
    for (int i = 0; i < MAX_STACK_SIZE; i++)
        printf("%c ", postfixStack[i]);
    printf("\n");
}
void reset() // 초기화를 하는 함수이다.
{
    infixExp[0] = '\0'; // 첫 번째 원소를 \0으로 보면, 비었다고 간주한다.
    postfixExp[0] = '\0';
    for (int i = 0; i < MAX_STACK_SIZE; i++)
        postfixStack[i] = '\0'; // 스택의 모든 원소들을 비워준다.

    postfixStackTop = -1; // 각 스택들의 top들도 초기화해준다.
    evalStackTop = -1;
    evalResult = 0;
}
void evaluation()
{
    char *exp = postfixExp; // postfix 표기법으로 표현된 수식을 하나씩 이동하면서 확인한다.
    evalStackTop = -1;
    int oper1, oper2;
    while (*exp != '\0') // postfixExp 스택의 끝까지 이동한다.
    {
        if (getToken(*exp) == operand) // 확인 중인 원소가 피연산자이면,
        {
            evalPush(*exp - '0'); // 그 값을 스택의 맨 뒤에 추가한다.
        }                         // - 연산자를 통해, 문자를 숫자로 만들어 준다!
        else                      // 연산자라면,
        {
            oper1 = evalPop();
            oper2 = evalPop();
            switch (*exp) // 연산자에 맞게,
            {
            case '+': // 스택 맨 끝, 맨 끝에서 2번째 원소를 더한다.
                evalPush(oper1 + oper2);
                break;
            case '-': // 스택 맨 끝, 맨 끝에서 2번째 원소를 뺀다.
                evalPush(oper1 - oper2);
                break;
            case '*': // 스택 맨 끝, 맨 끝에서 2번째 원소를 곱한다.
                evalPush(oper1 * oper2);
                break;
            case '/': // 스택 맨 끝, 맨 끝에서 2번째 원소를 나눠준다.
                evalPush(oper1 / oper2);
                break;
            default:
                break;
            }
        }
        printf("%d, %d, %d", oper1, oper2, postfixStackTop);
        exp++;
    }
    evalResult = evalPop();
    /* postfixExp, evalStack을 이용한 계산 */
}