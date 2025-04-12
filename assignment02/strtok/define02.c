char str[] = "Block D Mask.";	// 구분할 문자열
char *ptr = strtok(str, " ");		// 첫 번째 strtok 사용
while (ptr != NULL)	// ptr이 NULL일 때까지 = strtok 함수가 NULL을 return할 때까지
{
	printf("%s\n", ptr);	// 자른 문자 출력
    ptr = strtok(NULL, " ");	// 자른 문자 다음부터 구분자 찾기
}
