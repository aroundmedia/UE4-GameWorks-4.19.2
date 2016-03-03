// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "VREditorModule.h"
#include "VREditorUniformScaleGizmoHandle.h"
#include "VREditorBaseTransformGizmo.h"
#include "VREditorMode.h"
#include "UnitConversion.h"

UVREditorUniformScaleGizmoHandleGroup::UVREditorUniformScaleGizmoHandleGroup()
	: Super()
{
	// Setup uniform scaling
	UStaticMesh* UniformScaleMesh = nullptr;
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> ObjectFinder( TEXT( "/Engine/VREditor/TransformGizmo/UniformScaleHandle" ) );
		UniformScaleMesh = ObjectFinder.Object;
		check( UniformScaleMesh != nullptr );
	}

	UStaticMeshComponent* UniformScaleHandle = CreateDefaultSubobject<UStaticMeshComponent>( TEXT( "UniformScaleHandle" ) );
	check( UniformScaleHandle != nullptr );

	const bool bAllowGizmoLighting = false;	// @todo vreditor: Not sure if we want this for gizmos or not yet.  Needs feedback.  Also they're translucent right now.

	UniformScaleHandle->SetStaticMesh( UniformScaleMesh );
	UniformScaleHandle->SetMobility( EComponentMobility::Movable );
	UniformScaleHandle->AttachTo( this );

	UniformScaleHandle->SetCollisionEnabled( ECollisionEnabled::QueryOnly );
	UniformScaleHandle->SetCollisionResponseToAllChannels( ECR_Ignore );
	UniformScaleHandle->SetCollisionResponseToChannel( ECC_EditorGizmo, ECollisionResponse::ECR_Block );

	UniformScaleHandle->bGenerateOverlapEvents = false;
	UniformScaleHandle->SetCanEverAffectNavigation( false );
	UniformScaleHandle->bCastDynamicShadow = bAllowGizmoLighting;
	UniformScaleHandle->bCastStaticShadow = false;
	UniformScaleHandle->bAffectDistanceFieldLighting = bAllowGizmoLighting;
	UniformScaleHandle->bAffectDynamicIndirectLighting = bAllowGizmoLighting;

	FVREditorGizmoHandle& NewHandle = *new( Handles ) FVREditorGizmoHandle();
	NewHandle.HandleMesh = UniformScaleHandle;
}

void UVREditorUniformScaleGizmoHandleGroup::UpdateGizmoHandleGroup( const FTransform& LocalToWorld, const FBox& LocalBounds, const FVector ViewLocation, bool bAllHandlesVisible, class UActorComponent* DraggingHandle, const TArray< UActorComponent* >& HoveringOverHandles, float AnimationAlpha, float GizmoScale, const float GizmoHoverScale, const float GizmoHoverAnimationDuration, bool& bOutIsHoveringOrDraggingThisHandleGroup )
{
	// Call parent implementation (updates hover animation)
	Super::UpdateGizmoHandleGroup( LocalToWorld, LocalBounds, ViewLocation, bAllHandlesVisible, DraggingHandle, HoveringOverHandles, AnimationAlpha, GizmoScale, GizmoHoverScale, GizmoHoverAnimationDuration, bOutIsHoveringOrDraggingThisHandleGroup );

	FVREditorGizmoHandle& Handle = Handles[ 0 ];
	UStaticMeshComponent* UniformScaleHandle = Handle.HandleMesh;
	if (UniformScaleHandle != nullptr)	// Can be null if no handle for this specific placement
	{
		FVector HandleRelativeLocation = LocalBounds.GetCenter();
		UniformScaleHandle->SetRelativeLocation( HandleRelativeLocation );

		float GizmoHandleScale = GizmoScale;

		// Make the handle bigger while hovered (but don't affect the offset -- we want it to scale about it's origin)
		GizmoHandleScale *= FMath::Lerp( 1.0f, GizmoHoverScale, Handle.HoverAlpha );

		UniformScaleHandle->SetRelativeScale3D( FVector( GizmoHandleScale ) );

		// Update material
		{
			if (!UniformScaleHandle->GetMaterial( 0 )->IsA( UMaterialInstanceDynamic::StaticClass() ))
			{
				UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create( GizmoMaterial, this );
				UniformScaleHandle->SetMaterial( 0, MID );
			}
			if (!UniformScaleHandle->GetMaterial( 1 )->IsA( UMaterialInstanceDynamic::StaticClass() ))
			{
				UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create( TranslucentGizmoMaterial, this );
				UniformScaleHandle->SetMaterial( 1, MID );
			}
			UMaterialInstanceDynamic* MID0 = CastChecked<UMaterialInstanceDynamic>( UniformScaleHandle->GetMaterial( 0 ) );
			UMaterialInstanceDynamic* MID1 = CastChecked<UMaterialInstanceDynamic>( UniformScaleHandle->GetMaterial( 1 ) );

			ABaseTransformGizmo* GizmoActor = CastChecked<ABaseTransformGizmo>( GetOwner() );
			if( GizmoActor )
			{
				FVREditorMode* Mode =GizmoActor->GetOwnerMode();
				if( Mode )
				{
					FLinearColor HandleColor = Mode->GetColor( FVREditorMode::EColors::WhiteGizmoColor );
					if (UniformScaleHandle == DraggingHandle)
					{
						HandleColor = Mode->GetColor( FVREditorMode::EColors::DraggingGizmoColor );
					}
					else
					{
						HandleColor = VREd::GizmoColor::WhiteGizmoColor;

						if (HoveringOverHandles.Contains( UniformScaleHandle ))
						{
							HandleColor = FLinearColor::LerpUsingHSV( HandleColor, Mode->GetColor( FVREditorMode::EColors::HoverGizmoColor ), Handle.HoverAlpha );
						}
					}

					MID0->SetVectorParameterValue( "Color", HandleColor );
					MID1->SetVectorParameterValue( "Color", HandleColor );
				}
			}
		}
	}
}

ETransformGizmoInteractionType UVREditorUniformScaleGizmoHandleGroup::GetInteractionType() const
{
	return ETransformGizmoInteractionType::UniformScale;
}

EGizmoHandleTypes UVREditorUniformScaleGizmoHandleGroup::GetHandleType() const
{
	return EGizmoHandleTypes::Scale;
}