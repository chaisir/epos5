/*
 * vim: filetype=c:fenc=utf-8:ts=4:et:sw=4:sts=4
 */
#include <inttypes.h>
#include <stddef.h>
#include <math.h>
#include <stdio.h>
#include <sys/mman.h>
#include <syscall.h>
#include <netinet/in.h>
#include <stdlib.h>
#include "graphics.h"

int step=2;
#define buffer_size 10 //缓冲区数量
#define x g_graphic_dev.XResolution/buffer_size
#define y g_graphic_dev.YResolution/step-30
extern void *tlsf_create_with_pool(void* mem, size_t bytes);
extern void *g_heap;

void thread_bubbleSort(void *p);
void thread_insertSort(void *p);
void thread_quickSort(void *p);
void thread_shellSort(void *p);
void thread_bubbleSort_A(void *p);
void thread_bubbleSort_B(void *p);
void thread_control(void *p);
void thread_producer(void *p);
void thread_consumer(void *p);
/**
 * GCC insists on __main
 *    http://gcc.gnu.org/onlinedocs/gccint/Collect2.html
 */
void __main()
{
    size_t heap_size = 32*1024*1024;
    void  *heap_base = mmap(NULL, heap_size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, -1, 0);
	g_heap = tlsf_create_with_pool(heap_base, heap_size);
}

/**
 * 第一个运行在用户模式的线程所执行的函数
 */
//多参数结构体
//struct Control{
//    int tid_A,tid_B;
//};
//
//int mutex[buffer_size];
//int items;
//int space;

void main(void *pv){

    printf("task #%d: I'm the first user task(pv=0x%08x)!\r\n",task_getid(), pv);

    //TODO: Your code goes here
    
    extern void test_allocator();
    test_allocator();
    while(1)
        ;
    task_exit(0);
//    init_graphic(0x143);//进入图形页面
//    int i,j;
//    //create semaphore
//    for(i=0;i<buffer_size;i++){
//        mutex[i]=sem_create(1);
//    }
//
//    items=sem_create(0);
//    space=sem_create(buffer_size);
//
//    //申请线程栈
//    unsigned int stack_size = 1024*1024;
//    unsigned char* stack_producer=(unsigned char *)malloc(stack_size);
//    unsigned char* stack_consumer=(unsigned char *)malloc(stack_size);
//    unsigned char* stack_control=(unsigned char *)malloc(stack_size);
//
//    //申请数组空间并初始化
//    int(*arr)[buffer_size*y]=(int(*)[buffer_size*y])malloc(buffer_size*y*sizeof(int));
////    int(*arr)[buffer_size]=(int(*)[buffer_size])malloc(sizeof(int)*y*buffer_size);
//    srand(time(NULL));
//    for (i=0;i<buffer_size;i++){
//        for(j=0;j<y;j++){
//            arr[i][j]=rand()%x;
//            line(i*x,j*step,x*i+arr[i][j],j*step,RGB(12, 255,251));
//        }
//    }
//
//
//    //创建线程
//    int tid_producer=task_create(stack_producer+stack_size, &thread_producer,(void*)arr);
//    int tid_consumer=task_create(stack_consumer+stack_size, &thread_consumer,(void*)arr);
//
//    struct Control *ctl=(struct Control*)malloc(sizeof(struct Control));
//        ctl->tid_B=tid_consumer;
//        ctl->tid_A=tid_producer;
//    int tid_control=task_create(stack_control+stack_size, &thread_control,(void*)ctl);
//
//    setpriority(tid_producer,10);
//    setpriority(tid_consumer,10);
//    setpriority(tid_control,0);
//
//
//        while(1)
//            ;
//
//    sem_destroy(space);
//    sem_destroy(items);
//    for(i=0;i<buffer_size;i++){
//    sem_destroy(mutex[i]);
//    }
//    exit_graphic();
//    task_exit(0);
}


