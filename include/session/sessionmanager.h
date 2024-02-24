#ifndef __SESSIONMANAGER__H__
#define __SESSIONMANAGER__H__

#include <map>
#include <string>

class MediaSession;
class MediaSessionManager {
  public:
    static MediaSessionManager *createNewManager();

    MediaSessionManager() = default;
    ~MediaSessionManager() = default;

  public:
    bool addSession(MediaSession *session);
    bool removeSession(MediaSession *session);
    MediaSession *getSession(const std::string &name);

  private:
    std::map<std::string, MediaSession *> mSessMap;
};

#endif //!__SESSIONMANAGER__H__