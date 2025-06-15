#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

/**
 * BasePlayerController - Class cơ sở cho tất cả các PlayerController trong game
 * 
 * Class này cung cấp các chức năng cơ bản cho việc điều khiển nhân vật:
 * - Xử lý input cơ bản
 * - Quản lý camera
 * - Tương tác với UI
 * - Xử lý các sự kiện game
 * 
 * Các class con có thể kế thừa và mở rộng thêm các chức năng đặc thù
 */
UCLASS(Blueprintable, Abstract)
class THEFUTUREWAR_API ABasePlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    // Constructor
    ABasePlayerController();

    // Override các hàm cơ bản của PlayerController
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupInputComponent() override;

protected:
    // ===== Input Functions =====
    // Xử lý input di chuyển
    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void HandleMoveForward(float Value);

    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void HandleMoveRight(float Value);

    // Xử lý input nhìn xung quanh
    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void HandleLookHorizontal(float Value);

    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void HandleLookVertical(float Value);

    // Xử lý input nhảy
    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void HandleJump();

    // Xử lý input chạy
    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void HandleSprintStart();

    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void HandleSprintEnd();

    // ===== Camera Functions =====
    // Thiết lập camera
    UFUNCTION(BlueprintCallable, Category = "Camera")
    virtual void SetupCamera();

    // ===== Properties =====
    // Thuộc tính camera
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float MouseSensitivity = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float InvertedYAxis = 1.0f;

private:
    // Hàm khởi tạo
    void InitializeInputBindings();
}; 