cmake_minimum_required(VERSION 3.14)

# задаем используемый стандарт С++
set(CMAKE_CXX_STANDARD 17)

project(LabWork3)

# задаем имя исполняемого файла основной программы
set(exe_name ${PROJECT_NAME})

# задаем имя исполняемого файла программы для запуска тестов
set(test_name test)

# Включаем отслеживание всех предупреждений (warnings) и запрещаем компиляцию при наличии предупреждений.
# Считается хорошим тоном, когда программа собирается без предупреждений.
# Но вы можете убрать /WX и -Werror (разрешив компиляцию с предупреждениями), если сможете разумно объяснить своё решение.
if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

if(WIN32 OR MSVC)
    include(FetchContent)
    FetchContent_Declare(
            PDcurses
            GIT_REPOSITORY https://github.com/dsavenko/PDCurses.git
            GIT_TAG        dfbac0fe5ef90a40a67d6b13cc7d9c69bf40a5f5
    )
    FetchContent_MakeAvailable(PDcurses)
endif()

# описываем основную библиотеку, т.е. библиотеку, содержащую все cpp-файлы, кроме main.cpp
add_library(corelib OBJECT SupFuncs.cpp Objects.cpp Enemys.cpp Blocks.cpp)

# описываем исполняемый файл основной программы: это main.cpp + основная библиотека
add_executable(${exe_name} Main.cpp)
target_link_libraries(${exe_name} corelib)

if(WIN32 OR MSVC)
    target_link_libraries(${exe_name} PDcurses)
else()
    target_link_libraries(${exe_name} curses)
endif()

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories( ${SFML_INCLUDE_DIRS} )
target_link_libraries(${exe_name}  sfml-system sfml-window sfml-graphics sfml-network sfml-audio)

# ========== все, что ниже, относится к тестированию при помощи GoogleTest ==========

# скачиваем библиотеку GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/58d77fa8070e8cec2dc1ed015d66b454c8d78850.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# описываем исполняемый файл программы для запуска тестов: это cpp-файлы тестов + основная библиотека + библиотека gtest_main
# наш собственный main.cpp здесь не нужен, т.к. функция main реализована в gtest_main
add_executable(${test_name} SourceTests.cpp)
target_link_libraries(${test_name} gtest_main)
target_link_libraries(${test_name} corelib)
