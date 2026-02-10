// pch.h: это предварительно скомпилированный заголовочный файл.
// Перечисленные ниже файлы компилируются только один раз, что ускоряет последующие сборки.
// Это также влияет на работу IntelliSense, включая многие функции просмотра и завершения кода.
// Однако изменение любого из приведенных здесь файлов между операциями сборки приведет к повторной компиляции всех(!) этих файлов.
// Не добавляйте сюда файлы, которые планируете часто изменять, так как в этом случае выигрыша в производительности не будет.

#ifndef PCH_H
#define PCH_H

// Добавьте сюда заголовочные файлы для предварительной компиляции
#include <stdexcept>
#include <vector>
#include <array>
#include <memory>
#include <functional>
#include <cmath>
#include <iostream>
#include <sstream>

#include "framework.h"
#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include <SDL3_ttf/SDL_ttf.h>
#include "nlohmann/json.hpp"

#include "Utils.h"

#endif //PCH_H
