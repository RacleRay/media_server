# RSTP multimeida file server

> `test_server` completed.
> `ffplay -i rtsp://127.0.0.1:8554/testplay` for test

- Live555 设计思路，IO多路复用+定时器，音视频同步还有待改进
- 音视频同步思路可以参考 SDL_Player 中设计的基于音频的视频帧渲染时间轴调整方法
- RTSP 协议解析和 H.264 AAC 文件读取发包部分是最无聊的
- source 和 sink 队列设计还行，数据流向比较清晰
- 整体待优化部分很多，有空再说，比如线程池设计、回调逻辑简洁、语言编码规范、服务器并发能力优化、音视频同步

