#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

int main() {
	char str[] = "See. SeoWisdom is my wife, Everyone.";
	printf("%s", str);

	char* ptr = strchr(str, 'e');
	while (ptr != NULL)
	{
		printf("\n찾는 문자 : %c\n찾은 문자열 : %s\n",*ptr, ptr);
		ptr = strchr(ptr + 1, 'e');
	}
	return 0;
}
