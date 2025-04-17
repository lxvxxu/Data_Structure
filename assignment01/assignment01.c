#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

	FILE* file;
	file = fopen("input.txt", "r");

	if (file == NULL) {
		printf("파일을 열 수 없습니다.\n");
		return 1;
	}

	char instr1[100], instr2[100];
	int myfair = 0;

	// 다항식 쌍 개수 읽기
	myfair = atoi(fgets(instr1, sizeof(instr1), file));

	for (int j = 0; j < myfair; j++) {
		fgets(instr1, sizeof(instr1), file);
		fgets(instr2, sizeof(instr2), file);

		char sum[100][2] = { 0, }, sum2[100][2] = { 0, };  // **배열 초기화**
		int index1 = 0, index2 = 0;
		int sum_int[100][2]; // 정수형으로 변환한 배열
		int sum2_int[100][2];

		// **instr1 파싱**
		char* token = strtok(instr1, "+-\n");
		while (token != NULL) {
			int coeff = 0, power = 0;  // 계수와 차수를 저장할 변수
			if (strchr(token, 'x') != NULL) {  // x가 포함된 항 처리
				if (strchr(token, '^') != NULL) {  // n차항 (x^n)
					sscanf(token, "%dx^%d", &coeff, &power);
				}
				else {  // 1차항 (x)
					sscanf(token, "%dx", &coeff);
					power = 1;
				}
			}
			else {  // 상수항 처리
				coeff = atoi(token);
				power = 0;
			}
			sum[index1][0] = power;     // 차수 저장
			sum[index1][1] = coeff;    // 계수 저장
			index1++;
			token = strtok(NULL, "+-\n");  // 다음 항으로 이동
		}

		// **instr2 파싱**
		token = strtok(instr2, "+-\n");
		while (token != NULL) {
			int coeff = 0, power = 0;
			if (strchr(token, 'x') != NULL) {
				if (strchr(token, '^') != NULL) {
					sscanf(token, "%dx^%d", &coeff, &power);
				}
				else {
					sscanf(token, "%dx", &coeff);
					power = 1;
				}
			}
			else {  // 상수항 처리
				coeff = atoi(token);
				power = 0;
			}
			sum2[index2][0] = power;
			sum2[index2][1] = coeff;
			index2++;
			token = strtok(NULL, "+-\n");
		}

		// 1. 문자형을 정수형으로 변환 (기존 동일)
		for (int i = 0; i < index1; i++) {
			sum_int[i][0] = sum[i][0];     // 차수
			sum_int[i][1] = sum[i][1];     // 계수
		}
		for (int i = 0; i < index2; i++) {
			sum2_int[i][0] = sum2[i][0];
			sum2_int[i][1] = sum2[i][1];
		}

		// 2. 차수에 따라 내림차순 정렬 (기존 동일)
		for (int i = 0; i < index1 - 1; i++) {
			for (int j = i + 1; j < index1; j++) {
				if (sum_int[i][0] < sum_int[j][0]) {
					int temp_degree = sum_int[i][0];
					int temp_coeff = sum_int[i][1];
					sum_int[i][0] = sum_int[j][0];
					sum_int[i][1] = sum_int[j][1];
					sum_int[j][0] = temp_degree;
					sum_int[j][1] = temp_coeff;
				}
			}
		}

		for (int i = 0; i < index2 - 1; i++) {
			for (int j = i + 1; j < index2; j++) {
				if (sum2_int[i][0] < sum2_int[j][0]) {
					int temp_degree = sum2_int[i][0];
					int temp_coeff = sum2_int[i][1];
					sum2_int[i][0] = sum2_int[j][0];
					sum2_int[i][1] = sum2_int[j][1];
					sum2_int[j][0] = temp_degree;
					sum2_int[j][1] = temp_coeff;
				}
			}
		}

		// 3. 두 다항식의 합 계산
		int sum_result[100][2];
		int idx1 = 0, idx2 = 0, idx_res = 0;

		while (idx1 < index1 && idx2 < index2) {
			if (sum_int[idx1][0] == sum2_int[idx2][0]) {
				sum_result[idx_res][0] = sum_int[idx1][0];
				sum_result[idx_res][1] = sum_int[idx1][1] + sum2_int[idx2][1];
				idx1++;
				idx2++;
			}
			else if (sum_int[idx1][0] > sum2_int[idx2][0]) {
				sum_result[idx_res][0] = sum_int[idx1][0];
				sum_result[idx_res][1] = sum_int[idx1][1];
				idx1++;
			}
			else {
				sum_result[idx_res][0] = sum2_int[idx2][0];
				sum_result[idx_res][1] = sum2_int[idx2][1];
				idx2++;
			}
			idx_res++;
		}

		while (idx1 < index1) {
			sum_result[idx_res][0] = sum_int[idx1][0];
			sum_result[idx_res][1] = sum_int[idx1][1];
			idx1++;
			idx_res++;
		}

		while (idx2 < index2) {
			sum_result[idx_res][0] = sum2_int[idx2][0];
			sum_result[idx_res][1] = sum2_int[idx2][1];
			idx2++;
			idx_res++;
		}

		// 4. 두 다항식의 곱 계산
		int mul_result[100][2] = { 0 };
		int mul_index = 0;

		for (int i = 0; i < index1; i++) {
			for (int j = 0; j < index2; j++) {
				int power = sum_int[i][0] + sum2_int[j][0];
				int coeff = sum_int[i][1] * sum2_int[j][1];

				// 같은 차수가 합쳐지는 경우 처리
				int found = 0;
				for (int k = 0; k < mul_index; k++) {
					if (mul_result[k][0] == power) {
						mul_result[k][1] += coeff;
						found = 1;
						break;
					}
				}
				if (!found) {
					mul_result[mul_index][0] = power;
					mul_result[mul_index][1] = coeff;
					mul_index++;
				}
			}
		}

		// 5. 출력
		printf("<%d번째 쌍 다항식>\n", j + 1);

		// 합 출력
		printf("합: ");
		for (int i = 0; i < idx_res; i++) {
			if (sum_result[i][1] != 0) {
				printf("%dX^%d ", sum_result[i][1], sum_result[i][0]);
			}
			if (i < idx_res - 1 && sum_result[i + 1][1] > 0)
				printf("+ ");
		}
		printf("\n");

		// 곱 출력
		printf("곱: ");
		for (int i = 0; i < mul_index; i++) {
			if (mul_result[i][1] != 0) {
				printf("%dX^%d ", mul_result[i][1], mul_result[i][0]);
			}
			if (i < mul_index - 1 && mul_result[i + 1][1] > 0)
				printf("+ ");
		}
		printf("\n\n");
	}

	fclose(file);
	return 0;
}
