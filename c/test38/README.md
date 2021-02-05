# C 语言实现一个简单的协程调度器

协程：使用同步方式编写异步逻辑。

我们项目的RPC感觉还不算完美，所谓RPC即把远程调用的接口当作本地函数一般调用，只是我们还没做到可以直接获取它的返回值。例如

```c++
int nLobbyId = CSNSMgrConnect::getInstance()->RemoteCall(&ISNSMgr_Lobby::Request_ActorLobby, nFriendId)
if (nLobbyId > 0) {
        // friend is online, sending chat message.
} else {
        // to client: target is offline.
}
```

以后这样的逻辑会很常见。目前的 FORWARD 方式，就不得不同过透传 event 以便在回调中做逻辑分发。虽然也能实现需求，但不可避免得将一个函数拆成两个函数来处理，只因为当中有一个RPC调用。

1. Lobby::Request_SendMessage
2. SNSMgr::Request_ActorLobby
3. Lobby::Do_SendMessageToLobby

如果有更多RPC调用呢？

担心会拖慢开发组的生产力。

协程本身不复杂，云风之前就 [用一个C文件实现了](https://www.cyhone.com/articles/analysis-of-cloudwu-coroutine/)。复杂的是怎么将协程与RPC的逻辑分发结合起来，协程的创建还需要池子。

如果TARS本身支持就更好，也可以参考微信的 libco。

想在工作之余研究一下，看看能否用得上。
