#include "../Public/FirebaseAnalyticsProvider.h"

#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#include "Runtime/ApplicationCore/Public/Android/AndroidApplication.h"
#endif

FFirebaseAnalyticsProvider::FFirebaseAnalyticsProvider()
{

}

bool FFirebaseAnalyticsProvider::StartSession(const TArray<FAnalyticsEventAttribute>& Attributes)
{
	::firebase::App* firebaseApp = nullptr;

#if PLATFORM_ANDROID
	firebaseApp = ::firebase::App::Create(::firebase::AppOptions(), FAndroidApplication::GetJavaEnv(), FJavaWrapper::GameActivityThis);
#elif PLATFORM_IOS
	firebaseApp = ::firebase::App::Create(::firebase::AppOptions());
#else

	// Firebase support only Android & iOS platform,
	// but in need make debug easier we creating fake options so firebase can create an app
	::firebase::AppOptions Options;
	Options.set_api_key("Empty");
	Options.set_app_id("Empty");
	Options.set_database_url("Empty");
	Options.set_ga_tracking_id("Empty");
	Options.set_messaging_sender_id("Empty");
	Options.set_project_id("Empty");
	Options.set_storage_bucket("Empty");

	firebaseApp = ::firebase::App::Create(Options);
#endif

	if (firebaseApp)
	{
		::firebase::analytics::Initialize(*firebaseApp);
		::firebase::analytics::SetAnalyticsCollectionEnabled(true);
		::firebase::analytics::SetSessionTimeoutDuration(1000 * 60 * 30);
	}

	return firebaseApp != nullptr;
}

void FFirebaseAnalyticsProvider::RecordEvent(const FString& EventName, const TArray<FAnalyticsEventAttribute>& Attributes) 
{
	if (::firebase::App::GetInstance())
	{
		::firebase::analytics::LogEvent(TCHAR_TO_UTF8(*EventName));
	}
}

FString FFirebaseAnalyticsProvider::GetSessionID() const
{
	// Do nothing
	return "Unsupported";
}

bool FFirebaseAnalyticsProvider::SetSessionID(const FString & InSessionID)
{
	// Do nothing
	return false;
}

void FFirebaseAnalyticsProvider::FlushEvents()
{
	// Do nothing
	return;
}

void FFirebaseAnalyticsProvider::SetUserID(const FString & InUserID)
{
	if (::firebase::App::GetInstance())
	{
		::firebase::analytics::SetUserId(TCHAR_TO_UTF8(*InUserID));
	}
}

FString FFirebaseAnalyticsProvider::GetUserID() const
{
	// Do nothing
	return "Unsupported";
}

void FFirebaseAnalyticsProvider::EndSession()
{
	if (::firebase::App::GetInstance())
	{
		::firebase::analytics::Terminate();
		delete ::firebase::App::GetInstance();
	}
}

FFirebaseAnalyticsProvider::~FFirebaseAnalyticsProvider()
{

}
