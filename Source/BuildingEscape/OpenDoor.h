// Copy right Jerry Maldonado 2018

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/TriggerVolume.h"
#include "Engine/World.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void OpenOrCloseDoor(float OpenAngle);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	

		
private: 
	UPROPERTY(VisibleAnywhere)
		float OpenAngle = 90.0f;
	UPROPERTY(EditAnywhere)
		float DoorCloseDelay = 1.0f;
	// Mass needed in order for the door to open (kg)
	UPROPERTY(EditAnywhere)
		float OpenDoorMass = 40.0f;
	
	float DoorLastOpenTime;

	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlate = nullptr;

	bool DoorIsClosed = true;
	bool TimerStarted = false;

	//Returns total mass in kg
	float GetTotalMassOfActorsOnPlate();
};
