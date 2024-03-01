#include "sessionmanager.h"
#include "mediasession.h"

namespace RACLE {

MediaSessionManager *MediaSessionManager::createNewManager() {
    return new MediaSessionManager();
}

bool MediaSessionManager::addSession(MediaSession *session) {
    if (mSessMap.find(session->name()) != mSessMap.end()) { // 已存在
        return false;
    } else {
        mSessMap.insert(std::make_pair(session->name(), session));
        return true;
    }
}

bool MediaSessionManager::removeSession(MediaSession *session) {
    auto it = mSessMap.find(session->name());
    if (it == mSessMap.end()) {
        return false;
    } else {
        mSessMap.erase(it);
        return true;
    }
}

MediaSession *MediaSessionManager::getSession(const std::string &name) {
    auto it = mSessMap.find(name);
    if (it == mSessMap.end()) {
        return nullptr;
    } else {
        return it->second;
    }
}

} // namespace RACLE
