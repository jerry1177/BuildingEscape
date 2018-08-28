// Copy right Jerry Maldonado 2018

#include "Grabber.h"

#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
	// ...
	
}

/// Find physics handle
void UGrabber::FindPhysicsHandleComponent() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("%s Missing physics handler!"), *GetOwner()->GetName())
	}
}

/// Look for input component (only appears on runtime) and bind grab buttons 
void UGrabber::SetupInputComponent() {
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Found InputComponent!"))
		InputComponent->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", EInputEvent::IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s Missing Input Component!"), *GetOwner()->GetName())
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);	
	if (PhysicsHandle->GetGrabbedComponent()) {
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab key pressed!"))
	
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();

	/// If we hit something then attach a physics handle
	if (ActorHit) {
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
	}
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grab key released!"))
	PhysicsHandle->ReleaseComponent();
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const {	
	/// setup query parameters
	FCollisionQueryParams QueryParam(FName(TEXT("")), false, GetOwner());

	///LineTrace (AKA RayCast) out to reach
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		QueryParam
	);

	return HitResult;
}

FVector UGrabber::GetReachLineStart() const {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	/// Set player view point and player view rotation
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd() const {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	/// Set player view point and player view rotation
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	
	/// Return End Point
	return PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
}