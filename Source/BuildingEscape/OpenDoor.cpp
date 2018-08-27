// Copy right Jerry Maldonado 2018

#include "OpenDoor.h"

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
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
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
	if (PressurePlate->IsOverlappingActor(ActorThatOpens) && DoorIsClosed) {
		OpenOrCloseDoor(OpenAngle);
		DoorIsClosed = false;
		UE_LOG(LogTemp, Warning, TEXT("Actor opened door"));
	}
	else if (!PressurePlate->IsOverlappingActor(ActorThatOpens) && !DoorIsClosed) {
		OpenOrCloseDoor(-OpenAngle);
		DoorIsClosed = true;
		UE_LOG(LogTemp, Warning, TEXT("Actor closed door"));
	}
	

	// ...
}

