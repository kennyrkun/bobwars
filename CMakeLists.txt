CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_REQUIRED_STANDARD ON)
SET(CMAKE_DEBUG_POSTFIX "-d")

SET(PROJECT_NAME "bobwarss")
PROJECT(${PROJECT_NAME} LANGUAGES CXX)

SET(HEADERS AppEngine.hpp BaseEntity.hpp Bob.hpp Camera.hpp CommentSection.hpp Util/Graphics/DisabledButton.hpp Util/DiscordRPC.hpp EntityManager.hpp GameCreationState.hpp GameEndState.hpp GamePauseState.hpp GamePlayState.hpp GameStartupState.hpp Google.hpp GooglePlus.hpp Util/Graphics/Graphics.hpp Interface.hpp Util/Graphics/Line.hpp Util/Logger.cpp MainMenuState.hpp Menu.hpp Util/Graphics/ProgressBar.hpp ResourceCounter.hpp ResourceManager.hpp Server.hpp Util/SettingsParser.hpp Util/Graphics/SmallUnitIcon.hpp Util/Graphics/Text.hpp Util/Util.hpp)
SET(SOURCES AppEngine.cpp BaseEntity.cpp Bob.cpp Camera.cpp CommentSection.cpp Util/Graphics/DisabledButton.cpp Util/DiscordRPC.cpp EntityManager.cpp GameCreationState.cpp GameEndState.cpp GamePauseState.cpp GamePlayState.cpp GameStartupState.cpp Google.cpp GooglePlus.cpp Util/Graphics/Graphics.cpp Interface.cpp Util/Graphics/Line.cpp Util/Logger.cpp main.cpp MainMenuState.cpp Menu.cpp Util/Graphics/ProgressBar.cpp ResourceCounter.cpp ResourceManager.cpp Server.cpp Util/SettingsParser.cpp Util/Graphics/SmallUnitIcon.cpp Util/Graphics/Text.cpp Util/Util.cpp)
ADD_EXECUTABLE(${PROJECT_NAME} ${HEADERS} ${SOURCES})

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC ../SFML/include)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} sfml-graphics sfml-audio sfml-window sfml-network sfml-system -L"../SFML/lib")

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC ../SFUI/include)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} sfui -L"../SFUI/lib")

TARGET_LINK_LIBRARIES(${PROJECT_NAME} stdc++fs)
