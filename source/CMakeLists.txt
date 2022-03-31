add_library (BotLib ABSearch.cpp Table.cpp Game.cpp Vector.cpp)
target_include_directories (BotLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})