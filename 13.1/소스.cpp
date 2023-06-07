#include <cstdio>
#include <cmath>
void PrintTree2Matrix(int** M, int* bTree, int size, int idx, int col, int row, int height)
{
	if (idx >= size) return;
	M[row][col] = bTree[idx];
	PrintTree2Matrix(M, bTree, size, idx * 2 + 1, col - pow(2, height - 2), row + 1, height - 1);
	PrintTree2Matrix(M, bTree, size, idx * 2 + 2, col + pow(2, height - 2), row + 1, height - 1);
}

void TreePrinter(int* bTree, int size)
{
	int h = (int)ceil(log2(size + 1));
	int col = (1 << h) - 1;
	int** M = new int* [h];
	for (int i = 0; i < h; i++) {
		M[i] = new int[col];
	}
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < col; j++) {
			M[i][j] = 0;
		}
	}
	for (int j = 0; j < col; j++) {
		printf("==");
	}
	printf("\n");
	PrintTree2Matrix(M, bTree, size, 0, col / 2, 0, h);
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < col; j++) {
			if (M[i][j] == 0)
				printf("  ");
			else
				printf("%2d", M[i][j]);
		}
		printf("\n");
	}
	for (int j = 0; j < col; j++) {
		printf("==");
	}
	printf("\n");
}

int Query_Sum(int* seg_tree, int start, int end, int q_s, int q_e, int current);
int Construct_Tree(int* list, int start, int end, int* seg_tree, int current);
//1) 입력    리스트의    배열을    세그먼트    트리로    구성
int* SegTree_Const(int* list, int n, int* seg_size)
{
	int height = (int)ceil(log2(n));
	int t_size = 2 * pow(height, 2);
	int* seg_tree = (int*)malloc(sizeof(t_size));
	Construct_Tree(list, 0, n - 1, seg_tree, 0);
	*seg_size = t_size;
	return seg_tree;
}
int Construct_Tree(int* list, int start, int end, int* seg_tree, int current)
{
	if (start == end)
	{
		seg_tree[current] = list[start];
		return list[start];
	}
	int mid = start + (end - start) / 2;
	int child = 2 * current;
	seg_tree[current] = Construct_Tree(list, start, mid, seg_tree, child + 1) + Construct_Tree(list, mid + 1, end, seg_tree, child + 2);
	return seg_tree[current];
}
//2) 세그먼트    트리를    사용하여    질의    범위의    합    계산
int Get_Query(int* seg_tree, int n, int q_s, int q_e)
{
	int current = 0;
	if (q_s < 0 || q_e > n - 1 || q_e < q_s) exit(1);
	int sum = Query_Sum(seg_tree, 0, n - 1, q_s, q_e, current);
	return sum;
}
int Query_Sum(int* seg_tree, int start, int end, int q_s, int q_e, int current)
{
	if (q_s > end || q_e < start) return 0;
	if (q_s <= start && q_e >= end) return seg_tree[current];

	int mid = start + (end - start) / 2;
	int child = 2 * current;
	return Query_Sum(seg_tree, start, mid, q_s, q_e, child + 1) + Query_Sum(seg_tree, mid + 1, end, q_s, q_e, child + 2);
}
//3) 입력    리스트의    배열이    변경될    경우    세그먼트    트리를    갱신
void SegTree_Update(int* seg_tree, int start, int end, int i, int d_value, int current)
{
	if (i > end || i < start) return;
	seg_tree[current] = seg_tree[current] + d_value;
	if (start != end)
	{
		int mid = start + (end - start) / 2;
		int child = 2 * current;
		SegTree_Update(seg_tree, start, mid, i, d_value, child + 1);
		SegTree_Update(seg_tree, mid + 1, end, i, d_value, child + 2);
	}
}
int main() {
	int arr[] = { 21, 7, 8, 19, 2, 9, 6, 10 };
	int n = sizeof(arr) / sizeof(int);  //입력    배열의    크기
	int* seg_tree = NULL;  //세그먼트    트리   배열
	int seg_size = 0;       //세그먼트    트리   배열의    크기
	seg_tree = SegTree_Const(arr, n, &seg_size);  //세그먼트    트리    생성    후    반환 

	TreePrinter(seg_tree, seg_size);        //트리    출력
	printf("[%d ~ %d] 부분 합: %d\n", 0, 7, Get_Query(seg_tree, n, 0, 7));
	printf("[%d ~ %d] 부분 합: %d\n", 1, 3, Get_Query(seg_tree, n, 1, 3));
	printf("[%d ~ %d] 부분 합: %d\n", 2, 5, Get_Query(seg_tree, n, 2, 5));
	SegTree_Update(seg_tree, 0, n - 1, 3, -9, 0);  //3번째    값을    -9
	TreePrinter(seg_tree, seg_size);
	return 0;
}