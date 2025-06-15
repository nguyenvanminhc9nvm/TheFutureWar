#include "BaseHUD.h"
#include "Blueprint/UserWidget.h"

ABaseHUD::ABaseHUD()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ABaseHUD::BeginPlay()
{
    Super::BeginPlay();
    InitializeUI();
}

void ABaseHUD::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABaseHUD::DrawHUD()
{
    Super::DrawHUD();
}

void ABaseHUD::ShowWidget(TSubclassOf<UUserWidget> WidgetClass)
{
    if (WidgetClass)
    {
        UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
        if (Widget)
        {
            Widget->AddToViewport();
            ActiveWidgets.Add(Widget);
        }
    }
}

void ABaseHUD::HideWidget(TSubclassOf<UUserWidget> WidgetClass)
{
    for (int32 i = ActiveWidgets.Num() - 1; i >= 0; i--)
    {
        if (ActiveWidgets[i] && ActiveWidgets[i]->GetClass() == WidgetClass)
        {
            ActiveWidgets[i]->RemoveFromParent();
            ActiveWidgets.RemoveAt(i);
        }
    }
}

void ABaseHUD::ShowNotification(const FString& Message, float Duration)
{
    if (NotificationWidgetClass)
    {
        UUserWidget* NotificationWidget = CreateWidget<UUserWidget>(GetWorld(), NotificationWidgetClass);
        if (NotificationWidget)
        {
            NotificationWidget->AddToViewport();
            ActiveWidgets.Add(NotificationWidget);

            // TODO: Implement notification display logic
            // This could involve setting the message text and handling the duration
        }
    }
}

void ABaseHUD::InitializeUI()
{
    // Khởi tạo UI cơ bản
    if (MainWidgetClass)
    {
        ShowWidget(MainWidgetClass);
    }
} 