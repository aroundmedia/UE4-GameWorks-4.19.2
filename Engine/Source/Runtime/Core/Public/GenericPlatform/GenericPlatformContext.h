// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

class FString;

/** Enumerates crash description versions. */
enum class ECrashDescVersions
{
	/** Introduces a new crash description format. */
	VER_1_NewCrashFormat,

	/** Added misc properties (CPU,GPU,OS,etc), memory related stats and platform specific properties as generic payload. */
	VER_2_AddedNewProperties,
};

/**
 *	Contains a runtime crash's properties that are common for all platforms.
 *	This may change in the future.
 */
struct CORE_API FGenericCrashContext
{
public:
	static const ANSICHAR* CrashContextRuntimeXMLNameA;
	static const TCHAR* CrashContextRuntimeXMLNameW;

	/**	Whether the Initialize() has been called */
	static bool bIsInitialized;

	/** Initializes crash context related platform specific data that can be impossible to obtain after a crash. */
	static void Initialize();

	/**
	 * @return true, if the generic crash context has been initialized.
	 */
	static bool IsInitalized()
	{
		return bIsInitialized;
	}

	/** Default constructor. */
	FGenericCrashContext();

	/** Serializes all data to the buffer. */
	void SerializeContentToBuffer();

	const FString& GetBuffer() const
	{
		return CommonBuffer;
	}

	/** Serializes crash's informations to the specified filename. */
	void SerializeAsXML( const TCHAR* Filename );

	/** Writes a common property to the buffer. */
	void AddCrashProperty( const TCHAR* PropertyName, const TCHAR* PropertyValue );

	/** Writes a common property to the buffer. */
	template <typename Type>
	void AddCrashProperty( const TCHAR* PropertyName, const Type& Value )
	{
		AddCrashProperty( PropertyName, *TTypeToString<Type>::ToString( Value ) );
	}

private:
	/** Serializes platform specific properties to the buffer. */
	virtual void AddPlatformSpecificProperties();

	/** Writes header information to the buffer. */
	void AddHeader();

	/** Writes footer to the buffer. */
	void AddFooter();

	/** The buffer used to store the crash's properties. */
	FString& CommonBuffer;

	// FNoncopyable
	FGenericCrashContext( const FGenericCrashContext& );
	FGenericCrashContext& operator=(const FGenericCrashContext&);
};

struct CORE_API FGenericMemoryWarningContext
{};
