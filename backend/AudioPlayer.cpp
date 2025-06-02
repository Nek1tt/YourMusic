#include "AudioPlayer.h"
#include <iostream>
#include <cstring>

AudioPlayer::AudioPlayer(const char* filename) : filename(filename) {}

AudioPlayer::~AudioPlayer() {
    cleanup();
}

bool AudioPlayer::init() {
    avformat_network_init();

    if (avformat_open_input(&formatCtx, filename, nullptr, nullptr) != 0) {
        std::cerr << "Ошибка открытия файла: " << filename << std::endl;
        return false;
    }

    if (avformat_find_stream_info(formatCtx, nullptr) < 0) {
        std::cerr << "Ошибка получения информации о потоке!" << std::endl;
        return false;
    }

    for (unsigned int i = 0; i < formatCtx->nb_streams; i++) {
        if (formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            streamIndex = i;
            break;
        }
    }

    if (streamIndex == -1) {
        std::cerr << "Аудиопоток не найден!" << std::endl;
        return false;
    }

    AVCodecParameters* codecParams = formatCtx->streams[streamIndex]->codecpar;
    const AVCodec* codec = avcodec_find_decoder(codecParams->codec_id);
    if (!codec) {
        std::cerr << "Не найден кодек!" << std::endl;
        return false;
    }

    codecCtx = avcodec_alloc_context3(codec);
    if (!codecCtx) {
        std::cerr << "Ошибка выделения памяти под AVCodecContext!" << std::endl;
        return false;
    }

    if (avcodec_parameters_to_context(codecCtx, codecParams) < 0) {
        std::cerr << "Ошибка копирования параметров кодека!" << std::endl;
        return false;
    }

    if (avcodec_open2(codecCtx, codec, nullptr) < 0) {
        std::cerr << "Ошибка открытия кодека!" << std::endl;
        return false;
    }

    swrCtx = swr_alloc();
    AVChannelLayout out_ch_layout;
    av_channel_layout_default(&out_ch_layout, 2);

    swr_alloc_set_opts2(&swrCtx, &out_ch_layout, AV_SAMPLE_FMT_S16, 44100,
                        &codecCtx->ch_layout, codecCtx->sample_fmt, codecCtx->sample_rate, 0, nullptr);
    swr_init(swrCtx);

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "Ошибка инициализации SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    spec.freq = 44100;
    spec.format = AUDIO_S16SYS;
    spec.channels = 2;
    spec.silence = 0;
    spec.samples = 1024;
    spec.callback = audioCallback;

    audioBuffer = (uint8_t*)av_malloc(192000);
    audioData = new AudioData{codecCtx, formatCtx, streamIndex, swrCtx, av_packet_alloc(), av_frame_alloc(),
                              audioBuffer, 0, 0};

    spec.userdata = audioData;

    if (SDL_OpenAudio(&spec, nullptr) < 0) {
        std::cerr << "Ошибка открытия аудиоустройства: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void AudioPlayer::cleanup() {
    SDL_CloseAudio();

    if (audioData) {
        if (audioData->packet)
            av_packet_free(&audioData->packet);
        if (audioData->frame)
            av_frame_free(&audioData->frame);
        delete audioData;
        audioData = nullptr;
    }

    if (audioBuffer)
        av_free(audioBuffer);

    if (codecCtx)
        avcodec_free_context(&codecCtx);

    if (formatCtx)
        avformat_close_input(&formatCtx);

    if (swrCtx)
        swr_free(&swrCtx);

    SDL_Quit();
}

void AudioPlayer::audioCallback(void* userdata, Uint8* stream, int len) {
    AudioData* audio = (AudioData*)userdata;
    int bytesToCopy = len;
    uint8_t* outBuffer = stream;

    while (bytesToCopy > 0) {
        if (audio->bufferIndex >= audio->bufferSize) {
            if (av_read_frame(audio->formatCtx, audio->packet) >= 0) {
                if (audio->packet->stream_index == audio->streamIndex) {
                    int ret = avcodec_send_packet(audio->codecCtx, audio->packet);
                    if (ret >= 0) {
                        ret = avcodec_receive_frame(audio->codecCtx, audio->frame);
                        if (ret >= 0) {
                            int dst_nb_samples = av_rescale_rnd(
                                swr_get_delay(audio->swrCtx, audio->codecCtx->sample_rate) + audio->frame->nb_samples,
                                44100, audio->codecCtx->sample_rate, AV_ROUND_UP);

                            int out_samples = swr_convert(audio->swrCtx,
                                                          &audio->audioBuffer, dst_nb_samples,
                                                          (const uint8_t**)audio->frame->extended_data,
                                                          audio->frame->nb_samples);
                            audio->bufferSize = out_samples * 2 * sizeof(uint16_t);
                            audio->bufferIndex = 0;
                        }
                    }
                }
                av_packet_unref(audio->packet);
            } else {
                memset(outBuffer, 0, bytesToCopy);
                return;
            }
        }

        int copySize = std::min(bytesToCopy, audio->bufferSize - audio->bufferIndex);
        memcpy(outBuffer, audio->audioBuffer + audio->bufferIndex, copySize);
        bytesToCopy -= copySize;
        outBuffer += copySize;
        audio->bufferIndex += copySize;
    }
}




void AudioPlayer::playAudio() {
    if (!isPlaying) {
        SDL_PauseAudio(0);
        isPlaying = true;
    }
}

void AudioPlayer::pauseAudio() {
    if (isPlaying) {
        SDL_PauseAudio(1);
        isPlaying = false;
    }
}

void AudioPlayer::stopAudio() {
    if (isPlaying) {
        SDL_PauseAudio(1);
        isPlaying = false;
    }
    // Можно добавить очистку буфера, сброс позиций, если нужно
}

bool AudioPlayer::seekTo(double position) {
    if (!formatCtx || streamIndex < 0 || position < 0.01 || position > 1.0)
        return false;

    // Общая длительность в AV_TIME_BASE (микросекунды)
    int64_t duration = formatCtx->duration;

    // Переводим в микросекунды позицию
    int64_t target_ts = static_cast<int64_t>(duration * position);

    if (av_seek_frame(formatCtx, -1, target_ts, AVSEEK_FLAG_BACKWARD) < 0) {
        std::cerr << "Не удалось перемотать на позицию " << position << std::endl;
        return false;
    }

    // Очистка декодера
    avcodec_flush_buffers(codecCtx);

    // Сброс состояния буфера
    audioData->bufferIndex = 0;
    audioData->bufferSize = 0;

    return true;
}

