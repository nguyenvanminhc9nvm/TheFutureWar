#pragma once

#include "CoreMinimal.h"
#include "TheFutureWar/Characters/Base/BaseCharacter.h"
#include "PlayerCharacter.generated.h"

/**
 * PlayerCharacter - Class đại diện cho nhân vật người chơi
 * 
 * Kế thừa từ BaseCharacter và mở rộng thêm các tính năng đặc thù cho player:
 * - Hệ thống inventory
 * - Hệ thống combat
 * - Hệ thống ability
 * - UI interaction
 */
UCLASS()
class THEFUTUREWAR_API APlayerCharacter : public ABaseCharacter
{
    GENERATED_BODY()

public:
    // Constructor
    APlayerCharacter();

    // Override các hàm cơ bản
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    // Override hàm setup input để xử lý input của người chơi
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Override các hàm movement
    virtual void MoveForward(float Value) override;
    virtual void MoveRight(float Value) override;

    // Override các hàm state
    virtual void StartSprint() override;
    virtual void StopSprint() override;
    virtual bool CanSprint() const override;

    // Override các hàm camera
    virtual void RotateCamera(float Value) override;
    virtual void ZoomCamera(float Value) override;

protected:
    // Các hàm xử lý input
    // Xử lý input di chuyển tiến/lùi
    void OnMoveForward(float Value);
    // Xử lý input di chuyển trái/phải
    void OnMoveRight(float Value);

    // Các hàm xử lý delegate
    /** Handle health changes */
    UFUNCTION()
    void HandleHealthChanged(float NewHealth);

    /** Handle stamina changes */
    UFUNCTION()
    void HandleStaminaChanged(float NewStamina);

    /** Handle state changes */
    UFUNCTION()
    void HandleStateChanged(ECharacterState NewState);

    /** Handle character death */
    UFUNCTION()
    void HandleCharacterDied();

    // Các thuộc tính riêng của player
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
    float PlayerSprintSpeed = 1200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
    float PlayerStaminaRegenRate = 15.0f;

    // TODO: Add InventoryComponent in Phase 2
    // TODO: Add CombatComponent in Phase 2
    // TODO: Add AbilitySystemComponent in Phase 3
}; 