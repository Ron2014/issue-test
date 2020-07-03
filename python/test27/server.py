#!/usr/bin/env python3
# -*- coding:utf-8 -*-
 
# ==== 基于TCP协议的socket实现远程命令输入之Server ====
 
import subprocess
from socket import *
 
server = socket(AF_INET, SOCK_STREAM)
server.bind(("192.168.0.143",6666))  # 填入私网IP
server.listen(5)
 
while 1:  # 链接循环
    conn,client_addr = server.accept()
    while 1:  # 通信循环
        try:  # 防止Windows平台下Client端异常关闭导致双向链接崩塌Server端异常的情况发生
            cmd = conn.recv(1024)
            if not cmd:  # 防止类Unix平台下Client端异常关闭导致双向链接崩塌Server端异常的情况发生
                break
            res = subprocess.Popen(cmd.decode("utf-8"),
                             shell=True,
                             stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE,)
 
            stdout_res = res.stdout.read()  # 正确结果
            stderr_res = res.stderr.read()  # 错误结果
            # subprocess模块拿到的是bytes类型，所以直接发送即可
 
            cmd_res = stdout_res if stdout_res else stderr_res  # 因为两个结果只有一个有信息，所以我们只拿到有结果的那个
            conn.send(cmd_res)
 
        except Exception:
            break
 
    conn.close()  # 由于client端链接异常，故关闭链接循环
