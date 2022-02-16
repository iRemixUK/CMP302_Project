// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CableComponent.h"
#include "MyGrappleLine.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class GRAPPLEHOOK_API UMyGrappleLine : public UCableComponent
{

	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
		 void MoveRope(FVector hookLocation, bool &hookFinished);

	UFUNCTION(BlueprintCallable)
		void UpdateRope(FVector hookLocation);
};
