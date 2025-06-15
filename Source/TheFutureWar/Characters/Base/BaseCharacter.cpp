#include "TheFutureWar/Characters/Base/BaseCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Constructor của BaseCharacter
ABaseCharacter::ABaseCharacter()
{
    // Cho phép Tick function chạy mỗi frame
    PrimaryActorTick.bCanEverTick = true;

    // Khởi tạo các thuộc tính cơ bản
    WalkSpeed = 600.0f;
    SprintSpeed = 1000.0f;
    CrouchSpeed = 300.0f;

    MaxHealth = 100.0f;
    CurrentHealth = MaxHealth;
    MaxStamina = 100.0f;
    CurrentStamina = MaxStamina;
    StaminaRegenRate = 10.0f;
    SprintStaminaCost = 20.0f;

    DefaultCameraDistance = 300.0f;
    CrouchCameraDistance = 200.0f;
    CameraRotationRate = 45.0f;
    CameraPitchAngle = -15.0f;

    bIsSprinting = false;
    bIsCrouching = false;
    bIsJumping = false;
    bIsDead = false;

    // Tạo và thiết lập Camera Boom
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = DefaultCameraDistance;
    CameraBoom->bUsePawnControlRotation = true;
    CameraBoom->SetRelativeRotation(FRotator(CameraPitchAngle, 0.0f, 0.0f));

    // Tạo và thiết lập Camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    // Initialize state
    CurrentState = ECharacterState::Idle;
}

// Được gọi khi actor bắt đầu gameplay
void ABaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Khởi tạo các thuộc tính di chuyển
    if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
    {
        MovementComponent->bOrientRotationToMovement = true;
        MovementComponent->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
        MovementComponent->JumpZVelocity = 600.f;
        MovementComponent->AirControl = 0.2f;
        MovementComponent->MaxWalkSpeed = WalkSpeed;
    }

    // Khởi tạo input bindings
    InitializeInputBindings();
}

// Được gọi mỗi frame
void ABaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Update movement state
    UpdateMovementState();

    // Handle stamina
    if (bIsSprinting && CurrentStamina > 0.0f)
    {
        CurrentStamina = FMath::Max(0.0f, CurrentStamina - SprintStaminaCost * DeltaTime);
        OnStaminaChanged.Broadcast(CurrentStamina);
    }
    else if (!bIsSprinting && CurrentStamina < MaxStamina)
    {
        CurrentStamina = FMath::Min(MaxStamina, CurrentStamina + StaminaRegenRate * DeltaTime);
        OnStaminaChanged.Broadcast(CurrentStamina);
    }

    // Cập nhật camera khi ngồi xổm
    if (bIsCrouching)
    {
        CameraBoom->TargetArmLength = FMath::FInterpTo(
            CameraBoom->TargetArmLength,
            CrouchCameraDistance,
            DeltaTime,
            5.0f
        );
    }
    else
    {
        CameraBoom->TargetArmLength = FMath::FInterpTo(
            CameraBoom->TargetArmLength,
            DefaultCameraDistance,
            DeltaTime,
            5.0f
        );
    }
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Set up action bindings
    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // Moving
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABaseCharacter::OnMove);

        // Looking
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABaseCharacter::OnLook);

        // Jumping
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ABaseCharacter::OnJumpStarted);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ABaseCharacter::OnJumpCompleted);

        // Sprinting
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ABaseCharacter::OnSprintStarted);
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ABaseCharacter::OnSprintCompleted);

        // Crouching
        EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ABaseCharacter::OnCrouchStarted);
        EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ABaseCharacter::OnCrouchCompleted);

        // Camera
        EnhancedInputComponent->BindAction(CameraRotateAction, ETriggerEvent::Triggered, this, &ABaseCharacter::OnCameraRotate);
        EnhancedInputComponent->BindAction(CameraZoomAction, ETriggerEvent::Triggered, this, &ABaseCharacter::OnCameraZoom);
        EnhancedInputComponent->BindAction(CameraResetAction, ETriggerEvent::Started, this, &ABaseCharacter::OnCameraReset);
    }
}

// Hàm xử lý di chuyển tiến/lùi
void ABaseCharacter::MoveForward(float Value)
{
    if (Value != 0.0f)
    {
        // Lấy hướng nhìn của controller
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // Tính vector hướng di chuyển
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

// Hàm xử lý di chuyển trái/phải
void ABaseCharacter::MoveRight(float Value)
{
    if (Value != 0.0f)
    {
        // Lấy hướng nhìn của controller
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // Tính vector hướng di chuyển
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}

void ABaseCharacter::StartSprint()
{
    if (CanSprint())
    {
        bIsSprinting = true;
        UpdateMovementState();
    }
}

void ABaseCharacter::StopSprint()
{
    bIsSprinting = false;
    UpdateMovementState();
}

void ABaseCharacter::StartCrouch()
{
    if (CanCrouch())
    {
        Super::Crouch();
        bIsCrouching = true;
        UpdateMovementState();
    }
}

void ABaseCharacter::StopCrouch()
{
    Super::UnCrouch();
    bIsCrouching = false;
    UpdateMovementState();
}

void ABaseCharacter::Jump()
{
    if (CanCharacterJump())
    {
        Super::Jump();
        bIsJumping = true;
        UpdateMovementState();
    }
}

void ABaseCharacter::StopJumping()
{
    Super::StopJumping();
    bIsJumping = false;
    UpdateMovementState();
}

void ABaseCharacter::RotateCamera(float Value)
{
    if (Value != 0.0f)
    {
        AddControllerYawInput(Value * CameraRotationRate * GetWorld()->GetDeltaSeconds());
    }
}

void ABaseCharacter::ZoomCamera(float Value)
{
    if (Value != 0.0f)
    {
        float NewLength = CameraBoom->TargetArmLength + Value * 100.0f;
        CameraBoom->TargetArmLength = FMath::Clamp(NewLength, 100.0f, 1000.0f);
    }
}

void ABaseCharacter::ResetCamera()
{
    // Reset camera về vị trí mặc định
    CameraBoom->TargetArmLength = DefaultCameraDistance;
    CameraBoom->SetRelativeRotation(FRotator(CameraPitchAngle, 0.0f, 0.0f));
}

bool ABaseCharacter::IsAlive() const
{
    return !bIsDead && CurrentHealth > 0.0f;
}

bool ABaseCharacter::CanSprint() const
{
    return IsAlive() && !bIsCrouching && CurrentStamina > 0.0f;
}

bool ABaseCharacter::CanCrouch() const
{
    return IsAlive() && !bIsSprinting;
}

bool ABaseCharacter::CanCharacterJump() const
{
    // Check if character is in a state that allows jumping
    return !bIsDead && !bIsCrouching && !bIsSprinting && GetCharacterMovement() && GetCharacterMovement()->IsFalling() == false;
}

void ABaseCharacter::OnMove(const FInputActionValue& Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();
    MoveForward(MovementVector.Y);
    MoveRight(MovementVector.X);
}

void ABaseCharacter::OnLook(const FInputActionValue& Value)
{
    const FVector2D LookAxisVector = Value.Get<FVector2D>();
    AddControllerPitchInput(LookAxisVector.Y);
    AddControllerYawInput(LookAxisVector.X);
}

void ABaseCharacter::OnJumpStarted()
{
    Jump();
}

void ABaseCharacter::OnJumpCompleted()
{
    StopJumping();
}

void ABaseCharacter::OnSprintStarted()
{
    StartSprint();
}

void ABaseCharacter::OnSprintCompleted()
{
    StopSprint();
}

void ABaseCharacter::OnCrouchStarted()
{
    StartCrouch();
}

void ABaseCharacter::OnCrouchCompleted()
{
    StopCrouch();
}

void ABaseCharacter::OnCameraRotate(const FInputActionValue& Value)
{
    const float RotationValue = Value.Get<float>();
    RotateCamera(RotationValue);
}

void ABaseCharacter::OnCameraZoom(const FInputActionValue& Value)
{
    const float ZoomValue = Value.Get<float>();
    ZoomCamera(ZoomValue);
}

void ABaseCharacter::OnCameraReset()
{
    ResetCamera();
}

// Hàm khởi tạo các thuộc tính di chuyển
void ABaseCharacter::InitializeMovementProperties()
{
    // Thiết lập các thuộc tính di chuyển
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 600.f;
    GetCharacterMovement()->AirControl = 0.2f;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ABaseCharacter::InitializeStateProperties()
{
    // Khởi tạo các thuộc tính trạng thái
    CurrentHealth = MaxHealth;
    CurrentStamina = MaxStamina;
    bIsSprinting = false;
    bIsCrouching = false;
    bIsJumping = false;
    bIsDead = false;
}

void ABaseCharacter::InitializeInputBindings()
{
    // Add Input Mapping Context
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

void ABaseCharacter::InitializeCamera()
{
    // Tạo và thiết lập Camera Boom
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = DefaultCameraDistance;
    CameraBoom->bUsePawnControlRotation = true;
    CameraBoom->SetRelativeRotation(FRotator(CameraPitchAngle, 0.0f, 0.0f));

    // Tạo và thiết lập Camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
}

void ABaseCharacter::InitializeAnimation()
{
    // Khởi tạo các animation montage
    JumpMontage = nullptr;
    CrouchMontage = nullptr;
    SprintMontage = nullptr;
}

void ABaseCharacter::SetState(ECharacterState NewState)
{
    if (CurrentState != NewState)
    {
        CurrentState = NewState;
        OnStateChanged.Broadcast(NewState);
    }
}

void ABaseCharacter::UpdateMovementState()
{
    if (!GetCharacterMovement())
        return;

    if (bIsDead)
    {
        SetState(ECharacterState::Dead);
    }
    else if (bIsCrouching)
    {
        SetState(ECharacterState::Crouching);
    }
    else if (bIsJumping)
    {
        SetState(ECharacterState::Jumping);
    }
    else if (bIsSprinting)
    {
        SetState(ECharacterState::Running);
    }
    else if (GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0f)
    {
        SetState(ECharacterState::Walking);
    }
    else
    {
        SetState(ECharacterState::Idle);
    }
}

void ABaseCharacter::PlayHitReaction(const FVector& InHitDirection)
{
    if (!bIsHit)
    {
        bIsHit = true;
        HitDirection = InHitDirection;
        // Update animation state
        UpdateMovementState();
    }
}

void ABaseCharacter::OnHitReactionEnded()
{
    bIsHit = false;
    // Update animation state
    UpdateMovementState();
} 