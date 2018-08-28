// Copy right Jerry Maldonado 2018

#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	
	// ...
	
}

void UOpenDoor::OpenOrCloseDoor(float OpenAngle)
{
	AActor* door = GetOwner();
	FRotator DRotation = door->GetActorRotation();
	FRotator NewRotation = FRotator(DRotation.Roll, DRotation.Yaw - OpenAngle, DRotation.Pitch);
	door->SetActorRotation(NewRotation);
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (GetTotalMassOfActorsOnPlate() > 50.f) {
		TimerStarted = false;
	}
	
	if (DoorIsClosed && GetTotalMassOfActorsOnPlate() >= OpenDoorMass) {
		OpenOrCloseDoor(OpenAngle);
		DoorIsClosed = false;
		
		UE_LOG(LogTemp, Warning, TEXT("Actor opened door"));
	}
	else if (!DoorIsClosed && !TimerStarted && GetTotalMassOfActorsOnPlate() <OpenDoorMass) {
		DoorLastOpenTime = GetWorld()->GetTimeSeconds();
		TimerStarted = true;
		UE_LOG(LogTemp, Warning, TEXT("Timer has started"));
		
	}
	else if (!DoorIsClosed && TimerStarted && GetTotalMassOfActorsOnPlate() < OpenDoorMass && GetWorld()->GetRealTimeSeconds() - DoorLastOpenTime > DoorCloseDelay) {
		OpenOrCloseDoor(-OpenAngle);
		TimerStarted = false;
		DoorIsClosed = true;
		UE_LOG(LogTemp, Warning, TEXT("Actor closed door"));
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	for (const auto* Actor : OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("Name of object: %s"), *Actor->GetName())

	}
	return TotalMass;
}
