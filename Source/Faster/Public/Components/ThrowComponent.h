// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameStates/FasterGameStateBase.h"
#include "ThrowComponent.generated.h"

class AFasterGameMode;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSelectedItemChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnThrowItem);

class AFasterCharacter;
class AScorePickUp;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FASTER_API UThrowComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UThrowComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* ThrowAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* NextThrowItemAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* PrevThrowItemAction;
	
	void Client_Throw();
	void Client_SelectNextItem();
	void Client_SelectPreviousItem();
	
	UFUNCTION(BlueprintPure, Category = "Inventory")
	TSubclassOf<AScorePickUp> GetSelectedThrowClass() const {return SelectedPickupThrowClass;};

	UPROPERTY()
	FOnSelectedItemChanged OnSelectedItemChanged;

	UPROPERTY()
	FOnThrowItem OnThrowItem;
	
protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(Server, Reliable)
	void Server_Throw();

	UFUNCTION(Server, Reliable)
	void Server_SelectNextItem();

	UFUNCTION(Server, Reliable)
	void Server_SelectPreviousItem();
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TArray<TSubclassOf<AScorePickUp>> PickupThrowClasses;

private:
	UPROPERTY(ReplicatedUsing = OnRep_PickupThrowClass)
	TSubclassOf<AScorePickUp> SelectedPickupThrowClass = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* ThrowAnimation;
	
	UPROPERTY()
	bool bCanThrow = true;

	UPROPERTY()
	AFasterCharacter* OwnerCharacter;

	UPROPERTY()
	AFasterGameStateBase* FasterGameStateBase;
	
	bool CanThrow() const;

	UPROPERTY(ReplicatedUsing=OnRep_CountThrowAnimation)
	int32 CountThrowAnimation = 0;
	
	int32 PickupThrowClassesIndex = 0;

	UPROPERTY()
	AFasterGameMode* FasterGameMode;
	
	UFUNCTION()
	void OnRep_PickupThrowClass() const;

	UFUNCTION()
	void OnRep_CountThrowAnimation() const;
	
	void PlayThrowAnimation() const;

	void Equip();
};
