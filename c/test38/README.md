# C ����ʵ��һ���򵥵�Э�̵�����

Э�̣�ʹ��ͬ����ʽ��д�첽�߼���

������Ŀ��RPC�о���������������νRPC����Զ�̵��õĽӿڵ������غ���һ����ã�ֻ�����ǻ�û��������ֱ�ӻ�ȡ���ķ���ֵ������

```c++
int nLobbyId = CSNSMgrConnect::getInstance()->RemoteCall(&ISNSMgr_Lobby::Request_ActorLobby, nFriendId)
if (nLobbyId > 0) {
        // friend is online, sending chat message.
} else {
        // to client: target is offline.
}
```

�Ժ��������߼���ܳ�����Ŀǰ�� FORWARD ��ʽ���Ͳ��ò�ͬ��͸�� event �Ա��ڻص������߼��ַ�����ȻҲ��ʵ�����󣬵����ɱ���ý�һ�����������������������ֻ��Ϊ������һ��RPC���á�

1. Lobby::Request_SendMessage
2. SNSMgr::Request_ActorLobby
3. Lobby::Do_SendMessageToLobby

����и���RPC�����أ�

���Ļ��������������������

Э�̱������ӣ��Ʒ�֮ǰ�� [��һ��C�ļ�ʵ����](https://www.cyhone.com/articles/analysis-of-cloudwu-coroutine/)�����ӵ�����ô��Э����RPC���߼��ַ����������Э�̵Ĵ�������Ҫ���ӡ�

���TARS����֧�־͸��ã�Ҳ���Բο�΢�ŵ� libco��

���ڹ���֮���о�һ�£������ܷ��õ��ϡ�
