#include "TheFutureWar/Controllers/BasePlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TheFutureWar/Characters/Base/BaseCharacter.h"

ABasePlayerController::ABasePlayerController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ABasePlayerController::BeginPlay()
{
    Super::BeginPlay();
    SetupCamera();
    InitializeInputBindings();
}

void ABasePlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABasePlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
}

void ABasePlayerController::HandleMoveForward(float Value)
{
    if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetPawn()))
    {
        Character->MoveForward(Value);
    }
}

void ABasePlayerController::HandleMoveRight(float Value)
{
    if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetPawn()))
    {
        Character->MoveRight(Value);
    }
}

void ABasePlayerController::HandleLookHorizontal(float Value)
{
    AddYawInput(Value * MouseSensitivity);
}

void ABasePlayerController::HandleLookVertical(float Value)
{
    AddPitchInput(Value * MouseSensitivity * InvertedYAxis);
}

void ABasePlayerController::HandleJump()
{
    if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetPawn()))
    {
        Character->Jump();
    }
}

void ABasePlayerController::HandleSprintStart()
{
    if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetPawn()))
    {
        // TODO: Implement sprint functionality
    }
}

void ABasePlayerController::HandleSprintEnd()
{
    if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetPawn()))
    {
        // TODO: Implement sprint functionality
    }
}

void ABasePlayerController::SetupCamera()
{
    // Thiết lập các thuộc tính camera cơ bản
    bShowMouseCursor = false;
    DefaultMouseCursor = EMouseCursor::None;
}

void ABasePlayerController::InitializeInputBindings()
{
    // TODO: Implement input bindings using Enhanced Input System
} 