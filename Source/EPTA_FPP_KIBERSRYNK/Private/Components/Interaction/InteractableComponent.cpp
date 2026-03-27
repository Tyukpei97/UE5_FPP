// Удивительно, но здесь это сработало. Абузим.
#include "Components/Interaction/InteractableComponent.h"
#include "GameFramework/Actor.h"


UInteractableComponent::UInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = true; // Я хуй знает чё это, автогенерация скрипта. Как я понял, это что-то вроде Update() в юнити, то есть метод, который будет вызываться каждый кадр.
}

bool UInteractableComponent::CanInteraction_Implementation(AActor* _interactor) const
{
	if (!_interactionEnabled) return false;

	/*Это короче как в юнити получить root объекта. Получаем данные по типу объекта, который будет взаимодействовать.
	Например, если это игрок, то мы будем проверять дистанцию до него, а если это какой-то другой объект, то может быть какие-то другие условия. 
	Но это уже в блюпринте прописывать будем, а здесь просто получаем данные по тому кто взаимодействует и по тому кто владелец компонента.*/
	const AActor* _ownerActor = GetOwner(); 

	if (_ownerActor == nullptr || _interactor == nullptr) return false;

	if (_ownerActor == _interactor) return false; // Удивительная хуйня, оказывается, что если не проверять что владелец компонента и тот кто взаимодействует одна и та же сущность, то можно взаимодействовать с самим собой.

	const float _distanceSquared = FVector::DistSquared(
		_ownerActor->GetActorLocation(),
		_interactor->GetActorLocation());

	return _distanceSquared <= FMath::Square(_interactRadius); // И проверяем дистанцию до объекта, если она меньше или равна радиусу взаимодействия, то возвращаем true, иначе false.
}

void UInteractableComponent::Interact_Implementation(AActor* _interactor)
{
	if (!CanInteraction(_interactor)) return;

	BluePrintOnInteract(_interactor); // Во, теперь если всё на мази то мы вызываем блю принт событие, и уже там в редакторе, делаем что нам нужно будет.
}
