cmake_minimum_required(VERSION 3.7)
project(Hellfire)

set(SDL2_DIR ${PROJECT_SOURCE_DIR}/lib/cmake/SDL2/)
set(SDL2_ttf_DIR ${PROJECT_SOURCE_DIR}/lib/cmake/SDL2_ttf/)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/)
set(CMAKE_CXX_STANDARD 11)



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