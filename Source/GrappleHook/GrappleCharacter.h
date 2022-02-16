// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Particles/ParticleSystemComponent.h"
#include "Math/Vector.h"
#include "MyGrappleLine.h"
#include "GrappleCharacter.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class GRAPPLEHOOK_API AGrappleCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGrappleCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	UFUNCTION(BlueprintCallable)
		bool LineTraceMethod(FHitResult& OutHit);	
		
	UFUNCTION(BlueprintCallable)
		void SetHookLocationPull(FHitResult OutHit, bool hit);

	UFUNCTION(BlueprintCallable)
		void SetHookLocationSwing(FHitResult OutHit, bool hit);

	UFUNCTION(BlueprintCallable)
		void MoveGrapplePlayer();

	UFUNCTION(BlueprintCallable)
		void StopGrapple();
	
	UFUNCTION(BlueprintCallable)
		void BunnyHop();

	UFUNCTION(BlueprintCallable)
		void StopBunnyHop();

	UFUNCTION(BlueprintCallable)
		void AdditionalForce();

	UFUNCTION(BlueprintCallable)
		void Sprint();
	
	UFUNCTION(BlueprintCallable)
		void Walk();

	UFUNCTION(BlueprintCallable)
		FVector HookDirection();

	UFUNCTION(BlueprintCallable)
		float FindMiddle(FVector direction);

	UFUNCTION(BlueprintCallable)
		FVector FindForce(FVector direction, float dot);
	
	UFUNCTION(BlueprintCallable)
		void GrappleSwing();

	UFUNCTION(BlueprintCallable)
		void GrappleTick();


	// Variables used

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool hooked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool hookFinished;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool swing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector hookLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float grappleRange = 10000.f; // How far the grappling hook can be shot

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		UCameraComponent* camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
		USkeletalMeshComponent* gunComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMyGrappleLine* m_grappleLine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float walkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float sprintSpeed;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Create sound for grapple hook
	// Create particle effect system for when grapple attaches

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
