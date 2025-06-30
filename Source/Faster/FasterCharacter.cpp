// Copyright Epic Games, Inc. All Rights Reserved.

#include "FasterCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Public/Components/ThrowComponent.h"
#include "Public/Pickup/ScorePickUp.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AFasterCharacter::AFasterCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); 
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
	
	SelectedThrowMesh = CreateDefaultSubobject<UStaticMeshComponent>("SelectedThrowMesh");
	ThrowComponent = CreateDefaultSubobject<UThrowComponent>("ThrowComponent");

	SetReplicates(true);
}

void AFasterCharacter::BeginPlay()
{
	Super::BeginPlay();

	FasterGameState = Cast<AFasterGameStateBase>(GetWorld()->GetGameState());
	
	const FAttachmentTransformRules AttachmentRules(
		EAttachmentRule::SnapToTarget, 
		EAttachmentRule::SnapToTarget, 
		EAttachmentRule::KeepRelative, 
		true                         
	);
	
	SelectedThrowMesh->AttachToComponent(GetMesh1P(), AttachmentRules, FName(TEXT("PickupPreviewSocket")));

	ThrowComponent->OnSelectedItemChanged.AddDynamic(this, &AFasterCharacter::OnSelectedItemChanged);
	FasterGameState->OnChangeCanThrow.AddDynamic(this, &AFasterCharacter::OnCanThrowItem);
	
	OnSelectedItemChanged();
}

void AFasterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFasterCharacter::Move);
		
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFasterCharacter::Look);
		
		EnhancedInputComponent->BindAction(ThrowComponent->ThrowAction, ETriggerEvent::Triggered, ThrowComponent, &UThrowComponent::Client_Throw);
		EnhancedInputComponent->BindAction(ThrowComponent->NextThrowItemAction, ETriggerEvent::Triggered, ThrowComponent, &UThrowComponent::Client_SelectNextItem);
		EnhancedInputComponent->BindAction(ThrowComponent->PrevThrowItemAction, ETriggerEvent::Triggered, ThrowComponent, &UThrowComponent::Client_SelectPreviousItem);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AFasterCharacter::OnSelectedItemChanged()
{
	const auto SelectedThrowClass = ThrowComponent->GetSelectedThrowClass();
	if(!SelectedThrowClass) return;

	const auto ScorePickup = SelectedThrowClass->GetDefaultObject<AScorePickUp>();
	if(!ScorePickup) return;

	const auto ScorePickupStaticMeshComponent = ScorePickup->ScorePickUpStaticMesh;
	if(!ScorePickupStaticMeshComponent) return;
	
	SelectedThrowMesh->SetStaticMesh(ScorePickupStaticMeshComponent->GetStaticMesh());
	SelectedThrowMesh->SetMaterial(0, ScorePickupStaticMeshComponent->GetMaterial(0));
}

void AFasterCharacter::OnCanThrowItem(bool bCanThrowItem)
{
	SelectedThrowMesh->SetVisibility(bCanThrowItem);
}

void AFasterCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AFasterCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}