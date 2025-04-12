#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

int main() {
	char str[] = "Kim,Park,Lee,Choi,Seo";

	char* ptr = strtok(str, ",");

	while (ptr != NULL)
	{
		printf("%s\n", ptr);
		ptr = strtok(NULL, ",");
	}
	
	return 0;
}
