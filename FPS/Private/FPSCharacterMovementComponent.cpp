﻿#include "FPSCharacterMovementComponent.h"
#include "GameFramework/Character.h"

bool UFPSCharacterMovementComponent::FSavedMove_FPS::CanCombineWith(const FSavedMovePtr& NewMove,
                                                                    ACharacter* InCharacter, float MaxDelta) const
{
	FSavedMove_FPS* NewFPSMove = static_cast<FSavedMove_FPS*>(NewMove.Get());

	if(Saved_bWantsToSprint != NewFPSMove->Saved_bWantsToSprint)
	{
		return false;
	}
	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
		
}

void UFPSCharacterMovementComponent::FSavedMove_FPS::Clear()
{
	FSavedMove_Character::Clear();

	Saved_bWantsToSprint = 0;
}

uint8 UFPSCharacterMovementComponent::FSavedMove_FPS::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if(Saved_bWantsToSprint) Result |= FLAG_Custom_0;

	return Result;
}

void UFPSCharacterMovementComponent::FSavedMove_FPS::SetMoveFor(ACharacter* C, float InDeltaTime,
	FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	UFPSCharacterMovementComponent* CharacterMovement = Cast<UFPSCharacterMovementComponent>(C->GetCharacterMovement());

	Saved_bWantsToSprint = CharacterMovement->Safe_bWantsToSprint;
}

void UFPSCharacterMovementComponent::FSavedMove_FPS::PrepMoveFor(ACharacter* C)
{
	FSavedMove_Character::PrepMoveFor(C);

	UFPSCharacterMovementComponent* CharacterMovement = Cast<UFPSCharacterMovementComponent>(C->GetCharacterMovement());

	CharacterMovement->Safe_bWantsToSprint = Saved_bWantsToSprint;
}

UFPSCharacterMovementComponent::FNetworkPredictionData_Client_FPS::FNetworkPredictionData_Client_FPS(
	const UFPSCharacterMovementComponent& ClientMovement) : Super(ClientMovement)
{
}

FSavedMovePtr UFPSCharacterMovementComponent::FNetworkPredictionData_Client_FPS::AllocateNewMove()
{
	return  FSavedMovePtr(new FSavedMove_FPS());
}

FNetworkPredictionData_Client* UFPSCharacterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != nullptr)

	if(ClientPredictionData == nullptr)
	{
		UFPSCharacterMovementComponent* MutableThis = const_cast<UFPSCharacterMovementComponent*>(this);
		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_FPS(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}
	return ClientPredictionData;
}

void UFPSCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	Safe_bWantsToSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

void UFPSCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation,
	const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	if(MovementMode == MOVE_Walking)
	{
		if(Safe_bWantsToSprint)
		{
			MaxWalkSpeed = Sprint_MaxWalkSpeed;
		}
		else
		{
			MaxWalkSpeed = Walk_MaxWalkSpeed;
		}
	}
	
}

UFPSCharacterMovementComponent::UFPSCharacterMovementComponent()
{
	Sprint_MaxWalkSpeed = 100;
	Walk_MaxWalkSpeed = 100;
	Safe_bWantsToSprint = false;
}

void UFPSCharacterMovementComponent::SprintPressed()
{
	Safe_bWantsToSprint = true;
}

void UFPSCharacterMovementComponent::SprintRelease()
{
	Safe_bWantsToSprint = false;
}
