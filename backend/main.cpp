#include "AudioPlayer.h"
#include <iostream>
#include <string>

int main() {
    AudioPlayer* player = nullptr;

    std::string command;
    std::cout << "Введите команду:\n"
                 "load <путь_к_файлу> - загрузить и воспроизвести трек\n"
                 "pause - поставить на паузу\n"
                 "resume - продолжить воспроизведение\n"
                 "quit - выход\n";

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);

        if (command.rfind("load ", 0) == 0) {
            // Если уже есть плеер — очистим старый
            if (player) {
                player->stopAudio();
                delete player;
                player = nullptr;
            }

            std::string filepath = command.substr(5);
            player = new AudioPlayer(filepath.c_str());
            if (!player->init()) {
                std::cerr << "Не удалось загрузить трек: " << filepath << std::endl;
                delete player;
                player = nullptr;
            } else {
                player->playAudio();
                std::cout << "Проигрывание: " << filepath << std::endl;
            }
        } else if (command == "pause") {
            if (player) {
                player->pauseAudio();
                std::cout << "Пауза" << std::endl;
            } else {
                std::cout << "Трек не загружен." << std::endl;
            }
        } else if (command == "resume") {
            if (player) {
                player->playAudio();
                std::cout << "Воспроизведение продолжено" << std::endl;
            } else {
                std::cout << "Трек не загружен." << std::endl;
            }
        } else if (command == "quit") {
            break;
        } else {
            std::cout << "Неизвестная команда" << std::endl;
        }
    }

    if (player) {
        player->stopAudio();
        delete player;
    }

    return 0;
}
