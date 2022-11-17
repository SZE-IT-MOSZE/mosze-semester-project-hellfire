cmake_minimum_required(VERSION 3.7)
project(Hellfire)

#gtest
include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
#------

set(SDL2_DIR ${PROJECT_SOURCE_DIR}/lib/cmake/SDL2/)
set(SDL2_ttf_DIR ${PROJECT_SOURCE_DIR}/lib/cmake/SDL2_ttf/)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/)
set(CMAKE_CXX_STANDARD 14)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
include_directories(${PROJECT_NAME}  ${PROJECT_SOURCE_DIR}/include/ ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

set(SOURCE_FILES src/main.cpp src/attributes.cpp src/chapter.cpp src/chest.cpp src/choice.cpp src/consumable.cpp
src/game.cpp src/interaction.cpp src/inventory.cpp src/item.cpp src/player.cpp src/render.cpp src/scene.cpp src/tinyxml2.cpp src/weapon.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include ${SDL2_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS})

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
file(COPY story DESTINATION ${CMAKE_BINARY_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/SDL2.dll
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_ttf.dll
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARIES} -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf)

# testing
include(GoogleTest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

enable_testing()
# game test

add_executable(test_sajat tests/gtest-game.cpp)
target_link_libraries(test_sajat gtest_main)
add_test(basic_game test_sajat)
