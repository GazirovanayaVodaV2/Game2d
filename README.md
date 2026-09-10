Переписывание https://github.com/GazirovanayaVodaV2/EarlyGame с нуля с более продуманным кодом и с использованием cmake.

Проект всё также использует [SDL 3](https://github.com/libsdl-org/SDL), [SDL Image](https://github.com/libsdl-org/SDL_image), [SDL ttf](https://github.com/libsdl-org/SDL_ttf)  и [Nlohmann json](https://github.com/nlohmann/json).

По сравнению с предыдущем проектом сильно улучшилось качество кода и работа с памятью.

Был начат переход к более лучшему формату карт.

Старый формат:

```
"layers": {
        "layers_info" : {
            "W":10,
            "H":5
        },

        "0": [
            0,0,0,1,1,1,1,1,0,0,
            0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,
            1,0,0,0,0,0,0,0,0,1,
            1,1,1,1,1,1,1,1,1,1
        ],
        ...
    }

```

Новый:

```
    "objects": [
        {
            "id": 1,
            "pos": [1000,64],
            "scale":"default"
        },
        {
            "id":2,
            "pos":[64, 64],
            "scale":"default"
        }
    ]
```

Была начато создание погодный условий. На данный момент была реализована только смена дня и ночи.

```
    "environment" : {
        "weather": "clear",
        "time": 6000,
        "time_cycle": true
    },
```

Зачаток погодных условий:

```
    enum class weather_t {
        clear,
        rain,
        snow,
        snow_storm,
        error
    };
```



Тестовый уровень в днейвное время:

![](C:/Users/skpsc/AppData/Roaming/marktext/images/2026-09-10-21-39-45-image.png)

В ночное:

![](C:/Users/skpsc/AppData/Roaming/marktext/images/2026-09-10-21-41-40-image.png)

Был практически полностью реализован инвентарь, а так же подбираемые предметы:

![](C:/Users/skpsc/AppData/Roaming/marktext/images/2026-09-10-21-40-03-image.png)

Сборка:
В теории оно должно собраться через нижнюю панель в vs code <img width="469" height="170" alt="image" src="https://github.com/user-attachments/assets/73b98829-5895-48b9-aa8d-5b987d314772" />

Иначе запустите через Developer Powershell .\non_vs_build_release.cmd
и в build/Release/bin будет бинарь
