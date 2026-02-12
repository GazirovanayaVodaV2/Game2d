#!/bin/bash
# Скрипт для проверки и установки Android NDK

echo "=== Android NDK Installer ==="
echo ""

# Проверить есть ли SDK Manager
SDK_MANAGER="/home/vodapc/Android/Sdk/cmdline-tools/latest/bin/sdkmanager"

if [ ! -f "$SDK_MANAGER" ]; then
    echo "❌ SDK Manager не найден в $SDK_MANAGER"
    echo ""
    echo "Установите SDK Tools:"
    echo "1. Откройте Android Studio"
    echo "2. Go to Tools → SDK Manager"
    echo "3. SDK Tools tab"
    echo "4. Найдите 'Android SDK Command-line Tools' и установите"
    exit 1
fi

echo "✅ SDK Manager найден"
echo ""

# Список доступных NDK версий
echo "Проверка доступных NDK версий..."
echo "(это может занять время...)"
echo ""

# Показать какой NDK установлен
echo "=== Установленные NDK версии ==="
ls -la /home/vodapc/Android/Sdk/ndk/ 2>/dev/null || echo "Нет установленных NDK версий"
echo ""

# Помочь установить
echo "=== Как установить NDK ==="
echo ""
echo "Вариант 1: Android Studio (рекомендуется)"
echo "  1. Tools → SDK Manager"
echo "  2. SDK Tools tab"
echo "  3. Найти 'Android NDK (Side by side)'"
echo "  4. Нажать Install"
echo ""

echo "Вариант 2: Из командной строки"
echo "  $SDK_MANAGER \"ndk;26.1.10909125\""
echo ""
echo "Вариант 3: Установить последнюю версию"
echo "  $SDK_MANAGER --list | grep ndk"
echo "  $SDK_MANAGER \"ndk;<версия>\""
echo ""

echo "После установки NDK Gradle сможет собрать проект."
echo "Проверить стало ли лучше:"
echo "  cd /media/vodapc/Hdd/projects/MetroidGame/Game2d"
echo "  ./gradlew assembleDebug"
