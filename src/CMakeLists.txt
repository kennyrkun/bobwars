CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_REQUIRED_STANDARD ON)
SET(CMAKE_DEBUG_POSTFIX "-d")

SET(PROJECT_NAME "bobwarss")
PROJECT(${PROJECT_NAME} LANGUAGES CXX)

SET(HEADERS AppEngine.hpp 
			BaseEntity.hpp 
			Building.hpp
			Bob.hpp 
			Boomer.hpp
			Camera.hpp 
			CommentSection.hpp
			ComponentEntity.hpp
			EntityComponent.hpp
			EntityManager.hpp
			Explosion.hpp
			GroundMoveComponent.hpp
			GameCreationState.hpp
			GameEndState.hpp
			GamePauseState.hpp
			GamePlayState.hpp
			GameStartupState.hpp
			Google.hpp
			GooglePlus.hpp
			Interface.hpp
			MainMenuState.hpp
			ResourceCounter.hpp
			ResourceManager.hpp
			Util/Graphics/ProgressBar.hpp
			Util/SettingsParser.hpp
			Util/Graphics/SmallUnitIcon.hpp
			Util/Graphics/Text.hpp
			Util/Util.hpp
			Util/Graphics/DisabledButton.hpp
			Util/DiscordRPC.hpp
			Util/Graphics/Graphics.hpp
			Util/Graphics/Line.hpp
			Util/Logger.cpp)

SET(SOURCES AppEngine.cpp
			BaseEntity.cpp
			Bob.cpp
			Boomer.cpp
			Camera.cpp
			CommentSection.cpp
			ComponentEntity.cpp
			GroundMoveComponent.cpp
			EntityManager.cpp
			Explosion.cpp
			GameCreationState.cpp
			GameEndState.cpp
			GamePauseState.cpp
			GamePlayState.cpp
			GameStartupState.cpp
			Google.cpp
			GooglePlus.cpp
			Interface.cpp
			main.cpp
			MainMenuState.cpp
			ResourceCounter.cpp
			ResourceManager.cpp
			Util/SettingsParser.cpp
			Util/Graphics/SmallUnitIcon.cpp
			Util/Graphics/Text.cpp
			Util/Util.cpp
			Util/Graphics/DisabledButton.cpp
			Util/Graphics/DisabledButton.cpp
			Util/DiscordRPC.cpp
			Util/DiscordRPC.cpp
			Util/Graphics/Line.cpp
			Util/Graphics/Graphics.cpp
			Util/Logger.cpp
			Util/Graphics/ProgressBar.cpp)

ADD_EXECUTABLE(${PROJECT_NAME} ${HEADERS} ${SOURCES})

SET(SFUI_INCLUDE_DIR "../../SFUI/include")
SET(SFUI_LIB_DIR "../../SFUI/lib")
SET(SFML_INCLUDE_DIR "../../SFML/include")
SET(SFML_LIB_DIR "../../SFML/lib")

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC ${SFML_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} sfml-graphics sfml-audio sfml-window sfml-network sfml-system) # ${SFML_LIB_DIR})

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC ${SFUI_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} sfui) #${SFUI_LIB_DIR})

TARGET_LINK_LIBRARIES(${PROJECT_NAME} stdc++fs)
