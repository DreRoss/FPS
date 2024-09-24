#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "Coreminimal.h"
#include "FPSCharacterMovementComponent.generated.h"


UCLASS()
class FPS_API UFPSCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	bool Safe_bWantsToSprint;
	
	public:

	UFPSCharacterMovementComponent();
};