//    init_graphic(0x143);//进入图形页面
//	int buffer=10;
//    int n=g_graphic_dev.YResolution/step-20;
//    int x=g_graphic_dev.XResolution/buffer;
//    int y=g_graphic_dev.YResolution/step;
//    //申请线程栈
//    unsigned int stack_size = 1024*1024;
//    unsigned char* stack_producer= (unsigned char *)malloc(stack_size);
//    unsigned char* stack_consumer=(unsigned char *)malloc(stack_size);
//    unsigned char* stack_control=(unsigned char *)malloc(stack_size);
//    //定义数组
//    int* arr_bubble_A=(int*)malloc(n*sizeof(int));
//    int* arr_bubble_B=(int*)malloc(n*sizeof(int));
//    srand(time(NULL));//随机数种子
//    //赋初值
//    int i;
//    for (i=0;i<n;i++){
//        arr_bubble_A[i]=rand()%x;
//        arr_bubble_B[i]=rand()%x;
//    }
//
//    //创建线程
//    int tid_producer=task_create(stack_producer+stack_size,&thread_producer,(void*)arr_bubble_A);
//    int tid_consumer=task_create(stack_consumer+stack_size,&thread_consumer,(void*)arr_bubble_B);
//
//    struct Control *thr_control=(struct Control*)malloc(sizeof(struct Control));
//    thr_control->tid_A=tid_bubble_A;
//    thr_control->tid_B=tid_bubble_B;
//
//    int tid_control=task_create(stack_control+stack_size,&thread_control,(void*)thr_control);
//
//
//
//    //设置线程的静态优先级
//    setpriority(tid_bubble_A,10);
//    setpriority(tid_bubble_B,10);
//    setpriority(tid_control,0);
//
//
//    line(0,step*y-15,x,step*y-15,RGB(255,255,255));
//    line(x-10*getpriority(tid_bubble_A),step*y-15,x,step*y-15,RGB(0, 255,0));
//    line(x,step*y-15, 2*x ,step*y-15, RGB(255,255,255));
//    line(x,step*y-15, x+10*getpriority(tid_bubble_B),step*y-15, RGB(255,0,0));
//
//    while(1)
//
//        ;
//    exit_graphic();
//    task_exit(0);

//}



//void thread_bubbleSort_A(void *p){
//
//    int i,j,temp;
//    int *arr = (int *)p;
//    int n=g_graphic_dev.YResolution/step-20;
//    int x=g_graphic_dev.XResolution/2;
//    for(i=0;i<n;i++){
//        line(x-arr[i], step*i,x,step*i, RGB(0, 255,0));
//    }
//
//    for (i=0; i<n-1; i++){    // Bubble up i'th record
//        for (j=n-1; j>i; j--){
//            if(arr[j] < arr[j-1]){
//                temp = arr[j];
//                arr[j] = arr[j-1];
//                arr[j-1] = temp;
//                line(0,step*j,x,step*j, RGB(0, 0, 0));
//                line(x-arr[j],step*j,x,step*j,RGB(0, 255,0));
//                line(0, step*(j - 1), x,step*(j - 1), RGB(0, 0, 0));
//                line(x-arr[j-1],step*(j - 1), x,step*(j - 1), RGB(0, 255,0));
//            }
//        }
//    nanosleep((const struct timespec[]){{0,500000000L}}, NULL);
//    }
//    task_exit(0);
//}
//
//void thread_bubbleSort_B(void *p)
//{
//    int i,j,temp;
//    int *arr = (int *)p;
//    int n=g_graphic_dev.YResolution/step-20;
//    int x=g_graphic_dev.XResolution/2;
//    for(i=0;i<n;i++){
//        line(x, step*i, x+arr[i], step*i, RGB(255,0,0));
//    }
//
//    for (i=0; i<n-1; i++){    // Bubble up i'th record
//        for (j=n-1; j>i; j--){
//            if (arr[j]<arr[j-1]){
//                temp = arr[j];
//                arr[j] = arr[j-1];
//                arr[j-1] = temp;
//                line(x,step*j,2*x,step*j, RGB(0, 0, 0));
//                line(x,step*j, x+arr[j],step*j,RGB(255,0,0));
//                line(x, step*(j-1),2*x,step*(j-1), RGB(0, 0, 0));
//                line(x,step*(j-1), x+arr[j-1],step*(j-1),RGB(255,0,0));
//            }
//        }
//    nanosleep((const struct timespec[]){{0,500000000L}}, NULL);
//    }
//    task_exit(0);
//}

