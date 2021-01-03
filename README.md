This is ICNDataClient
ICNDataClient 主要事情如下：
1. 接收用户对于某事件的请求，将请求Interest其转发至ICN节点
2. 接收到ICN节点转发的数据包，根据不同事件名称转发至接收线程进行还原，存放于本地
