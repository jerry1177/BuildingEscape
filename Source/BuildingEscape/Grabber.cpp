// Copy right Jerry Maldonado 2018

#include "Grabber.h"

#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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

	if (PhysicsHandle) {
		UE_LOG(LogTemp, Warning, TEXT("Found PhysicsHandler!"))
	}
	else {
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
		PhysicsHandle->SetTargetLocation(GetLineTracedEnd());
	}
	// ...
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
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	/// Set player view point and player view rotation
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	FVector LineEndPoint = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
	
	/// setup query parameters
	FCollisionQueryParams QueryParam(FName(TEXT("")), false, GetOwner());

	///LineTrace (AKA RayCast) out to reach
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(OUT Hit,
		PlayerViewPointLocation,
		LineEndPoint,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		QueryParam
	);

	AActor* ActorHit = Hit.GetActor();
	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Object name is %s"), *ActorHit->GetName())
	}

	return Hit;
}

FVector UGrabber::GetLineTracedEnd() {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	/// Set player view point and player view rotation
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	FVector LineEndPoint = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
	return LineEndPoint;
}