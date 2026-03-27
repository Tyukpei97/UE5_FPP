#include "Components/Interaction/PlayerInteractorComponent.h"
#include "Components/Interaction/InteractableComponent.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"

UPlayerInteractorComponent::UPlayerInteractorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	_searchObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
}

bool UPlayerInteractorComponent::TryInteract()
{
	AActor* _ownerActor = GetOwner();

	if (_ownerActor == nullptr) return false;

	UInteractableComponent* _bestInteractable = FindBestInteractable(); 

	if (_bestInteractable == nullptr) return false;

	_bestInteractable->Interact(_ownerActor);

	return true;
}

UInteractableComponent* UPlayerInteractorComponent::FindBestInteractable() const
{
	AActor* _ownerActor = GetOwner();
	UWorld* _world = GetWorld();

	if (_ownerActor == nullptr || _world == nullptr) return nullptr;

	TArray<AActor*> _ignoredActors;
	_ignoredActors.Add(_ownerActor); // Игнорируем самого себя, чтобы не словить исключение при взаимодействии с самим собой.

	TArray<AActor*> _overlappedActors;

	const bool _foundAny = UKismetSystemLibrary::SphereOverlapActors( // Пройдёмся конкретно по всем передаваемым параметрам.
		_world, //- это мир, в котором мы находимся.
		_ownerActor->GetActorLocation(), //- это точка, от которой мы будем искать объекты. Он же сам владелец компонента, то есть игрок.
		_searchRadius,
		_searchObjectTypes,
		nullptr,  //- это класс актера, который мы хотим найти(мы передаём nullptr, потому что нам не важен класс актера)
		_ignoredActors, // - это массив акторов, которых мы хотим игнорировать при поиске(мы передаём массив с самим собой)
		_overlappedActors); //- _overlappedActors - это массив акторов, в который будут записаны найденные объекты.

	if (!_foundAny) return nullptr;

	UInteractableComponent* _bestInteractable = nullptr; 

	float _bestDistanceSquared = TNumericLimits<float>::Max(); // Короче, для старта поиска ближайшего объекта, мы задаём максимальное значение для дистанции, чтобы можно было спокойно сравнивать это значение с реальными дистанциями до найденных объектов.

	for (AActor* _candidateActor : _overlappedActors) 
	{
		if (_candidateActor == nullptr) continue;

		// Ищем компонент взаимодействия, если нашли, то записываем в переменную и сейвим. Если после дистанция этого актора будет ближе чем у другого "лучшего" объекта, то мы его и будем сохранять как лучший объект для взаимодействия.
		UInteractableComponent* _interactable = _candidateActor->FindComponentByClass<UInteractableComponent>();
		if (_interactable == nullptr) continue;

		if (!_interactable->CanInteraction(_ownerActor)) continue;
		
		const float _distanceSquared = FVector::DistSquared(
			_ownerActor->GetActorLocation(),
			_candidateActor->GetActorLocation()); // Ищем ближайший объект, с которым можно взаимодействовать, чтобы при наличии нескольких объектов, находящихся в радиусе, взаимодействовать с самым близким.

		if (_distanceSquared < _bestDistanceSquared) 
		{
			_bestDistanceSquared = _distanceSquared;
			_bestInteractable = _interactable;
		}
	}

	return _bestInteractable;
}

