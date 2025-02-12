#include "headers/SoundManager.hpp"

SoundManager::~SoundManager() {
    StopBackgroundMusic();

    StopSound(m_threadGameWin.alias);
    StopSound(m_threadGameOver.alias);

    CleanupFinishedThreads();
}

void SoundManager::PreloadSound(const std::string &alias, const std::wstring &filename) {
    m_sounds[alias] = filename;
}

void SoundManager::PlayCustomSound(const std::string& sound) {
    if(m_sounds.find(sound) == m_sounds.end()) return;

    CleanupFinishedThreads();

    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    std::stringstream aliasStream;
    aliasStream << "sound_" << timestamp;
    std::string alias = aliasStream.str();

    std::wstring wAlias;
    wAlias.assign(alias.begin(), alias.end());

    if(sound == "gamewin") {
        m_threadGameWin.alias = alias;
        m_threadGameWin.thread = std::thread(&SoundManager::PlaySoundFile, this, sound, wAlias);
    } else if(sound == "gameover") {
        m_threadGameOver.alias = alias;
        m_threadGameOver.thread = std::thread(&SoundManager::PlaySoundFile, this, sound, wAlias);
    } else {
        std::thread soundThread(&SoundManager::PlaySoundFile, this, sound, wAlias);
        soundThread.detach();
    }
}

void SoundManager::PlayBackgroundMusic() {
    CleanupFinishedThreads();

    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    std::stringstream aliasStream;
    aliasStream << "bgm_" << timestamp;
    std::string alias = aliasStream.str();

    std::wstring wAlias;
    wAlias.assign(alias.begin(), alias.end());

    m_threadBackgroundMusic.alias = alias;
    m_threadBackgroundMusic.thread = std::thread(&SoundManager::PlaySoundFileLooped, this, "background", wAlias);
}

void SoundManager::StopBackgroundMusic() {
    CleanupFinishedThreads();

    StopSound(m_threadBackgroundMusic.alias);
    if(m_threadBackgroundMusic.thread.joinable()) m_threadBackgroundMusic.thread.join();
}

bool SoundManager::IsSoundPlaying(const std::string &alias) {
    wchar_t status[256];
    std::wstring statusCmd;

    if(alias == "background") {
        std::wstring wAlias;
        wAlias.assign(m_threadBackgroundMusic.alias.begin(), m_threadBackgroundMusic.alias.end());
        statusCmd = L"status " + wAlias + L" mode";
    } else if(alias == "gamewin") {
        std::wstring wAlias;
        wAlias.assign(m_threadGameWin.alias.begin(), m_threadGameWin.alias.end());
        statusCmd = L"status " + wAlias + L" mode";
    } else if(alias == "gameover") {
        std::wstring wAlias;
        wAlias.assign(m_threadGameOver.alias.begin(), m_threadGameOver.alias.end());
        statusCmd = L"status " + wAlias + L" mode";
    }

    mciSendString(statusCmd.c_str(), status, 256, NULL);
    return std::wstring(status) == L"playing";
}

void SoundManager::StopSound(const std::string& alias) {
    if(alias.empty()) return;

    std::wstring wAlias;
    wAlias.assign(alias.begin(), alias.end());

    std::wstring stopCmd = L"stop " + wAlias;
    std::wstring closeCmd = L"close " + wAlias;

    mciSendString(stopCmd.c_str(), NULL, 0, NULL);
    mciSendString(closeCmd.c_str(), NULL, 0, NULL);
}

void SoundManager::PlaySoundFile(const std::string &sound, const std::wstring &alias) {
    std::wstring openCommand = L"open \"" + m_sounds[sound] + L"\" type waveaudio alias " + alias;
    mciSendString(openCommand.c_str(), NULL, 0, NULL);

    std::wstring playCommand = L"play " + alias;
    mciSendString(playCommand.c_str(), NULL, 0, NULL);
}

void SoundManager::PlaySoundFileLooped(const std::string &sound, const std::wstring &alias) {
    std::wstring openCommand = L"open \"" + m_sounds[sound] + L"\" type waveaudio alias " + alias;
    mciSendString(openCommand.c_str(), NULL, 0, NULL);

    std::wstring playCommand = L"play " + alias + L" repeat";
    mciSendString(playCommand.c_str(), NULL, 0, NULL);
}

void SoundManager::CleanupFinishedThreads() {
    if(m_threadBackgroundMusic.thread.joinable()) m_threadBackgroundMusic.thread.join();
    if(m_threadGameWin.thread.joinable()) m_threadGameWin.thread.join();
    if(m_threadGameOver.thread.joinable()) m_threadGameOver.thread.join();
}