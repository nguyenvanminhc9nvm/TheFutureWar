#include "TheFutureWar/GameModes/BaseGameMode.h"
#include "TheFutureWar/Controllers/BasePlayerController.h"
#include "TheFutureWar/Characters/Base/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"

ABaseGameMode::ABaseGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
    InitializeGameState();
}

void ABaseGameMode::BeginPlay()
{
    Super::BeginPlay();
}

void ABaseGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Cập nhật thời gian game nếu game đang chạy
    if (bIsGameStarted && !bIsGamePaused)
    {
        GameTime += DeltaTime;
    }
}

void ABaseGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);
}

void ABaseGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
    Super::HandleStartingNewPlayer_Implementation(NewPlayer);
    SpawnPlayer(NewPlayer);
}

void ABaseGameMode::StartGame()
{
    if (!bIsGameStarted)
    {
        bIsGameStarted = true;
        bIsGamePaused = false;
        GameTime = 0.0f;
    }
}

void ABaseGameMode::PauseGame()
{
    if (bIsGameStarted && !bIsGamePaused)
    {
        bIsGamePaused = true;
    }
}

void ABaseGameMode::ResumeGame()
{
    if (bIsGameStarted && bIsGamePaused)
    {
        bIsGamePaused = false;
    }
}

void ABaseGameMode::EndGame()
{
    if (bIsGameStarted)
    {
        bIsGameStarted = false;
        bIsGamePaused = false;
    }
}

void ABaseGameMode::SpawnPlayer(APlayerController* PlayerController)
{
    if (PlayerController)
    {
        // Get spawn location
        FVector SpawnLocation = FVector::ZeroVector;
        FRotator SpawnRotation = FRotator::ZeroRotator;

        // Try to find a player start
        if (AActor* PlayerStart = FindPlayerStart(PlayerController))
        {
            SpawnLocation = PlayerStart->GetActorLocation();
            SpawnRotation = PlayerStart->GetActorRotation();
        }

        // Spawn the player
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        if (APawn* PlayerPawn = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, SpawnLocation, SpawnRotation, SpawnParams))
        {
            PlayerController->Possess(PlayerPawn);
        }
    }
}

void ABaseGameMode::HandlePlayerJoin(APlayerController* NewPlayer)
{
    if (NewPlayer)
    {
        PlayerCount++;
    }
}

void ABaseGameMode::HandlePlayerLeave(APlayerController* LeavingPlayer)
{
    if (LeavingPlayer)
    {
        PlayerCount--;
    }
}

void ABaseGameMode::InitializeGameState()
{
    bIsGameStarted = false;
    bIsGamePaused = false;
    GameTime = 0.0f;
    PlayerCount = 0;
}

void ABaseGameMode::InitializeGameSettings()
{
    // Initialize any additional game settings here
} 