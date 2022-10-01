// 这个程序将尝试用两种不同的算法实现在一堆数中找到第k个最大者
// 算法1：直接将数据放入数组，使用冒泡排序，按照递减的顺序规整数组，然后返回位置k上的元素。
// 算法2：先将前k个元素，按照递减顺序放入数组，余下元素再逐个读入。新元素效于第k个数组元素时，将会被忽略，否则就放到数组中正确的位置上，并排出一个数组元素，算法终止时，返回第k位置上的元素。

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define NUM_NUMS 10000
#define REPETITION 50

int algorithm_1(int[], int, int);
int algorithm_2(int[], int, int);
void my_quick_sort(int[], int);

int main(void)
{
    // 用来计算时间的特别变量
    clock_t start_1, stop_1, start_2, stop_2;
    int arr[NUM_NUMS] = { 0 };
    int random_number, counter = 0;
    int rank;

    // 形成一个有若干随机数的数组，随机排列
    srand((unsigned)time(NULL));
    while (arr[NUM_NUMS - 1] == 0) {
        random_number = rand() % 30000;
        arr[counter++] = random_number;
    }

    // 打印生成的随机数组
    for (int i = 0; i < NUM_NUMS; i++) {
        if ((i % 10) == 0) {
            printf("\n\t ");
        }
        printf("%10d", arr[i]);
    }

    // 询问想要第几大的数
    do {
        printf("\n\n\t Which greatest number do you want to know? (a number between 1 - %d) ", NUM_NUMS);
        scanf(" %d", &rank);
        getchar();
    } while (rank < 1 || rank > NUM_NUMS);

    // 用不同的算法分别处理，返回结果和处理时间
    start_1 = clock();
    for (int i = 0; i < REPETITION; i++) {
        printf("\n\t Algorithm 1 for %d repetition: %d ", (i+1), algorithm_1(arr, rank, NUM_NUMS));
    }
    stop_1 = clock();
    start_2 = clock();
    for (int i = 0; i < REPETITION; i++) {
        printf("\n\t Algorithm 2 for %d repetition: %d ", (i+1), algorithm_2(arr, rank, NUM_NUMS));
    }
    stop_2 = clock();

    // 打印算法使用的时间
    printf("\n\n\t Algorithm 1 uses time: %lf ", ((double)((stop_1 - start_1) / CLK_TCK)) / (double)REPETITION);
    printf("\n\n\t Algorithm 2 uses time: %lf ", ((double)((stop_2 - start_2) / CLK_TCK)) / (double)REPETITION);

    getchar();
    printf("\n\n");
    return 0;
}

int algorithm_1(int arr[], int rank, int len)
{
    double process_time;
    int ruled_arr[len];
    int temp, swap_count, index;

    // 复制数组，鉴于这里不是复制字符串，最后的复制的字符数量我们不作+1处理
    memcpy(ruled_arr, arr, len * sizeof(int));

    // 冒泡排序
    do {
        swap_count = 0;
        index = 0;
        for (;index < len - 1; index++) {
            temp = ruled_arr[index];
            // 遍历和比较两个相邻的数，把两个之中较大的放在左边
            ruled_arr[index] = (ruled_arr[index] > ruled_arr[index + 1] ? ruled_arr[index] : ruled_arr[index + 1]);
            if (temp != ruled_arr[index]) {
                swap_count += 1;
                // 完成交换
                ruled_arr[index + 1] = temp;
            }
        }
    } while (swap_count); // 记录交换的次数，当遍历一次后交换的次数为零时，说明算法结束

    // 返回算法运算的结果
    return (ruled_arr[rank - 1]);
}

int algorithm_2(int arr[], int rank, int len)
{
    int ruled_arr_[rank];
    double process_time;

    // 只需要将前rank位的元素复制到ruled_arr_中即可
    memcpy(ruled_arr_, arr, rank * sizeof(int));

    // 将ruled_arr_中的元素快速排序
    my_quick_sort(ruled_arr_, rank);

    // 排序完成后将arr中剩余的元素和ruled_arr_中的元素进行比较
    for (int j = rank; j < len; j++) { // [rank --- len-1]
        for (int i = 0; i < rank; i++) { // [0 --- rank-1]
            // 如果存在比 ruled_arr_ 中的元素大的元素，就将该元素放入并排出ruled_arr_最后的一个元素
            if (ruled_arr_[i] < arr[j]) {
                // 需要插入一个元素，该元素右边的元素向右腾挪一个位置，进行比较的元素 i 也要右移
                for (int k = (rank - 1); k > i; k--) { // [i --- rank-1]
                    ruled_arr_[k] = ruled_arr_[k - 1];
                }
                // 插入该元素
                ruled_arr_[i] = arr[j];
                break;
            }
        }
    }

    // 返回算法运算的结果
    return (ruled_arr_[rank - 1]);
}

void my_quick_sort(int arr[], int len)
{
    // 最终返回条件
    if (len <= 1) {
        return;
    }

    // 设定high和low两个指针，还有stand作为一个临时存储单位和分界线
    int *high, *low, stand;

    // stand先取数组第一个元素
    stand = *arr;
    high = arr;
    low = (arr + len - 1);

    while (high < low) {
        //  向左移动low指针，并比较指针的内容是否小于stand，如果大于，则放入high所指向的位置，并将程序流导入high中
        while (high < low) {
            if ((*low) >= stand) {
                (*high) = (*low);
                break;
            }
            else {
                low--;
            }
        }
        // 向右移动high指针，并比较指针内容是否大于stand，如果小于，则放入low所指位置，把程序流导回low中。
        while (high < low) {
            if ((*high) < stand) {
                (*low) = (*high);
                break;
            }
            else {
                high++;
            }
        }
    }

    // 经过上面的循环，high和low两个指针应该指向了数组的同一个位置，stand放入该位置
    (*high) = stand;

    // 递归的将stand之后与之前的数组片段进行快速排序
    my_quick_sort(arr, (high/*low*/ - arr));
    // &stand 代表了指向 stand 的指针，指针和数组名之间相加减，就可以得到数组部分和长度。
    my_quick_sort((arr + (int)(high/*low*/ - arr)) + 1, (len - (int)(high/*low*/ - arr)) - 1);
}