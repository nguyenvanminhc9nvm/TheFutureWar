#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Animation/AnimMontage.h"
#include "BaseCharacter.generated.h"

// Forward declarations
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;

/** Character states */
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
    Idle        UMETA(DisplayName = "Idle"),
    Walking     UMETA(DisplayName = "Walking"),
    Running     UMETA(DisplayName = "Running"),
    Jumping     UMETA(DisplayName = "Jumping"),
    Crouching   UMETA(DisplayName = "Crouching"),
    Dead        UMETA(DisplayName = "Dead")
};

/**
 * Base Character class for The Future War
 * 
 * This class serves as the foundation for all playable characters in the game.
 * It provides core functionality for:
 * - Movement (walk, run, jump, crouch)
 * - Camera control
 * - Animation state management
 * - Health and stamina systems
 * 
 * Future extensions:
 * - Combat system (Phase 2)
 * - Inventory system (Phase 2)
 * - Ability system (Phase 3)
 * - AI integration (Phase 3)
 */
UCLASS(Blueprintable, Abstract)
class THEFUTUREWAR_API ABaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Delegates
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, NewHealth);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaChanged, float, NewStamina);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateChanged, FName, NewState);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterDied);

    /** Called when health changes */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
    UPROPERTY(BlueprintAssignable, Category = "Character|Health")
    FOnHealthChangedSignature OnHealthChanged;

    /** Called when stamina changes */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaChangedSignature, float, NewStamina);
    UPROPERTY(BlueprintAssignable, Category = "Character|Stamina")
    FOnStaminaChangedSignature OnStaminaChanged;

    /** Called when character state changes */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateChangedSignature, ECharacterState, NewState);
    UPROPERTY(BlueprintAssignable, Category = "Character|State")
    FOnStateChangedSignature OnStateChanged;

    /** Called when character dies */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterDiedSignature);
    UPROPERTY(BlueprintAssignable, Category = "Character|Health")
    FOnCharacterDiedSignature OnCharacterDied;

    ABaseCharacter();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Movement functions
    UFUNCTION(BlueprintCallable, Category = "Movement")
    virtual void MoveForward(float Value);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    virtual void MoveRight(float Value);

    // State management functions
    UFUNCTION(BlueprintCallable, Category = "State")
    virtual void StartSprint();

    UFUNCTION(BlueprintCallable, Category = "State")
    virtual void StopSprint();

    UFUNCTION(BlueprintCallable, Category = "State")
    virtual void StartCrouch();

    UFUNCTION(BlueprintCallable, Category = "State")
    virtual void StopCrouch();

    virtual void Jump() override;

    virtual void StopJumping() override;

    // Camera control functions
    UFUNCTION(BlueprintCallable, Category = "Camera")
    virtual void RotateCamera(float Value);

    UFUNCTION(BlueprintCallable, Category = "Camera")
    virtual void ZoomCamera(float Value);

    UFUNCTION(BlueprintCallable, Category = "Camera")
    virtual void ResetCamera();

    // State check functions
    UFUNCTION(BlueprintCallable, Category = "State")
    virtual bool IsAlive() const;

    UFUNCTION(BlueprintCallable, Category = "State")
    virtual bool CanSprint() const;
    
    virtual bool CanCrouch() const override;

    /** Check if character can jump with custom conditions */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    virtual bool CanCharacterJump() const;

    // Input handlers
    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void OnMove(const FInputActionValue& Value);

    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void OnLook(const FInputActionValue& Value);

    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void OnJumpStarted();

    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void OnJumpCompleted();

    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void OnSprintStarted();

    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void OnSprintCompleted();

    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void OnCrouchStarted();

    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void OnCrouchCompleted();

    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void OnCameraRotate(const FInputActionValue& Value);

    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void OnCameraZoom(const FInputActionValue& Value);

    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void OnCameraReset();

    /** Animation Layer Variables */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation|Layers")
    bool bIsHit;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation|Layers")
    FVector HitDirection;

    /** Animation Layer Functions */
    UFUNCTION(BlueprintCallable, Category = "Animation|Layers")
    void PlayHitReaction(const FVector& InHitDirection);

protected:
    // Initialization functions
    virtual void InitializeMovementProperties();
    virtual void InitializeStateProperties();
    virtual void InitializeCamera();
    virtual void InitializeAnimation();
    virtual void InitializeInputBindings();

    // Movement properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float WalkSpeed = 600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float SprintSpeed = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float CrouchSpeed = 300.0f;

    // Health and stamina properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MaxHealth = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float CurrentHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MaxStamina = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float CurrentStamina;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float StaminaRegenRate = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float SprintStaminaCost = 20.0f;

    // Camera properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float DefaultCameraDistance = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float CrouchCameraDistance = 200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float CameraRotationRate = 45.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float CameraPitchAngle = -15.0f;

    // Camera components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* FollowCamera;

    // Animation properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* JumpMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* CrouchMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* SprintMontage;

    // Input properties
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* SprintAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* CrouchAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* CameraRotateAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* CameraZoomAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* CameraResetAction;

    // State flags
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
    bool bIsSprinting;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
    bool bIsCrouching;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
    bool bIsJumping;

    /** Current character state */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|State")
    ECharacterState CurrentState;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
    bool bIsDead;

    /** Set character state */
    UFUNCTION(BlueprintCallable, Category = "Character|State")
    void SetState(ECharacterState NewState);

    /** Update movement state based on current conditions */
    UFUNCTION(BlueprintCallable, Category = "Character|State")
    void UpdateMovementState();

    /** Called when hit reaction ends */
    UFUNCTION(BlueprintCallable, Category = "Animation|Layers")
    void OnHitReactionEnded();

    // TODO: Add CombatComponent in Phase 2
    // TODO: Add InventoryComponent in Phase 2
    // TODO: Add AbilitySystemComponent in Phase 3
    // TODO: Add AIPerceptionComponent in Phase 3
}; 