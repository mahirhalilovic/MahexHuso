#include "headers/SoundManager.hpp"

SoundManager::~SoundManager() {
    StopBackgroundMusic();

    StopSound(m_threadGameWin.alias);
    StopSound(m_threadGameOver.alias);

    CleanupFinishedThreads();
}

void SoundManager::PreloadSound(const std::string &alias, const std::string &filename) {
    m_sounds[alias] = filename;
}

void SoundManager::PlayCustomSound(const std::string& sound) {
    if(m_sounds.find(sound) == m_sounds.end()) return;

    CleanupFinishedThreads();

    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    std::stringstream aliasStream;
    aliasStream << sound << "_" << timestamp;
    std::string alias = aliasStream.str();

    if(sound == "gamewin") {
        m_threadGameWin.alias = alias;
        m_threadGameWin.thread = std::thread(&SoundManager::PlaySoundFile, this, sound, alias);
    } else if(sound == "gameover") {
        m_threadGameOver.alias = alias;
        m_threadGameOver.thread = std::thread(&SoundManager::PlaySoundFile, this, sound, alias);
    } else {
        std::thread soundThread(&SoundManager::PlaySoundFile, this, sound, alias);
        soundThread.detach();
    }
}

void SoundManager::PlayBackgroundMusic() {
    PlaySound(m_sounds["background"].c_str(), NULL, SND_LOOP | SND_ASYNC);
}

void SoundManager::StopBackgroundMusic() {
    PlaySound(NULL, NULL, 0);
}

bool SoundManager::IsSoundPlaying(const std::string &alias) {
    char status[256];
    std::string statusCmd;

    if(alias == "background") {
        statusCmd = "status " + alias + " mode";
    } else if(alias == "gamewin") {
        statusCmd = "status " + alias + " mode";
    } else if(alias == "gameover") {
        statusCmd = "status " + alias + " mode";
    }

    if(statusCmd == "") return false;

    mciSendString(statusCmd.c_str(), status, 256, NULL);
    return status == "playing";
}

void SoundManager::StopSound(const std::string& alias) {
    if(alias.empty()) return;

    std::string stopCmd = "stop " + alias;
    std::string closeCmd = "close " + alias;

    mciSendString(stopCmd.c_str(), NULL, 0, NULL);
    mciSendString(closeCmd.c_str(), NULL, 0, NULL);
}

void SoundManager::PlaySoundFile(const std::string &sound, const std::string &alias) {
    std::string openCommand = std::string("open \"") + m_sounds[sound].c_str() + "\" type waveaudio alias " + alias;
    mciSendString(openCommand.c_str(), NULL, 0, NULL);

    std::string playCommand = "play " + alias;
    mciSendString(playCommand.c_str(), NULL, 0, NULL);
}

void SoundManager::PlaySoundFileLooped(const std::string &sound, const std::string &alias) {
    std::string openCommand = std::string("open \"") + m_sounds[sound].c_str() + "\" type waveaudio alias " + alias;
    MCIERROR openError = mciSendString(openCommand.c_str(), NULL, 0, NULL);
    if(openError != 0) {
        MessageBox(NULL, "Could not open background music file!", "Error", MB_OK);
        return;
    }

    std::string playCommand = "play " + alias + " repeat";
    MCIERROR playError = mciSendString(playCommand.c_str(), NULL, 0, NULL);
    if(playError != 0) {
        MessageBox(NULL, "Could not play background music file!", "Error", MB_OK);
    }
}

void SoundManager::CleanupFinishedThreads() {
    if(m_threadBackgroundMusic.thread.joinable()) m_threadBackgroundMusic.thread.join();
    if(m_threadGameWin.thread.joinable()) m_threadGameWin.thread.join();
    if(m_threadGameOver.thread.joinable()) m_threadGameOver.thread.join();
}
