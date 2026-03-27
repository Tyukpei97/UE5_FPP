// Вот блять, что за нахуй. Вот видишь строчку GENERATED_BODY(). Эта залупа, очень нежно относится к тому, сколько строчек от этого комента до неё и если их будет на одну больше/меньше, то лови исключение педрила.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerInteractorComponent.generated.h"

class UInteractableComponent;

UCLASS( ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent) )
class EPTA_FPP_KIBERSRYNK_API UPlayerInteractorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerInteractorComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (ClampMin = "0.0"))
	float _searchRadius = 250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	TArray<TEnumAsByte<EObjectTypeQuery>> _searchObjectTypes; // Типы объектов, с которыми можно взаимодействовать. Например: WorldDynamic. Это динамические объекты, аля, двери, ящики и т.д.

	UFUNCTION(BlueprintCallable, Category = "Interation")
	bool TryInteract();


protected:

	UInteractableComponent* FindBestInteractable() const;
		
};
