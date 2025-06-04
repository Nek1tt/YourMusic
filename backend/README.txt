запуск трека
curl.exe -X POST http://localhost:8080 -H "Content-Type: application/json" --data-raw '{\"command\":\"load\", \"path\":\"C:/Users/LeMeS/Downloads/gimn-sssr-klassicheskiy.mp3\"}'


вопсроизвести
curl.exe -X POST http://localhost:8080 -H "Content-Type: application/json" --data-raw '{\"command\":\"resume\"}'

музыкальная паУза
curl.exe -X POST http://localhost:8080 -H "Content-Type: application/json" --data-raw '{\"command\":\"pause\"}'


выключить
curl.exe -X POST http://localhost:8080 -H "Content-Type: application/json" --data-raw '{\"command\":\"quit\"}'

громкость
curl.exe -X POST http://localhost:8080 -H "Content-Type: application/json" --data-raw '{\"command\":\"volume\", \"level\":0.7}'


пермотка
curl.exe -X POST http://localhost:8080 -H "Content-Type: application/json" --data-raw '{\"command\":\"seeking\", \"position\":0.4}'
