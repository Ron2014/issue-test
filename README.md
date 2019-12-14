# 测试用例集锦

## test0: isdigit 的参数好奇怪

观察如下函数，为什么 isdigit 传递的参数这么复杂？

```c
    static const char * scanformat(const char *strfrmt, char *form) {
        const char *p = strfrmt;
        while (*p != '\0' && strchr(FLAGS_CHARS, *p) != NULL) p++;  /* skip flags */
        if ((size_t)(p - strfrmt) >= sizeof(FLAGS_CHARS)) {
            Log::Error("invalid format (repeated flags) for %s", strfrmt);
            return nullptr;
        }

        if (isdigit((unsigned char)(*p))) p++;  /* skip width */
        if (isdigit((unsigned char)(*p))) p++;  /* (2 digits at most) */
        if (*p == '.') {
            p++;
            if (isdigit((unsigned char)(*p))) p++;  /* skip precision */
            if (isdigit((unsigned char)(*p))) p++;  /* (2 digits at most) */
        }
        if (isdigit((unsigned char)(*p))) {
            Log::Error("invalid format (width or precision too long) for %s", strfrmt);
            return nullptr;
        }

        *(form++) = '%';
        strncpy(form, strfrmt, p - strfrmt + 1);
        form += p - strfrmt + 1;
        *form = '\0';
        return p;
    }
```
