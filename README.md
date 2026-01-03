# 🎭 MimicV: A Minimal PagedAttention Simulator

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Language: C99](https://img.shields.io/badge/Language-C99-blue.svg)](https://en.wikipedia.org/wiki/C99)
[![Status: Educational](https://img.shields.io/badge/Status-Educational-green.svg)](#)

**MimicV** 是一個旨在透過 C 語言手作「虛擬記憶體管理系統」來深入理解 **vLLM PagedAttention** 核心原理的教學專案。

---

## 🎯 專案目標

透過從零實現一個用戶態（User-space）的內存分頁器，掌握以下 OS 與 AI 推理關鍵技術： 

- **物理內存抽象**：模擬 GPU 顯存塊（Physical Blocks）。
- **分頁映射 (Paging)**：實現邏輯地址到物理地址的映射表（Page Table）。
- **動態分配**：解決 LLM 推理中 KV Cache 長度不可知導致的內存碎片問題。
- **寫時複製 (Copy-on-Write)**：模擬多束搜索（Beam Search）或並行採樣時的內存共享。
- **交換機制 (Swapping)**：模擬顯存不足時將數據換出到 CPU RAM。

---

## 🏗️ 系統架構 (Roadmap)

### 📍 第一階段：物理層 (The Physical Layer)
- [ ] **Storage Pool**: 使用 C 數組模擬連續顯存。
- [ ] **Block Allocator**: 實現 `allocate` 與 `free` 邏輯，管理空閒塊清單。

### 📍 第二階段：邏輯層 (The Logical Layer)
- [ ] **Page Table Structure**: 建立虛擬索引與物理索引的映射。
- [ ] **Memory Virtualization**: 讓「推理進程」認為它擁有一塊連續的 KV Cache 空間。

### 📍 第三階段：動態管理 (The Manager)
- [ ] **Block Manager**: 仿照 vLLM，根據 Token 生成動態申請新 Block。
- [ ] **Reference Counting**: 為 CoW 機制打基礎。

### 📍 第四階段：高級特性 (Advanced Features)
- [ ] **Copy-on-Write (CoW)**：實作 `fork` 邏輯。
- [ ] **Swap-in/out**：模擬 VRAM 與 DRAM 之間的數據搬運。

---

## 📖 核心概念對照表

| OS 術語 | vLLM 術語 | MimicV 實作組件 |
| :--- | :--- | :--- |
| **Physical Frame** | GPU KV Block | `global_device_memory[ID]` |
| **Virtual Page** | Logical KV Block | `logical_block_id` |
| **Page Table** | Block Table | `PageTableEntry` |
| **malloc/free** | Block Manager | `allocate_physical_block()` |
| **Copy-on-Write** | CoW / Paged Split | `ref_count` + `memcpy` |

---

## 🛠️ 開發環境需求

- **語言**: C99 或以上
- **編譯器**: `gcc` 或 `clang`
- **工具**: `make` (選配)

---

## 🚀 快速開始 (Coming Soon)

```bash
# 尚未發布，敬請期待
git clone https://github.com/your-repo/MimicV.git
cd MimicV
make
```

---

## 📄 授權協議

本專案採用 [MIT License](LICENSE) 授權。