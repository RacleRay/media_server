#include "AACsink.h"
#include "AACsource.h"
#include "H264sink.h"
#include "H264source.h"
#include "log.h"
#include "rtspserver.h"
#include "scheduler.h"
#include "sessionmanager.h"
#include "threadpool.h"
#include "uenv.h"

int main() {
    /*
    // rtp over tcp
    ffplay -i -rtsp_transport tcp rtsp://127.0.0.1:8554/testplay

    // rtp over udp
    ffplay -i rtsp://127.0.0.1:8554/testplay
    */

    srand(time(nullptr)); // 时间初始化

    RACLE::Scheduler *scheduler = RACLE::Scheduler::createNew(RACLE::Scheduler::POLLER_SELECT);
    ThreadPool *threadPool = ThreadPool::createNew(1);
    RACLE::UserEnvironment *env = RACLE::UserEnvironment::createNew(scheduler, threadPool);

    RACLE::MediaSessionManager *sessMgr = RACLE::MediaSessionManager::createNewManager();
    IPv4Address rtspAddr("127.0.0.1", 8554);
    RACLE::RtspServer *rtspServer = RACLE::RtspServer::createNew(env, sessMgr, rtspAddr);

    LOGI("------session init start------");
    {
        RACLE::MediaSession *session = RACLE::MediaSession::createNewSession("testplay");

        RACLE::MediaSource *source = RACLE::H264FileMediaSource::createNew(env, "../data/file.h264");
        RACLE::Sink *sink = RACLE::H264FileSink::createNew(env, source);
        session->addSink(RACLE::MediaSession::TrackId0, sink);

        source = RACLE::AACFileMeidaSource::createNew(env, "../data/file.aac");
        sink = RACLE::AACFileSink::createNew(env, source);
        session->addSink(RACLE::MediaSession::TrackId1, sink);

        // session->startMulticast(); //多播
        sessMgr->addSession(session);
    }
    LOGI(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");

    rtspServer->start();

    env->scheduler()->loop();

    return 0;
}