#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.h"

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
    Queue *bitmap;
} memory_pool;


// 分頁表，負責記錄每個請求 借用了哪些記憶體區塊
typedef struct {
    // 分頁表
    int page_table[MAX_REQUESTS][MAX_BLOCK_PER_REQUEST];
    // 空間換時間，紀錄借了多少塊
    int borrow_count[MAX_REQUESTS];
    // 借出總額
    int total_borrow_count;
} page_table;


// 初始化記憶體區塊
memory_pool* init_memory_pool() {
    // malloc 餐餐自由配
    memory_pool *pool = (memory_pool*)malloc(sizeof(memory_pool));
    if (pool == NULL) {
        return NULL;
    }
    pool->bitmap = create_queue(MAX_BLOCKS);
    if (pool->bitmap == NULL) {
        free(pool);
        return NULL;
    }
    // 初始化布林
    for (int i = 0; i<MAX_BLOCKS; i++) {
        enqueue(pool->bitmap,i);
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
    table->total_borrow_count = 0;
    for (int i = 0; i<MAX_REQUESTS; i++) {
        table->borrow_count[i] = 0;
        for (int j = 0; j<MAX_BLOCK_PER_REQUEST; j++) {
            table->page_table[i][j] = -1;
        }
    }
    return table;
}

// 接受調度請求，分配記憶體區塊
bool allocate_block(memory_pool *pool, page_table *table, int request_id, int block_quantity) {
    // 檢查請求是否合法
    if (request_id < 0 || request_id >= MAX_REQUESTS || block_quantity <= 0 || block_quantity > MAX_BLOCK_PER_REQUEST) {
        return false;
    }
    // 檢查記憶體區塊是否足夠
    if (block_quantity > pool->block_count - table->total_borrow_count) {
        return false;
    }
    // 檢查是否超出每個請求的總上限
    if (block_quantity + table->borrow_count[request_id]  > MAX_BLOCK_PER_REQUEST) {
        return false;
    }
    // 分配記憶體區塊
    for (int j = 0; j < block_quantity; j++) {

        // 填坑
        int value =dequeue(pool->bitmap);

        // 紀錄到對應 Request 的 Page Table
        int next_p_idx = table->borrow_count[request_id];
        table->page_table[request_id][next_p_idx] = value;
    
        // 更新計數器
        table->borrow_count[request_id]++;
        table->total_borrow_count++;
    }
    return true;
}

bool free_mem_by_request(memory_pool *pool, page_table *table, int request_id) {
    if (request_id < 0|| request_id >= MAX_REQUESTS) {
        return false;
    }
    int count = table->borrow_count[request_id];
    for (int i = 0; i < count; i++) {
        int block_idx = table->page_table[request_id][i];
        // 回queue
        enqueue(pool->bitmap, block_idx);
        // 清空紀錄
        table->page_table[request_id][i] = -1;
    }
    table->total_borrow_count -= count;
    table->borrow_count[request_id] = 0;
    return true;
}
