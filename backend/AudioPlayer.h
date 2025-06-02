#pragma once

extern "C" {
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libswresample/swresample.h>
    #include <SDL2/SDL.h>
}

#include <cstdint>

class AudioPlayer {
public:
    explicit AudioPlayer(const char* filename);
    ~AudioPlayer();

    bool init();

    void playAudio();
    void pauseAudio();
    void stopAudio();
    bool seekTo(double position); 
    void setVolume(float vol);

private:
    static void audioCallback(void* userdata, Uint8* stream, int len);

    const char* filename;

    AVFormatContext* formatCtx = nullptr;
    AVCodecContext* codecCtx = nullptr;
    int streamIndex = -1;
    SwrContext* swrCtx = nullptr;
    AVPacket* packet = nullptr;
    AVFrame* frame = nullptr;

    uint8_t* audioBuffer = nullptr;
    int bufferSize = 0;
    int bufferIndex = 0;

    SDL_AudioSpec spec{};
    bool isPlaying = false;

     float volume = 1.0f;
    struct AudioData {
        AVCodecContext* codecCtx;
        AVFormatContext* formatCtx;
        int streamIndex;
        SwrContext* swrCtx;
        AVPacket* packet;
        AVFrame* frame;
        uint8_t* audioBuffer;
        int bufferSize;
        int bufferIndex;
        float volume;
    };

    AudioData* audioData = nullptr;

    void cleanup();
};
