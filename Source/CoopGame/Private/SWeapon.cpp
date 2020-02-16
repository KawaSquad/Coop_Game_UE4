// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"

// Sets default values
ASWeapon::ASWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	mMuzzleSocketName = "MuzzleSocketName";
	mWeaponRange = 1000;
}

// Called when the game starts or when spawned
void ASWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASWeapon::Fire()
{
	AActor * myOwner = GetOwner();
	if (myOwner)
	{
		FVector eyeLocation;
		FRotator eyeRotator;
		myOwner->GetActorEyesViewPoint(eyeLocation, eyeRotator);

		FVector shootDirection = eyeRotator.Vector();
		FVector traceEnd = eyeLocation + (shootDirection * mWeaponRange);

		FCollisionQueryParams queryParams;
		queryParams.AddIgnoredActor(myOwner);
		queryParams.AddIgnoredActor(this);
		queryParams.bTraceComplex = true;

		//smokeEnd
		FVector traceEndPoint = traceEnd;

		if (mThrowable)
		{
			FVector muzzleLocation = MeshComp->GetSocketLocation(mMuzzleSocketName);
			AActor * throwable = GetWorld()->SpawnActor<AActor>(mThrowable, muzzleLocation,GetActorRotation());

			UPrimitiveComponent * baseMesh = Cast<UPrimitiveComponent>(throwable->GetRootComponent());
			if (baseMesh)
			{
				baseMesh->AddForce(shootDirection*mWeaponRange);
			}
		}
		else
		{

			FHitResult hitResult;
			if (GetWorld()->LineTraceSingleByChannel(hitResult, eyeLocation, traceEnd, ECC_Visibility, queryParams))
			{
				//Blocking Hit => Damage
				AActor * hitActor = hitResult.GetActor();
				UGameplayStatics::ApplyPointDamage(hitActor, mBaseDamage, shootDirection, hitResult, myOwner->GetInstigatorController(), this, mDamageType);

				if (mImpact)
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), mImpact, hitResult.ImpactPoint, hitResult.ImpactNormal.Rotation(), true);

				traceEndPoint = hitResult.ImpactPoint;
			}
			//DrawDebugLine(GetWorld(), eyeLocation, traceEnd, FColor::White,false,1.0f);

			if (mMuzzle)
			{
				UGameplayStatics::SpawnEmitterAttached(mMuzzle, MeshComp, mMuzzleSocketName);
			}
			if (mSmokeTrail)
			{
				FVector muzzleLocation = MeshComp->GetSocketLocation(mMuzzleSocketName);
				UParticleSystemComponent * smokeTrailComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), mSmokeTrail, muzzleLocation);
				if (smokeTrailComp)
				{
					smokeTrailComp->SetVectorParameter("BeamEnd", traceEndPoint);
				}
			}
		}
	}
}


// Called every frame
void ASWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

