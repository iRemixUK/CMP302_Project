// CMP302 Assignment 
// Aaron Crawford - 1900400


#include "GrappleCharacter.h"

// Sets default values
AGrappleCharacter::AGrappleCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up capsule component
	GetCapsuleComponent()->InitCapsuleSize(40.f, 95.0f);
	RootComponent = GetCapsuleComponent();

	// Set up camera component
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(GetCapsuleComponent());
	camera->SetRelativeLocation(FVector(-39.65f, 1.75f, 64.f));
	camera->bUsePawnControlRotation = true;

	// Setup mesh component
	GetMesh()->SetupAttachment(camera);
	GetMesh()->SetRelativeLocation((FVector(3.29f, -5.f, -161.3f)));

	// Setup gun component
	//gunComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	//gunComponent->SetupAttachment(GetMesh());

	// Setup cable component
	m_grappleLine = CreateDefaultSubobject<UMyGrappleLine>(TEXT("Grapple line"));
	m_grappleLine->SetupAttachment(RootComponent);

	m_grappleLine->SetVisibility(false);
	m_grappleLine->CableLength = 0.0f;
	m_grappleLine->CableWidth = 6.0f;
	m_grappleLine->EndLocation = FVector(0, 0, 0);
	m_grappleLine->NumSegments = 1;

	// Setup player walk and sprint speed
	walkSpeed = 600.f;
	sprintSpeed = 1000.f;
	GetCharacterMovement()->AirControl = 1.00;

}

// Called when the game starts or when spawned
void AGrappleCharacter::BeginPlay()
{
	Super::BeginPlay();
}

bool AGrappleCharacter::LineTraceMethod(FHitResult& OutHit)
{
	// Do a line trace to see if the player can grapple to geometry
	FVector startPoint = camera->GetComponentLocation();
	FVector endPoint = (camera->GetForwardVector() * grappleRange) + startPoint;
	FCollisionQueryParams Parameters;
	return GetWorld()->LineTraceSingleByChannel(OutHit, startPoint, endPoint, ECC_Visibility, Parameters);
}

void AGrappleCharacter::SetHookLocationPull(FHitResult OutHit, bool hit)
{
	// Set the location of the grapple hook so player can be pulled towards it
	if (hit == true)
	{
		hooked = true;
		hookLocation = OutHit.Location;
	}
	else
	{
		StopGrapple();
	}
}

void AGrappleCharacter::SetHookLocationSwing(FHitResult OutHit, bool hit)
{
	// Set the location of the grapple hook so player can swing
	if (hit == true)
	{
		swing = true;
		hookLocation = OutHit.Location;
	}
	else
	{
		StopGrapple();
	}
}

void AGrappleCharacter::MoveGrapplePlayer()
{
	// Pulls the player towards hook location
	FVector launchVector = hookLocation - GetActorLocation();
	float velocity = GetWorld()->GetDeltaSeconds() * 500.f;
	launchVector = launchVector * velocity;
	this->LaunchCharacter(launchVector, true, true);
}

void AGrappleCharacter::StopGrapple()
{
	// Stops the grapple 
	hooked = false;
	hookFinished = false;
	swing = false;

	// Set cable to be invisible
	m_grappleLine->SetVisibility(false);
	m_grappleLine->SetWorldLocation(GetActorLocation());
}

void AGrappleCharacter::BunnyHop()
{
	if (!GetCharacterMovement()->IsFalling())
	{
		// Character jumps as normal but movement speed is incremented each time the player jumps
		Jump();
		float bhopIncrement = GetCharacterMovement()->MaxWalkSpeed + 200.f;

		// Max movement speed is 3000
		if (bhopIncrement > 3000)
		{
			bhopIncrement = 3000.f;
		}

		// Set walk speed to new speed generated 
		GetCharacterMovement()->MaxWalkSpeed = bhopIncrement;
	}
}

void AGrappleCharacter::StopBunnyHop()
{
	// Stops the bunny hop and resets speed
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	StopJumping();
}

void AGrappleCharacter::AdditionalForce()
{
	// Adds more force to the swing so player can move faster
	FVector force = GetActorForwardVector() * 50000.f;
	GetCharacterMovement()->AddForce(force);
}

void AGrappleCharacter::Sprint()
{
	// Increases walking speed
	GetCharacterMovement()->MaxWalkSpeed = sprintSpeed;
}

void AGrappleCharacter::Walk()
{
	// Resets speed back to normal
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}

FVector AGrappleCharacter::HookDirection()
{
	// Gets the directional vector of hook
	FVector hookDirection = GetActorLocation() - hookLocation;
	return hookDirection;
}

float AGrappleCharacter::FindMiddle(FVector direction)
{
	// Find the middle of the current velocity and direction of hook
	float dot = FVector::DotProduct(GetVelocity(), direction);
	return dot;
}

FVector AGrappleCharacter::FindForce(FVector direction, float dot)
{
	// Normalize vector then multiply with direction to get force that points towards hook location
	direction.Normalize();
	FVector force = direction * dot;
	force = force * -2.f;
	return force;
}

void AGrappleCharacter::GrappleSwing()
{
	// Swings the player around hooked geometry
	FVector hookDirection = HookDirection();
	float dot = FindMiddle(hookDirection);

	FVector force = FindForce(hookDirection, dot);

	GetCharacterMovement()->AddForce(force);
}

void AGrappleCharacter::GrappleTick()
{
	// Does both of the grapple movements when hooked to geometry
	if (hooked == true)
	{
		MoveGrapplePlayer();
	}

	if (swing == true)
	{
		GrappleSwing();
		AdditionalForce();
	}
}

// Called every frame
void AGrappleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (hooked == true || swing == true)
	{
		if (hookFinished == true)
		{
			m_grappleLine->UpdateRope(hookLocation);
		}

		if (hookFinished == false)
		{
			m_grappleLine->MoveRope(hookLocation, hookFinished);
		}
	}
}

// Called to bind functionality to input
void AGrappleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Detach", IE_Pressed, this, &AGrappleCharacter::StopGrapple); // Detaches grapple when right click is pressed 
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AGrappleCharacter::Sprint); // Characters sprints when shift is pressed 
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AGrappleCharacter::Walk); // Resets the character movement speed
}

