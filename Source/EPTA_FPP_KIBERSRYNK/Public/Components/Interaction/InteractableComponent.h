// Fill out your copyright notice in the Description page of Project Settings.
//Вы спросите, почему я не убрал авто-комменты, а я отвечу. Если убрать к примеру комент выше и также убрать пустующие поля, то появится ошибка компиляции. Так что не трогаем.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"


UCLASS( ClassGroup=(Interation), meta=(BlueprintSpawnableComponent) )
class EPTA_FPP_KIBERSRYNK_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractableComponent();

	// Радиус взаимодействия.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interation", meta = (ClampMin = "0.0"))
	float _interactRadius = 200.0f;

	// Для UI. ДЛЯ ТЕБЯ СЛАВОДАН, чтобы текст через принты можно было менять.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interation")
	FText _promptText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interation")
	bool _interactionEnabled = true;

	// Пробуем проверить можно ли с объектом взаимодействовать. Ваще нахуя я этим занимаюсь, максимум что будет, так это конкретный момент с дверью (сомневаюсь) и всё блять. Но я уже начал, так что пусть будет.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interation")
	bool CanInteraction(AActor* _interactor) const;
	virtual bool CanInteraction_Implementation(AActor* _interactor) const;

	// Собственно взаимодействуем.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interation")
	void Interact(AActor* _interactor);
	virtual void Interact_Implementation(AActor* _interactor);

	// Событие для блюпринта, чтобы там уже конкретно прописывать что будет при взаимодействии. Короче, в визуальном пространстве BP вызываем метод и там уже прописываем что будет при взаимодействии.
	UFUNCTION(BlueprintImplementableEvent, Category = "Interation")
	void BluePrintOnInteract(AActor* _interactor);
		
};
