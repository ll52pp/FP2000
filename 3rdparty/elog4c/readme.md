elog4c
----
简单C语言日志库(easy log for c)

- 功能(support)
    1. 日志等级共支持6个等级, 分别是: **alert, error, warn, info, debug, verbose**
    2. 可通过等级或标签来**过滤日志**


## 用法:

```
#include "elog4c.h"

elog_a("mytag", "hello elog4c, %d", 12);
elog_e("mytag", "hello elog4c, %s", "str");
elog_w("mytag", "hello elog4c, %lf", 12.3);
elog_i("mytag", "hello elog4c, %c", 'x');
elog_d("mytag", "hello elog4c");
elog_v("mytag", "hello elog4c");
```

输出结果
```
A[mytag][main.c:4] hello elog4c, 12
E[mytag][main.c:5] hello elog4c, str
W[mytag][main.c:6] hello elog4c, 12.3
I[mytag][main.c:7] hello elog4c, x
D[mytag][main.c:8] hello elog4c
V[mytag][main.c:9] hello elog4c
```
