// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Toolkits/AssetEditorToolkit.h"


enum class EMapChangeType : uint8;
class FTabManager;
class ILevelViewport;
class ISequencer;
class IToolkitHost;
class SWidget;
class UActorAnimation;
class UWorld;
struct FSequencerViewParams;


/**
 * Implements an Editor toolkit for actor animations.
 */
class FActorAnimationEditorToolkit
	: public FAssetEditorToolkit
{ 
public:

	/**
	 * Creates and initializes a new instance.
	 *
	 * @param InStyle The style set to use.
	 */
	FActorAnimationEditorToolkit(const TSharedRef<ISlateStyle>& InStyle);

	/** Virtual destructor */
	virtual ~FActorAnimationEditorToolkit();

public:

	/**
	 * Initialize this asset editor.
	 *
	 * @param Mode Asset editing mode for this editor (standalone or world-centric).
	 * @param InViewParams Parameters for how to view sequencer UI.
	 * @param InitToolkitHost When Mode is WorldCentric, this is the level editor instance to spawn this editor within.
	 * @param ActorAnimation The animation to edit.
	 * @param TrackEditorDelegates Delegates to call to create auto-key handlers for this sequencer.
	 * @param bEditWithinLevelEditor Whether or not sequencer should be edited within the level editor.
	 */
	void Initialize(const EToolkitMode::Type Mode, const FSequencerViewParams& InViewParams, const TSharedPtr<IToolkitHost>& InitToolkitHost, UActorAnimation* ActorAnimation, bool bEditWithinLevelEditor);

public:

	// IToolkit interface

	virtual FText GetBaseToolkitName() const override;
	virtual FName GetToolkitFName() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;

	void UpdateViewports(AActor* ActorToViewThrough) const;

private:

	/** Callback for executing the Add Component action. */
	void HandleAddComponentActionExecute(UActorComponent* Component);

	/** Callback for map changes. */
	void HandleMapChanged(UWorld* NewWorld, EMapChangeType MapChangeType);

	/** Callback for the menu extensibility manager. */
	TSharedRef<FExtender> HandleMenuExtensibilityGetExtender(const TSharedRef<FUICommandList> CommandList, const TArray<UObject*> ContextSensitiveObjects);

	/** Callback for spawning tabs. */
	TSharedRef<SDockTab> HandleTabManagerSpawnTab(const FSpawnTabArgs& Args);	

	/** Callback for the track menu extender. */
	void HandleTrackMenuExtensionAddTrack(FMenuBuilder& AddTrackMenuBuilder, TArray<UObject*> ContextObjects);

private:

	/** The sequencer used by this editor. */
	TSharedPtr<ISequencer> Sequencer;

	/** A map of all the transport controls to viewports that this sequencer has made */
	TMap<TWeakPtr<ILevelViewport>, TSharedPtr<SWidget>> TransportControls;

	FDelegateHandle SequencerExtenderHandle;

	/** Pointer to the style set to use for toolkits. */
	TSharedRef<ISlateStyle> Style;

private:

	/**	The tab ids for all the tabs used */
	static const FName SequencerMainTabId;
};
