// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

#include "../Components/HealthComponent.h"

#include "../Actors/ProjectileBase.h"

// Sets default values
APawnBase::APawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
    RootComponent = CapsuleComp;

    BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
    BaseMesh->SetupAttachment(RootComponent);

    TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
    TurretMesh->SetupAttachment(BaseMesh);

    ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
    ProjectileSpawnPoint->SetupAttachment(TurretMesh);

    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
}


void APawnBase::RotateTurret(FVector LookAtTarget)
{
    FVector LookAtTargetCleaned = FVector(LookAtTarget.X, LookAtTarget.Y, TurretMesh->GetComponentLocation().Z);
    FVector StartLocation = TurretMesh->GetComponentLocation();

    FRotator TurretRotation = FVector(LookAtTargetCleaned - StartLocation).Rotation();
    TurretMesh->SetWorldRotation(TurretRotation);
}

void APawnBase::Fire()
{
    if (ProjectileClass != nullptr)
    {
        FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
        FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

        AProjectileBase* TempProjectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation);
        TempProjectile->SetOwner(this);
    }
}

void APawnBase::HandleDestruction()
{
    UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, GetActorLocation());
    UGameplayStatics::SpawnSoundAtLocation(this, DeathSound, GetActorLocation());
    GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(DeathShake);
}