//procucer_thread
//void thread_producer(void *p){
//    int (*arr)[buffer_size*y]=(int(*)[buffer_size*y])p;
//    int buffer_p=0,i,j;
//    srand(time(NULL));
//    while(1){
//        sem_wait(space);
//        sem_wait(mutex[buffer_p]);
//        //清除缓冲区
//        for(j=0;j<y;j++)
//            line(x*buffer_p,step*j,x*(buffer_p+1),step*j,RGB(0,0,0));
//        //produce
//        for (i=0;i<y;i++){
//            arr[buffer_p][i]=rand()%x;
//            line(x*buffer_p,step*i,x*buffer_p+arr[buffer_p][i],step*i, RGB(255,100,100));//画线
//            nanosleep((const struct timespec[]){{0,5000000L}}, NULL);
//        }
//        sem_signal(mutex[buffer_p]);
//        sem_signal(items);
//        buffer_p++;
//        if(buffer_p==buffer_size)
//            buffer_p=0;
//    }
//}
//
////control_thread
//void thread_control(void *p){
//    struct Control* ctl= (struct Control*)p;
//    line(0,step*(y+30)-20,10*x,step*(y+30)-20,RGB(0,0,0));
//    line(0,step*(y+30)-20,20*getpriority(ctl->tid_A),step*(y+30)-20,RGB(255,0,0));
//    line(0,step*(y+30)-10,10*x,step*(y+30)-10,RGB(0,0,0));
//    line(0,step*(y+30)-10,20*getpriority(ctl->tid_B),step*(y+30)-10,RGB(0,255,0));
//
//    while(1){//循环键入
//        int keyboard= getchar();
//        switch(keyboard){
//            case 0x4d00:{//RIGHT
//                line(0,step*(y+30)-10,10*x,step*(y+30)-10,RGB(0,0,0));
//                setpriority(ctl->tid_B,getpriority(ctl->tid_B)-2);//增加B的优先级
//                line(0,step*(y+30)-10,20*getpriority(ctl->tid_B),step*(y+30)-10,RGB(0,255,0));
//            }break;
//            case 0x4b00:{//LEFT
//                line(0,step*(y+30)-10,10*x,step*(y+30)-10,RGB(0,0,0));
//                setpriority(ctl->tid_B,getpriority(ctl->tid_B)+2);//降低B的优先级
//                line(0,step*(y+30)-10,20*getpriority(ctl->tid_B),step*(y+30)-10,RGB(0,255,0));
//            }break;
//            case 0x4800:{//UP
//                line(0,step*(y+30)-20,10*x,step*(y+30)-20,RGB(0,0,0));
//                setpriority(ctl->tid_A,getpriority(ctl->tid_A)-2);//增加A的优先级
//                line(0,step*(y+30)-20,20*getpriority(ctl->tid_A),step*(y+30)-20,RGB(255,0,0));
//            }break;
//            case 0x5000:{//DOWN
//                line(0,step*(y+30)-20,10*x,step*(y+30)-20,RGB(0,0,0));
//                setpriority(ctl->tid_A,getpriority(ctl->tid_A)+2);//降低A的优先级
//                line(0,step*(y+30)-20,20*getpriority(ctl->tid_A),step*(y+30)-20,RGB(255,0,0));
//            }break;
//        default:break;
//        }
//    }
//
//    task_exit(0);
//}
////第二次实验_线程创建
////    init_graphic(0x143);//进入图形页面：mode=0x143
////    int x=g_graphic_dev.XResolution/4;//水平分辨率
////    int y=g_graphic_dev.YResolution/step;//垂直分辨率
////    srand(time(NULL));//设置随机数种子
////
////    //定义数组
////    int* arr_bubble=(int*)malloc(sizeof(int)*x);
////    int* arr_insert=(int*)malloc(sizeof(int)*x);
////    int* arr_quick=(int*)malloc(sizeof(int)*x);
////    int* arr_shell = (int*)malloc(sizeof(int)*x);
////
////
////    //初始化数组
////    int i;
////    for (i=0;i<y;i++){
////        arr_insert[i]=rand()%x;
////        arr_bubble[i]=rand()%x;
////        arr_shell[i] =rand()%x;
////        arr_quick[i] =rand()%x;
////    }
////
////    //申请线程栈
////    unsigned int  stack_size = 1024*1024;
////    unsigned char* stack_bubble=(unsigned char *)malloc(stack_size);
////    unsigned char* stack_insert=(unsigned char *)malloc(stack_size);
////    unsigned char* stack_quick=(unsigned char *)malloc(stack_size);
////    unsigned char* stack_shell =(unsigned char *)malloc(stack_size);
////
////    //创建线程
////    task_create(stack_bubble+stack_size,&thread_bubbleSort,(void*)arr_bubble);
////    task_create(stack_insert+stack_size,&thread_insertSort,(void*)arr_insert);
////    task_create(stack_quick+stack_size,&thread_quickSort,(void*)arr_quick);
////    task_create(stack_shell+stack_size,&thread_shellSort,(void*)arr_shell);
////
////
////    while(1)
////        ;
////    exit_graphic();//退出图形模式
////    task_exit(0);//退出线程
////}
//
//
////冒泡排序线程函数
//void thread_bubbleSort(void *p){
//    int i,j,temp;
//    int *arr = (int *)p;
//    //int y=  g_graphic_dev.YResolution/step;
//    for(i=0;i<y;i++){
//         line(0, step*i, arr[i], step*i, RGB(255, 0, 0));
//     }
//
//    //冒泡排序
//    for(i=0;i<y-1;i++){
//        for(j=0;j<y-1-i;j++){
//            if(arr[j] > arr[j+1]){
//                temp = arr[j];
//                line(0,step*j, arr[j],step*j, RGB(0, 0, 0));//清除原来的线
//                arr[j] = arr[j+ 1];
//                line(0,step*j, arr[j],step*j,RGB(255,0,0));//重新画线
//                line(0, step*(j + 1), arr[j + 1],step*(j + 1), RGB(0, 0, 0));//清除原来的线
//                arr[j+ 1] = temp;
//                line(0,step*(j + 1), arr[j + 1],step*(j + 1), RGB(255, 0, 0));//重新画线
//             }
//        }
//    nanosleep((const struct timespec[]){{0,10000000L}}, NULL);//睡眠1s
//    }
//     task_exit(0);
//}
//
////插入排序线程函数
//void thread_insertSort(void *p){
//     int i,j,temp;
//     int*arr=(int*)p;
//     //int x = g_graphic_dev.XResolution/4;
//     //int y=  g_graphic_dev.YResolution/step;
//
//   //画排序前数组
//    for(i=0;i<y;i++){
//        line(x, step * i,x+ arr[i],step* i, RGB(0, 255, 0));
//    }
//    //插入排序
//    for (i = 1; i <y; i++){
//        temp = arr[i];
//        for (j = i - 1; j >= 0 && arr[j] > temp;j--){
//            line(x,step * (j + 1), x+arr[j + 1], step * (j + 1), RGB(0, 0, 0));//清除原来的线
//            arr[j + 1] = arr[j];
//            line(x,step * (j + 1), x+arr[j + 1], step * (j + 1), RGB(0,255, 0));//重新画线
//        }
//        line(x, step * (j + 1), x+arr[j + 1],step * (j + 1), RGB(0, 0, 0));//清除原来的线
//        arr[j + 1] = temp;
//        line(x, step* (j + 1), x+arr[j + 1], step * (j + 1), RGB(0, 255, 0));//重新画线
//        nanosleep((const struct timespec[]){{0, 10000000L}}, NULL);
//    }
//    task_exit(0);
//}
//
//
////shell排序线程函数
//void thread_shellSort(void *p){
//    void shellSort(int *A, int n);
//    int i;
//    int*arr = (int*)p;
//    //int x = g_graphic_dev.XResolution / 4;
//    //int y=  g_graphic_dev.YResolution / step;
//
//    for (i = 0; i <y; i++){
//        line(2*x, step* i, 2*x + arr[i],step* i, RGB(0, 255, 255));
//    }
//    shellSort(arr,y);
//    task_exit(0);
//}
//
////shell排序中的插入排序
//void insertsort(int* Arr,int k, int n, int incr){
//    int i, j,temp;
//    //int x = g_graphic_dev.XResolution / 4;
//    for ( i = incr; i< n; i += incr){
//        for ( j = i; (j>= incr) && (Arr[j]<Arr[j - incr]); j-= incr){
//            temp= Arr[j];
//            line(2* x, step*(j+k),2 * x + Arr[j], step*(j+k), RGB(0, 0, 0));//清除原来的线
//            Arr[j] = Arr[j - incr];
//            line(2* x, step* (j +k), 2 * x + Arr[j], step*(j+k), RGB(0, 255, 255));//重新画线
//            line(2 * x, step* (j+k- incr), 2 * x + Arr[j - incr],step*(j+k - incr), RGB(0, 0, 0));//清除原来的线
//            Arr[j - incr] =temp;
//            line(2 * x, step* (j+k- incr), 2 * x + Arr[j - incr],step* (j+k - incr), RGB(0, 255, 255));//重新画线
//        }
//    }
//}
//
////shell排序
//void shellSort(int *Arr, int n){
//    int i, j;
//    void insertsort(int *Arr, int k,int n, int incr);
//    for (i = n / 2; i > 2; i /= 2){
//        for (j = 0; j < i; j++){
//            insertsort(&Arr[j],j, n - j,i);
//            nanosleep((const struct timespec[]){{0, 5000000L}}, NULL);
//        }
//        insertsort(Arr,0, n,1);
//    }
//}
//
////快速排序线程函数
//void thread_quickSort(void *p){
//    int i,temp;
//    int*arr=(int*)p;
//    //int x =g_graphic_dev.XResolution/4;
//    //int y=g_graphic_dev.YResolution/step;
//
//    for(i=0;i<y;i++){
//        line(3*x, step* i,3*x+arr[i], step* i, RGB(0, 0, 255));
//    }
//
//
//   //快速排序
//    int findpivot(int A[], int i, int j){
//        return (i+j)/2;
//    }
//
//    int partition(int A[], int l, int r, int pivot) {
//      do {             // Move the bounds inward until they meet
//        while (A[++l]<pivot);  // Move l right and
//        while ((l < r) && (pivot<A[--r])); // r left
//        //exchange(A[l],A[r]);              // Swap out-of-place values
//        temp=arr[r];
//        line(3 * x,step * r, 3 * x + arr[r], step* r, RGB(0, 0, 0));//清除原来的线
//        arr[r] = arr[l];
//        line(3* x, step* r, 3 * x + arr[r], step* r, RGB(0, 0, 255));//重新画线
//        line(3* x, step*l, 3* x + arr[l], step* l, RGB(0, 0, 0));//清除原来的线
//        arr[l] =temp;
//        line(3* x, step*l,3* x + arr[l], step* l, RGB(0, 0, 255));//重新画线
//      } while (l < r);              // Stop when they cross
//      return l;      // Return first position in right partition
//    }
//
//    void qsort(int arr[], int i, int j) { // Quicksort
//        if (j <= i) return; // Don't sort 0 or 1 element
//        int pivotindex = findpivot(arr, i, j);
//      //exchange(arr[pivotindex],arr[j]);    // Put pivot at end
//        temp=arr[j];
//        line(3 * x,step * j, 3 * x + arr[j], step* j, RGB(0, 0, 0));//清除原来的线
//        arr[j] = arr[pivotindex];
//        line(3* x, step* j, 3 * x + arr[j], step* j, RGB(0, 0, 255));//重新画线
//        line(3* x, step*pivotindex, 3 * x + arr[pivotindex], step* pivotindex, RGB(0, 0, 0));//清除原来的线
//        arr[pivotindex] =temp;
//        line(3 * x, step * pivotindex,3 * x + arr[pivotindex], step* pivotindex, RGB(0, 0, 255));//重新画线
//        // k will be the first position in the right subarray
//        int k = partition(arr, i-1, j, arr[j]);
//        //exchange(arr[k],arr[j]);             // Put pivot in place
//        temp=arr[j];
//        line(3 * x,step * j, 3 * x + arr[j], step* j, RGB(0, 0, 0));//清除原来的线
//        arr[j] = arr[k];
//        line(3* x, step* j, 3 * x + arr[j], step* j, RGB(0, 0, 255));//重新画线
//        line(3* x, step*k, 3 * x + arr[k], step* k, RGB(0, 0, 0));//清除原来的线
//        arr[k] =temp;
//        line(3 * x, step * k,3 * x + arr[k], step* k, RGB(0, 0, 255));//重新画线
//        nanosleep((const struct timespec[]){{0,5000000L}}, NULL);
//        qsort(arr, i, k-1);
//        qsort(arr, k+1, j);
//    }
//
//    qsort(arr,0,y);
//    task_exit(0);
//}
//
//
////consumer_thread
//void thread_consumer(void *p){
//    int (*arr)[buffer_size*y]=(int(*)[buffer_size*y])p;
//    int buffer_c=0;
//    while(1){
//        sem_wait(items);
//        sem_wait(mutex[buffer_c]);
//        //consume
//        int i,j,temp,n=buffer_c;
//        for (i = 1; i <y; i++){
//            temp = arr[n][i];
//            for (j = i - 1; j >= 0 && arr[n][j] > temp;j--){
//                line(x*n,step * (j + 1), x*n+arr[n][j + 1], step * (j + 1), RGB(0, 0, 0));
//                arr[n][j + 1] = arr[n][j];
//                line(x*n,step * (j + 1), x*n+arr[n][j + 1], step * (j + 1), RGB(0,255, 0));
//            }
//            line(x*n, step * (j + 1),x*n+arr[n][j + 1],step * (j + 1), RGB(0, 0, 0));
//            arr[n][j + 1] = temp;
//            line(x*n, step* (j + 1),x*n+arr[n][j + 1], step * (j + 1), RGB(0, 255, 0));
//            nanosleep((const struct timespec[]){{0, 5000000L}}, NULL);
//        }
//        sem_signal(mutex[buffer_c]);
//        sem_signal(space);
//        buffer_c++;
//        if(buffer_c==buffer_size)
//            buffer_c=0;
//    }
//}
//
//
