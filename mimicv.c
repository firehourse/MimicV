#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 區塊大小
#define BLOCK_SIZE 128

// 最大記憶體塊數
#define MAX_BLOCKS 1024
// 最大請求數
#define MAX_REQUESTS 1024
// 限制每個請求最多借取多少塊
#define MAX_BLOCK_PER_REQUEST 16

// 主體記憶體區塊
typedef struct {
    // 先宣告個指標指向記憶體
    unsigned char *blocks;
    // 記憶體區塊數量，避免後面有擴容需求好了
    int block_count;
    // 管理記憶體是否有被占用
    bool bitmap[MAX_BLOCKS];
} memory_pool;


// 分頁表，負責記錄每個請求 借用了哪些記憶體區塊
typedef struct {
    // 分頁表
    int page_table[MAX_REQUESTS][MAX_BLOCK_PER_REQUEST];
    // 空間換時間，紀錄借了多少塊
    int borrow_count[MAX_REQUESTS];
} page_table;


// 初始化記憶體區塊
memory_pool* init_memory_pool() {
    // malloc 餐餐自由配
    memory_pool *pool = (memory_pool*)malloc(sizeof(memory_pool));
    if (pool == NULL) {
        return NULL;
    }
    // 初始化布林
    for (int i = 0; i<MAX_BLOCKS; i++) {
        pool->bitmap[i] = false;
    }
    // 記錄區塊大小
    pool->block_count = MAX_BLOCKS;
    // 分配記憶體區塊
    pool->blocks = (unsigned char*)malloc(BLOCK_SIZE * MAX_BLOCKS);
    return pool;
}

// 初始化分頁表
page_table* init_page_table() {
    page_table *table = (page_table*)malloc(sizeof(page_table));
    if (table == NULL) {
        return NULL;
    }
    // 初始化分頁表
    for (int i = 0; i<MAX_REQUESTS; i++) {
        table->borrow_count[i] = 0;
        for (int j = 0; j<MAX_BLOCK_PER_REQUEST; j++) {
            table->page_table[i][j] = -1;
        }
    }
    return table;
}


