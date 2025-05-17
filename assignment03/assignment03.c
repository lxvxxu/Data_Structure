#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_KINGS 50
#define NAME_LEN 20

typedef struct King {
    char name[NAME_LEN];
    struct King* father;
    struct King* children[MAX_KINGS];
    int childCount;
} King;

King* kings[MAX_KINGS];
int kingCount = 0;

King* findKing(const char* name) {
    for (int i = 0; i < kingCount; i++) {
        if (strcmp(kings[i]->name, name) == 0)
            return kings[i];
    }
    return NULL;
}

King* createOrGetKing(const char* name) {
    King* k = findKing(name);
    if (k) return k;
    k = (King*)malloc(sizeof(King));
    strcpy(k->name, name);
    k->father = NULL;
    k->childCount = 0;
    kings[kingCount++] = k;
    return k;
}

void buildTree() {
    FILE* fp = fopen("조선왕조.txt", "r");
    if (!fp) {
        perror("파일 열기 실패");
        exit(1);
    }

    char child[NAME_LEN], father[NAME_LEN];
    while (fscanf(fp, "%s", child) != EOF) {
        if (fscanf(fp, "%s", father) == 1) {
            King* f = createOrGetKing(father);
            King* c = createOrGetKing(child);
            c->father = f;
            f->children[f->childCount++] = c;
        }
        else {
            createOrGetKing(child);  // 태조처럼 아버지가 없는 경우
        }
    }
    fclose(fp);
}

void printInOrder() {
    for (int i = 0; i < kingCount; i++) {
        if (!kings[i]->father) {
            King* cur = kings[i];
            while (cur) {
                printf("%s ", cur->name);
                if (cur->childCount > 0)
                    cur = cur->children[0];
                else
                    break;
            }
            printf("\n");
        }
    }
}

void printReverseOrder() {
    for (int i = 0; i < kingCount; i++) {
        if (strcmp(kings[i]->name, "순종") == 0) {
            King* cur = kings[i];
            while (cur) {
                printf("%s ", cur->name);
                cur = cur->father;
            }
            printf("\n");
            return;
        }
    }
}

int countKings() {
    return kingCount;
}

void findDescendants(King* k) {
    for (int i = 0; i < k->childCount; i++) {
        printf("%s ", k->children[i]->name);
        findDescendants(k->children[i]);
    }
}

int hasDescendants(King* k) {
    return k->childCount > 0;
}

int maxDescendantCount() {
    int max = 0;
    for (int i = 0; i < kingCount; i++) {
        if (kings[i]->childCount > max)
            max = kings[i]->childCount;
    }
    return max;
}

int countMultiSuccessorKings() {
    int count = 0;
    for (int i = 0; i < kingCount; i++) {
        int childKings = 0;
        for (int j = 0; j < kings[i]->childCount; j++) {
            if (findKing(kings[i]->children[j]->name)) {
                childKings++;
            }
        }
        if (childKings >= 2)
            count++;
    }
    return count;
}

int generationDistance(King* ancestor, King* descendant) {
    int dist = 0;
    King* cur = descendant;
    while (cur && cur != ancestor) {
        cur = cur->father;
        dist++;
    }
    if (cur == NULL) return -1;
    return dist;
}

void printAncestors(King* k) {
    King* cur = k;
    while (cur) {
        printf("%s ", cur->name);
        cur = cur->father;
    }
    printf("\n");
}

// ========== MAIN ==========
int main() {
    buildTree();

    printf("[Q1] 조선의 왕을 순서대로 출력하시오:\n");
    printInOrder();

    printf("[Q2] 조선의 왕을 역순으로 출력하시오:\n");
    printReverseOrder();

    printf("[Q3] 조선의 왕은 모두 몇 명인가?\n%d명\n", countKings());

    printf("[Q4] 조선의 왕 중에서 인조의 후손은 누구누구인가?\n");
    findDescendants(findKing("인조"));
    printf("\n");

    printf("[Q5] 직계 후손이 왕이 되지 못한 왕은 누구누구인가?\n");
    for (int i = 0; i < kingCount; i++) {
        int hasKingChild = 0;
        for (int j = 0; j < kings[i]->childCount; j++) {
            if (findKing(kings[i]->children[j]->name)) {
                hasKingChild = 1;
                break;
            }
        }
        if (!hasKingChild) {
            printf("%s ", kings[i]->name);
        }
    }
    printf("\n");

    printf("[Q6] 직계 후손이 왕이 된 수가 가장 많은 왕은 누구인가?\n");
    int max = maxDescendantCount();
    for (int i = 0; i < kingCount; i++) {
        if (kings[i]->childCount == max) {
            printf("%s (%d명)\n", kings[i]->name, max);
        }
    }

    printf("[Q7] 정종의 형제로 조선의 왕이 된 사람은 누구인가?\n");
    King* taejong = findKing("태종");
    King* jeongjong = findKing("정종");
    if (taejong && jeongjong && taejong->father == jeongjong->father) {
        printf("%s\n", taejong->name);
    }

    printf("[Q8] 순종의 직계 선조를 모두 출력하시오:\n");
    printAncestors(findKing("순종"));

    printf("[Q9] 직계 후손이 2명 이상 왕이 된 왕은 몇 명인가?\n");
    printf("%d명\n", countMultiSuccessorKings());

    printf("[Q10] 예종은 태종의 몇 대 후손인가?\n");
    int d = generationDistance(findKing("태종"), findKing("예종"));
    if (d != -1) printf("%d대 후손입니다.\n", d);
    else printf("찾을 수 없습니다.\n");

    return 0;
}
