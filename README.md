# mcu-scheduler

一个mcu任务调度内核，管理mcu的多任务调度。

功能比uc/os或freertos要简单，但更轻量，更容易理解。

基于定时器的任务轮流调用，在有些资料中也被称为“前后台系统”。

### 工作原理
使用1路MCU内置定时器

### 任务类型和优先级
分为定时任务和空闲任务2类。
- 定时任务：设置定时时间，在优先级满足的情况下，按定时时间调用

### 移植方式

### 添加和配置任务