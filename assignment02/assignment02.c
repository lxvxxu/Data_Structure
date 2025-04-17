#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef char element; // char 스택 데이터 타입

typedef struct stackNode {
    element data;
    struct stackNode* link;
} stackNode;

stackNode* top;

// 스택이 비어있는지 확인
int isEmpty() {
    return top == NULL;
}

// 스택 초기화
void topMakeNull() {
    stackNode* temp;
    while (top != NULL) {
        temp = top;
        top = top->link;
        free(temp);
    }
}

// 스택 push 함수
void push(element item) {
    stackNode* temp = (stackNode*)malloc(sizeof(stackNode));
    temp->data = item;
    temp->link = top;
    top = temp;
}

// 스택 pop 함수
element pop() {
    if (isEmpty()) {
        printf("\n[오류] 스택이 비어 있습니다.\n");
        exit(1);
    }
    stackNode* temp = top;
    element item = temp->data;
    top = temp->link;
    free(temp);
    return item;
}

// 스택 peek 함수 (상단 값 반환)
element peek() {
    if (isEmpty()) {
        printf("\n[오류] 스택이 비어 있습니다.\n");
        exit(1);
    }
    return top->data;
}

// 연산자 우선순위 반환
int precedence(char op) {
    switch (op) {
    case '+': case '-': return 1;
    case '*': case '/': return 2;
    case '^': return 3;
    default: return 0;
    }
}

// 중위 표현식을 후위 표기로 변환
void infixToPostfix(char* infix, char* postfix) {
    int i = 0, k = 0;
    char ch, stackTop;
    int len = strlen(infix);

    for (i = 0; i < len; i++) {
        ch = infix[i];

        // 숫자나 소수점(e 포함)을 후위 표기에 바로 추가
        if (isdigit(ch) || ch == '.' || ch == 'e' || ch == 'E' ||
            (ch == '-' && (i == 0 || infix[i - 1] == '('))) {
            postfix[k++] = ch;
        }
        // '('는 스택에 push
        else if (ch == '(') {
            push(ch);
        }
        // ')'는 스택에서 '('까지 연산자 꺼내기
        else if (ch == ')') {
            while (!isEmpty() && (stackTop = pop()) != '(') {
                postfix[k++] = ' ';
                postfix[k++] = stackTop;
            }
        }
        // 연산자 처리
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^') {
            postfix[k++] = ' ';
            while (!isEmpty() && precedence(peek()) >= precedence(ch)) {
                postfix[k++] = pop();
                postfix[k++] = ' ';
            }
            push(ch);
        }
    }

    // 스택에 남은 연산자를 후위 표기에 추가
    while (!isEmpty()) {
        postfix[k++] = ' ';
        postfix[k++] = pop();
    }
    postfix[k] = '\0'; // 종료 문자 추가
}

// 후위 표기 수식을 계산
double evalPostfix(char* postfix) {
    double stack[100]; // 숫자를 계산할 스택
    int top = -1;
    char* token = strtok(postfix, " ");

    while (token != NULL) {
        // 숫자일 경우 스택에 push
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1])) || strchr(token, '.') != NULL) {
            stack[++top] = atof(token);
        }
        // 연산자일 경우 처리
        else {
            if (top < 1) {
                printf("\n[오류] 후위 표기식에서 피연산자가 부족합니다.\n");
                exit(1);
            }
            double b = stack[top--];
            double a = stack[top--];
            switch (token[0]) {
            case '+': stack[++top] = a + b; break;
            case '-': stack[++top] = a - b; break;
            case '*': stack[++top] = a * b; break;
            case '/': stack[++top] = a / b; break;
            case '^': stack[++top] = pow(a, b); break;
            default:
                printf("\n[오류] 알 수 없는 연산자: %s\n", token);
                exit(1);
            }
        }
        token = strtok(NULL, " ");
    }

    if (top != 0) {
        printf("\n[오류] 후위 표기식에서 피연산자가 남아 있습니다.\n");
        exit(1);
    }

    return stack[top];
}

int main() {
    FILE* inputFile = fopen("input.txt", "r");
    if (inputFile == NULL) {
        printf("input.txt 파일을 열 수 없습니다.\n");
        return 1;
    }

    int numExpressions;
    fscanf(inputFile, "%d\n", &numExpressions);

    char infix[256];
    char postfix[256];
    double result;

    for (int i = 0; i < numExpressions; i++) {
        fgets(infix, 256, inputFile);
        infix[strcspn(infix, "\n")] = '\0'; // 개행 문자 제거

        // 후위 표기식 생성
        infixToPostfix(infix, postfix);

        // 후위 표기식 출력 용 복사본 생성
        char postfixCopy[256];
        strcpy(postfixCopy, postfix);

        // 후위 표기식 평가
        result = evalPostfix(postfixCopy);

        printf("Infix : %s\n", infix);
        printf("Postfix : %s\n", postfix);
        printf("Result : %.2f\n\n", result);
    }

    fclose(inputFile);
    return 0;
}
