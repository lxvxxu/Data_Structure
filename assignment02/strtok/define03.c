#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

int main() {

	char str[] = "x + y - z + 9 = 1000";
	char* ptr = strtok(str, "+-= ");	//	하나의 인자 뿐 아니라 여러 문자들을	한꺼번에 구분자로 포함 가능
	while (ptr != NULL)
	{
		printf("%s\n", ptr);
		ptr = strtok(NULL, "+-= ");
	}
	return 0;
}
