#include "TheFutureWar/Characters/Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Constructor của PlayerCharacter
APlayerCharacter::APlayerCharacter()
{
    // Khởi tạo các thuộc tính riêng của player
    SprintSpeed = PlayerSprintSpeed;
    StaminaRegenRate = PlayerStaminaRegenRate;
}

// Được gọi khi actor bắt đầu gameplay
void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Bind delegates
    if (ABaseCharacter* BaseChar = Cast<ABaseCharacter>(this))
    {
        BaseChar->OnHealthChanged.AddDynamic(this, &APlayerCharacter::HandleHealthChanged);
        BaseChar->OnStaminaChanged.AddDynamic(this, &APlayerCharacter::HandleStaminaChanged);
        BaseChar->OnStateChanged.AddDynamic(this, &APlayerCharacter::HandleStateChanged);
        BaseChar->OnCharacterDied.AddDynamic(this, &APlayerCharacter::HandleCharacterDied);
    }
}

void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Thiết lập các input binding cho người chơi
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Xử lý input di chuyển tiến/lùi
void APlayerCharacter::OnMoveForward(float Value)
{
    // Gọi hàm di chuyển từ BaseCharacter
    MoveForward(Value);
}

// Xử lý input di chuyển trái/phải
void APlayerCharacter::OnMoveRight(float Value)
{
    // Gọi hàm di chuyển từ BaseCharacter
    MoveRight(Value);
}

void APlayerCharacter::MoveForward(float Value)
{
    // Override với logic riêng nếu cần
    Super::MoveForward(Value);
}

void APlayerCharacter::MoveRight(float Value)
{
    // Override với logic riêng nếu cần
    Super::MoveRight(Value);
}

void APlayerCharacter::StartSprint()
{
    // Override với logic riêng nếu cần
    Super::StartSprint();
}

void APlayerCharacter::StopSprint()
{
    // Override với logic riêng nếu cần
    Super::StopSprint();
}

bool APlayerCharacter::CanSprint() const
{
    // Override với logic riêng nếu cần
    return Super::CanSprint();
}

void APlayerCharacter::RotateCamera(float Value)
{
    // Override với logic riêng nếu cần
    Super::RotateCamera(Value);
}

void APlayerCharacter::ZoomCamera(float Value)
{
    // Override với logic riêng nếu cần
    Super::ZoomCamera(Value);
}

void APlayerCharacter::HandleHealthChanged(float NewHealth)
{
    // Xử lý khi health thay đổi
    // TODO: Update UI, play effects, etc.
}

void APlayerCharacter::HandleStaminaChanged(float NewStamina)
{
    // Xử lý khi stamina thay đổi
    // TODO: Update UI, play effects, etc.
}

void APlayerCharacter::HandleStateChanged(ECharacterState NewState)
{
    // Handle state changes
    switch (NewState)
    {
        case ECharacterState::Idle:
            // Handle idle state
            break;
        case ECharacterState::Walking:
            // Handle walking state
            break;
        case ECharacterState::Running:
            // Handle running state
            break;
        case ECharacterState::Jumping:
            // Handle jumping state
            break;
        case ECharacterState::Crouching:
            // Handle crouching state
            break;
        case ECharacterState::Dead:
            // Handle dead state
            break;
        default:
            break;
    }
}

void APlayerCharacter::HandleCharacterDied()
{
    // Xử lý khi character chết
    // TODO: Play death animation, show game over screen, etc.
} 