#include "GameCreationState.hpp"

#include "AppEngine.hpp"
#include "AppState.hpp"

#include <ENGINE/Logger.hpp>
#include "GamePlayState.hpp"

GameCreationState GameCreationState::GameCreationState_dontfuckwithme;

void GameCreationState::Init(AppEngine* app_)
{
	logger::INFO("GameCreationState Init");

	app = app_;

	logger::INFO("GameCreationState ready.");
}

void GameCreationState::Cleanup()
{
	logger::INFO("GameCreationState Cleaningup");

	logger::INFO("GameCreationState Cleanedup");
}

void GameCreationState::Pause()
{
	logger::INFO("GameCreationState Paused");
}

void GameCreationState::Resume()
{
	logger::INFO("GameCreationState Resumed");
}

void GameCreationState::HandleEvents()
{
}

void GameCreationState::Update()
{
	app->ChangeState(GamePlayState::Instance());
}

void GameCreationState::Draw()
{
}
