#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseGameMode.generated.h"

/**
 * BaseGameMode - Class cơ sở cho tất cả các GameMode trong game
 * 
 * Class này cung cấp các chức năng cơ bản cho việc quản lý game:
 * - Quản lý trạng thái game
 * - Xử lý các sự kiện game
 * - Quản lý người chơi
 * - Quản lý điểm số và thời gian
 * 
 * Các class con có thể kế thừa và mở rộng thêm các chức năng đặc thù
 */
UCLASS(Blueprintable, Abstract)
class THEFUTUREWAR_API ABaseGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    // Constructor
    ABaseGameMode();

    // Override các hàm cơ bản của GameMode
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // ===== Game State Functions =====
    // Bắt đầu game
    UFUNCTION(BlueprintCallable, Category = "Game State")
    virtual void StartGame();

    // Kết thúc game
    UFUNCTION(BlueprintCallable, Category = "Game State")
    virtual void EndGame();

    // Tạm dừng game
    UFUNCTION(BlueprintCallable, Category = "Game State")
    virtual void PauseGame();

    // Tiếp tục game
    UFUNCTION(BlueprintCallable, Category = "Game State")
    virtual void ResumeGame();

    // ===== Player Management =====
    // Xử lý khi người chơi tham gia
    UFUNCTION(BlueprintCallable, Category = "Player Management")
    virtual void HandlePlayerJoin(APlayerController* NewPlayer);

    // Xử lý khi người chơi rời đi
    UFUNCTION(BlueprintCallable, Category = "Player Management")
    virtual void HandlePlayerLeave(APlayerController* LeavingPlayer);

protected:
    // ===== Properties =====
    // Trạng thái game
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
    bool bIsGameStarted;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
    bool bIsGamePaused;

    // Thời gian game
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
    float GameTime;

    // Số người chơi
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Management")
    int32 PlayerCount;

    /** Initialize game settings */
    UFUNCTION(BlueprintCallable, Category = "Game")
    virtual void InitializeGameSettings();

    /** Spawn player at start location */
    UFUNCTION(BlueprintCallable, Category = "Game")
    virtual void SpawnPlayer(APlayerController* PlayerController);

    /** Handle starting new player */
    virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

    /** Initialize game */
    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

private:
    // Hàm khởi tạo
    void InitializeGameState();
}; 