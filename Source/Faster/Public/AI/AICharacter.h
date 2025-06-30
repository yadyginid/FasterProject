// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTakenItem);

UCLASS()
class FASTER_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AAICharacter();

	virtual void Tick(float DeltaTime) override;
	
	void SetMovementSpeed(float NewSpeed);

	UFUNCTION(Blueprintable)
	float GetMoveSpeed() const {return MovementSpeed;}

	UPROPERTY(BlueprintAssignable)
	FOnTakenItem OnTakenItem;
	
	UFUNCTION()
	void OnItemTaken() const;

	UPROPERTY(ReplicatedUsing=OnRep_ChangeMoveSpeed)
	float MovementSpeed = 300.f;

	bool bEnableRotate = true;

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY()
	APawn* LocalPlayer;

	UFUNCTION()
	void OnRep_ChangeMoveSpeed();
};
