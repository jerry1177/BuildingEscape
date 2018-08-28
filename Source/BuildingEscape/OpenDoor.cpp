// Copy right Jerry Maldonado 2018

#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	if(!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("There is no pressure plait assigned to %s"), *GetOwner()->GetName())
	}	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (DoorIsClosed && GetTotalMassOfActorsOnPlate() >= OpenDoorMass) {
		OnOpen.Broadcast();
		DoorIsClosed = false;		
		UE_LOG(LogTemp, Warning, TEXT("Actor opened door"));
	}
	else if (!DoorIsClosed && GetTotalMassOfActorsOnPlate() < OpenDoorMass) {
		OnClose.Broadcast();
		DoorIsClosed = true;
		UE_LOG(LogTemp, Warning, TEXT("Actor closed door"));
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;

	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors); 
	
	for (const auto* Actor : OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("Name of object: %s"), *Actor->GetName())

	}
	return TotalMass;
}
