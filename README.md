# The Future War - Base Character System

## 1. Tổng quan

Base Character System là nền tảng cho tất cả các nhân vật trong game, cung cấp các chức năng cốt lõi:
- Di chuyển cơ bản (đi bộ, chạy, nhảy, ngồi xổm)
- Quản lý trạng thái (health, stamina)
- Hệ thống camera third-person
- Animation state management
- Input handling với Enhanced Input System

## 2. Kiến trúc

### 2.1. Class Hierarchy
```
ABaseCharacter (Abstract)
├── APlayerCharacter
└── AEnemyCharacter
```

### 2.2. Core Components
- **Movement**: UCharacterMovementComponent
- **Camera**: USpringArmComponent + UCameraComponent
- **Animation**: UAnimInstance (Blueprint)
- **Input**: Enhanced Input System

### 2.3. State Management
- Health/Stamina system
- Movement states (idle, walk, run, jump, crouch)
- Camera states

## 3. Hướng dẫn sử dụng

### 3.1. Tạo Character mới
```cpp
// PlayerCharacter.h
UCLASS()
class THEFUTUREWAR_API APlayerCharacter : public ABaseCharacter
{
    GENERATED_BODY()
public:
    APlayerCharacter();
    
    // Override các hàm cần thiết
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
};

// PlayerCharacter.cpp
APlayerCharacter::APlayerCharacter()
{
    // Khởi tạo các thuộc tính riêng
}
```

### 3.2. Override các hàm quan trọng
```cpp
// Movement
virtual void MoveForward(float Value) override;
virtual void MoveRight(float Value) override;

// State
virtual void StartSprint() override;
virtual void StopSprint() override;
virtual bool CanSprint() const override;

// Camera
virtual void RotateCamera(float Value) override;
virtual void ZoomCamera(float Value) override;
```

### 3.3. Sử dụng Delegates
```cpp
// Bind to delegates
OnHealthChanged.AddDynamic(this, &APlayerCharacter::HandleHealthChanged);
OnStaminaChanged.AddDynamic(this, &APlayerCharacter::HandleStaminaChanged);
OnStateChanged.AddDynamic(this, &APlayerCharacter::HandleStateChanged);
OnCharacterDied.AddDynamic(this, &APlayerCharacter::HandleCharacterDied);
```

## 4. Best Practices

### 4.1. Kế thừa và Mở rộng
- Luôn override các hàm virtual khi cần thay đổi behavior
- Sử dụng `Super::` để gọi phiên bản base class
- Không override các hàm không cần thiết

### 4.2. State Management
- Sử dụng các hàm Can*() để kiểm tra điều kiện
- Cập nhật state thông qua các hàm Start/Stop
- Broadcast events khi state thay đổi

### 4.3. Performance
- Tránh gọi các hàm nặng trong Tick
- Sử dụng timers cho các tác vụ định kỳ
- Cache các component references

## 5. Testing

### 5.1. Unit Tests
- Test các hàm movement cơ bản
- Test state transitions
- Test camera behavior

### 5.2. Integration Tests
- Test với Animation Blueprint
- Test với Input System
- Test với Gameplay Framework

## 6. Mở rộng trong tương lai

### 6.1. Phase 2
- Combat System
- Inventory System
- Advanced Movement

### 6.2. Phase 3
- Ability System
- AI Integration
- Advanced Animation

## 7. Troubleshooting

### 7.1. Common Issues
- Camera jittering: Kiểm tra camera lag và rotation rate
- Animation glitches: Kiểm tra state machine và montages
- Input not working: Kiểm tra Input Mapping Context

### 7.2. Debug Tips
- Enable debug visualization trong Editor
- Sử dụng debug logs cho state changes
- Test với different input devices

## 8. Resources

### 8.1. Documentation
- [Unreal Engine Documentation](https://docs.unrealengine.com)
- [Enhanced Input System](https://docs.unrealengine.com/5.0/en-US/enhanced-input-in-unreal-engine/)
- [Character Movement](https://docs.unrealengine.com/5.0/en-US/character-movement-in-unreal-engine/)

### 8.2. Tools
- Unreal Insights
- Unreal Editor
- Visual Studio/Visual Studio Code

## 9. Contributing

### 9.1. Code Style
- Follow Unreal Engine coding standards
- Use meaningful names
- Add comments for complex logic

### 9.2. Git Workflow
- Create feature branches
- Write descriptive commit messages
- Review code before merging

## 10. Contact

For questions or suggestions, please contact the development team. 