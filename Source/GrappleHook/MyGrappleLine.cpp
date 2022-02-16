// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGrappleLine.h"

void UMyGrappleLine::MoveRope(FVector hookLocation, bool& hookFinished)
{
	// Moves and updates the cable
	FVector worldLocation = GetComponentLocation();
	FVector reachedLocation = worldLocation - hookLocation;

	if (reachedLocation.Size() <= 100)
	{
		hookFinished = true;
	}

	if (reachedLocation.Size() > 100)
	{
		SetVisibility(true);
		UpdateRope(hookLocation);
		hookFinished = false;
	}
}

void UMyGrappleLine::UpdateRope(FVector hookLocation)
{
	// Sets the end location
	FVector worldLocation = GetComponentLocation();
	FVector newLocation = FMath::VInterpTo(worldLocation, hookLocation, GetWorld()->GetDeltaSeconds(), 250.f);
	SetWorldLocation(newLocation);
}
