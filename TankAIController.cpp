// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"

ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	auto PlayerTank = GetPlayerTank();

	if (!PlayerTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("ATankAIController can't find player tank "));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ATankAIController found player: %s "), *(PlayerTank->GetName()));
	}
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetPlayerTank())
	{
	// TODO move towards the player 

	// aim towards the player
	GetControlledTank()->AimAt(GetPlayerTank()->GetActorLocation());

	// fire towords the player
	}

}

ATank* ATankAIController::GetPlayerTank() const
{
	auto PlayerPown = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PlayerPown)
	{
		return nullptr;
	}
	return Cast<ATank>(PlayerPown);
}
