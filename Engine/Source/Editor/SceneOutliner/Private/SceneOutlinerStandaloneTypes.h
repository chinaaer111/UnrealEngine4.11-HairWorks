// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ObjectKey.h"
#include "SceneOutlinerFwd.h"

#include "SceneOutlinerDragDrop.h"
#include "SceneOutlinerFilters.h"

namespace SceneOutliner
{
	#if PLATFORM_COMPILER_HAS_VARIADIC_TEMPLATES

		template <typename...> struct TMaxSizeof;
		template <> struct TMaxSizeof<> { static const uint32 Value = 0; };
		template <typename T, typename... TRest> struct TMaxSizeof<T, TRest...> { static const uint32 Value = sizeof(T) > TMaxSizeof<TRest...>::Value ? sizeof(T) : TMaxSizeof<TRest...>::Value; };

	#else

		template <typename T1, typename T2=void, typename T3=void, typename T4=void, typename T5=void, typename T6=void>
		struct TMaxSizeof
		{
			static const uint32 Value = sizeof(T1) > TMaxSizeof<T2, T3, T4, T5, T6>::Value ? sizeof(T1) : TMaxSizeof<T2, T3, T4, T5, T6>::Value;
		};
		template <> struct TMaxSizeof<void,void,void,void,void,void> { static const uint32 Value = 0; };
		
	#endif

	/** Variant type that defines an identifier for a tree item. Assumes 'trivial relocatability' as with many unreal containers. */
	struct FTreeItemID
	{
	public:
		/** Default constructed null item ID */
		FTreeItemID() : Type(EType::Null), CachedHash(0) {}

		/** ID representing a UObject */
		FTreeItemID(const UObject* InObject) : Type(EType::Object)
		{
			check(InObject);
			new (Data) FObjectKey(InObject);
			CachedHash = CalculateTypeHash();
		}
		FTreeItemID(const FObjectKey& InKey) : Type(EType::Object)
		{
			new (Data) FObjectKey(InKey);
			CachedHash = CalculateTypeHash();
		}

		/** ID representing a folder */
		FTreeItemID(const FName& InFolder) : Type(EType::Folder)
		{
			new (Data) FName(InFolder);
			CachedHash = CalculateTypeHash();
		}

		/** Copy construction / assignment */
		FTreeItemID(const FTreeItemID& Other)
		{
			*this = Other;
		}
		FTreeItemID& operator=(const FTreeItemID& Other)
		{
			Type = Other.Type;
			switch(Type)
			{
				case EType::Object:			new (Data) FObjectKey(Other.GetAsObjectKey());											break;
				case EType::Folder:			new (Data) FName(Other.GetAsFolderRef());												break;
				default:																											break;
			}

			CachedHash = CalculateTypeHash();
			return *this;
		}

		/** Move construction / assignment */
		FTreeItemID(FTreeItemID&& Other)
		{
			*this = MoveTemp(Other);
		}
		FTreeItemID& operator=(FTreeItemID&& Other)
		{
			FMemory::Memswap(this, &Other, sizeof(FTreeItemID));
			return *this;
		}

		~FTreeItemID()
		{
			switch(Type)
			{
				case EType::Object:			GetAsObjectKey().~FObjectKey();							break;
				case EType::Folder:			GetAsFolderRef().~FName();								break;
				default:																			break;
			}
		}

		friend bool operator==(const FTreeItemID& One, const FTreeItemID& Other)
		{
			return One.Type == Other.Type && One.CachedHash == Other.CachedHash && One.Compare(Other);
		}
		friend bool operator!=(const FTreeItemID& One, const FTreeItemID& Other)
		{
			return One.Type != Other.Type || One.CachedHash != Other.CachedHash || !One.Compare(Other);
		}

		uint32 CalculateTypeHash() const
		{
			uint32 Hash = 0;
			switch(Type)
			{
				case EType::Object:			Hash = GetTypeHash(GetAsObjectKey());				break;
				case EType::Folder:			Hash = GetTypeHash(GetAsFolderRef());				break;
				default:																		break;
			}

			return HashCombine((uint8)Type, Hash);
		}

		friend uint32 GetTypeHash(const FTreeItemID& ItemID)
		{
			return ItemID.CachedHash;
		}

	private:

		FObjectKey& 				GetAsObjectKey() const 			{ return *reinterpret_cast<FObjectKey*>(Data); }
		FName& 						GetAsFolderRef() const			{ return *reinterpret_cast<FName*>(Data); }

		/** Compares the specified ID with this one - assumes matching types */
		bool Compare(const FTreeItemID& Other) const
		{
			switch(Type)
			{
				case EType::Object:			return GetAsObjectKey() == Other.GetAsObjectKey();
				case EType::Folder:			return GetAsFolderRef() == Other.GetAsFolderRef();
				case EType::Null:			return true;
				default: check(false);		return false;
			}
		}

		enum class EType : uint8 { Object, Folder, Null };
		EType Type;

		uint32 CachedHash;
		static const uint32 MaxSize = TMaxSizeof<FObjectKey, FName>::Value;
		mutable uint8 Data[MaxSize];
	};

	struct ETreeItemSortOrder
	{
		enum Type { World = 0, Folder = 10, Actor = 20 };
	};

	/** Folder-specific helper functions */

	/** Parse a new path (including leaf-name) into this tree item. Does not do any notification */
	FName GetFolderLeafName(FName InPath);

	/** Get the parent path for the specified folder path */
	FORCEINLINE FName GetParentPath(FName Path)
	{
		return FName(*FPaths::GetPath(Path.ToString()));
	}

	/** Move the specified folder path to the specified new parent */
	FName MoveFolderTo(FName Path, FName NewParent, UWorld& World);

}	// namespace SceneOutliner