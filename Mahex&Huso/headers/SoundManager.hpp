#include <windows.h>
#include <mmsystem.h>
#include <string>
#include <map>
#include <thread>
#include <sstream>
#include <chrono>
#include <vector>
#include <atomic>
#pragma comment(lib, "winmm.lib")

class SoundManager {
    public:
    SoundManager() = default;
    ~SoundManager();
    void PreloadSound(const std::string&, const std::wstring&);
    void PlayCustomSound(const std::string&);
    void PlayBackgroundMusic();
    void StopBackgroundMusic();
    bool IsSoundPlaying(const std::string&);

    private:
    struct SoundThread {
        std::thread thread;
        std::string alias;
    };
    std::map<std::string, std::wstring> m_sounds;
    SoundThread m_threadBackgroundMusic, m_threadGameWin, m_threadGameOver;

    void PlaySoundFile(const std::string&, const std::wstring&);
    void PlaySoundFileLooped(const std::string&, const std::wstring&);
    void StopSound(const std::string&);
    void CleanupFinishedThreads();
};