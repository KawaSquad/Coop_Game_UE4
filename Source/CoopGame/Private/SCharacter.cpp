// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameframeWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mSpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	mSpringArmComp->SetupAttachment(RootComponent);
	mSpringArmComp->bUsePawnControlRotation = true;
	mCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mCameraComp->SetupAttachment(mSpringArmComp);
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASCharacter::MoveForward(float value)
{
	AddMovementInput(GetActorForwardVector() * value);
}
void ASCharacter::MoveRight(float value)
{
	AddMovementInput(GetActorRightVector() * value);
}

void ASCharacter::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void ASCharacter::LookRight(float value)
{
	AddControllerYawInput(value);
}

void ASCharacter::BeginCrouch()
{
	Crouch();
}

void ASCharacter::EndCrouch()
{
	UnCrouch();
}


void ASCharacter::Fire()
{
	/*
	AActor * myOwner = GetOwner();
	if (myOwner)
	{

		FHitResult hitResult;
//		GetWorld()->LineTraceSingleByChannel(hitResult, GetActorLocation(), GetActorLocation() + GetActorForwardVector(), ECollisionEnabled.QueryAndPhysics, CollisionQue)
	}
	*/
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &ASCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &ASCharacter::LookRight);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASCharacter::Fire);

}

FVector ASCharacter::GetPawnViewLocation() const
{
	if (mCameraComp)
	{
		return mCameraComp->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

