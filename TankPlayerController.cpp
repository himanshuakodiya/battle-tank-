// Fill out your copyright notice in the Description page of Project Settings.

#include "Battle_Tank.h"
#include "TankPlayerController.h"


void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto ControlledTank = GetControlledTank();

	if (!ControlledTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController not possessing a tank "));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController possessing: %s "), *(ControlledTank->GetName()));

	}
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowordsCrosshair();
	// UE_LOG(LogTemp, Warning, TEXT("ticking"));
}

void ATankPlayerController::AimTowordsCrosshair()
{
	if (!GetControlledTank()) { return; }

	FVector HitLocation; //out perameter
	if (GetSightRayHitLocation(HitLocation)) //has "side-effect", is going to line trace
	{
		GetControlledTank()->AimAt(HitLocation);
	}

}
//get world location of linetrce through crosshair , true if hits landscape 
bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{
	//find the position of crosshair
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	auto ScreenLocation = FVector2D(ViewportSizeX*CrosshairXLocation, ViewportSizeY*CrosshairYLocation);
	///UE_LOG(LogTemp, Warning, TEXT("screenLocation: %s"), *ScreenLocation.ToString());

	//de-project the screen position of the crosshair to a world direction
	FVector WorldLocation, LookDirection;
	if (DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, WorldLocation, LookDirection))
	{
		///UE_LOG(LogTemp, Warning, TEXT("WorldDirection : %s"), *WorldDirection.ToString());

		//line-trace along that look direction , and what we hit (up to max range )
		GetLookVectorHitLocation( LookDirection, HitLocation);
	}
	
	
	return true;
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector & HitLocation) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection*LineTraceRange);
	if (GetWorld()->LineTraceSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation,
			ECollisionChannel::ECC_Visibility)
		)
	{
		HitLocation = HitResult.Location;
		return true;
	}
	HitLocation = FVector(0);
	return false;
}


ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}
