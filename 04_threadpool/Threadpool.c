



struct nTask
{
    // 每一个人到营业厅的任务不一样，定义执行的任务
    void (*task_func)(void *arg);
    // 每一个任务都有相应的参数
    void *user_data;

    // 定义一个链表的方式
    struct nTask *prev;
    struct nTask *next;
};


struct nWorker {
    pthread_t threadid;
    struct nWorker *prev;
    struct nWorker *next;
};

typedef struct nManager {
    struct nTask *tasks;
    struct nWorker *workers;

    pthread_mutex_t mutex;
    pthread_cond_t cond;  
}ThreadPool;

// 线程池的回调函数
static void *nThreadPoolCallback(void *arg){

}
// 创建一个线程池
int nThreadPoolCreate(ThreadPool *pool, int nWorker){

}
// 销毁一个线程池
int nThreadPoolDestory(ThreadPool *pool, int nWorker){

}
// 向线程池push一个任务
int nThreadPoolPushTask(ThreadPool *pool, struct nTask *task){

}


