#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "Slate/SlateVectorArtData.h"
#include "BaseHUD.generated.h"

/**
 * BaseHUD - Class cơ sở cho tất cả các HUD trong game
 * 
 * Class này cung cấp các chức năng cơ bản cho việc hiển thị UI:
 * - Quản lý các widget UI
 * - Hiển thị thông tin người chơi
 * - Hiển thị thông báo
 * - Xử lý các sự kiện UI
 * 
 * Các class con có thể kế thừa và mở rộng thêm các chức năng đặc thù
 */
UCLASS(Blueprintable, Abstract)
class THEFUTUREWAR_API ABaseHUD : public AHUD
{
    GENERATED_BODY()

public:
    // Constructor
    ABaseHUD();

    // Override các hàm cơ bản của HUD
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void DrawHUD() override;

    // ===== UI Management =====
    // Hiển thị widget
    UFUNCTION(BlueprintCallable, Category = "UI")
    virtual void ShowWidget(TSubclassOf<UUserWidget> WidgetClass);

    // Ẩn widget
    UFUNCTION(BlueprintCallable, Category = "UI")
    virtual void HideWidget(TSubclassOf<UUserWidget> WidgetClass);

    // Hiển thị thông báo
    UFUNCTION(BlueprintCallable, Category = "UI")
    virtual void ShowNotification(const FString& Message, float Duration = 3.0f);

protected:
    // ===== Properties =====
    // Widget chính
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> MainWidgetClass;

    // Widget thông báo
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UUserWidget> NotificationWidgetClass;

    // Danh sách các widget đang hiển thị
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
    TArray<UUserWidget*> ActiveWidgets;

private:
    // Hàm khởi tạo
    void InitializeUI();
}; 