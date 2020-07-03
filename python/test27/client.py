#!/usr/bin/env python3
# -*- coding:utf-8 -*-
 
# ==== 基于TCP协议的socket实现远程命令输入之Client ====
 
from socket import *
 
client = socket(AF_INET,SOCK_STREAM)
client.connect(("192.168.0.143",6666))  # 填入Server端公网IP
 
while 1:
    cmd = input("请输入命令>>>:").strip()
    if not cmd:
        continue
    if cmd == "quit":
        break
    client.send(cmd.encode("utf-8"))
    cmd_res = client.recv(1024)  # 本次接收1024字节数据
    # print(cmd_res.decode("utf-8"))  # 如果Server端是Windows则用gbk解码，类Unix用utf-8解码
    while cmd_res:
        print(cmd_res.decode("utf-8"))  # 如果Server端是Windows则用gbk解码，类Unix用utf-8解码
        if len(cmd_res) < 1024:
            break
        cmd_res = client.recv(1024)
 
client.close()