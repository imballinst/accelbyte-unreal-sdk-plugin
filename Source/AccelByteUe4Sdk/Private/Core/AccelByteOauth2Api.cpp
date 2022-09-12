// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteOauth2Api.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "JsonUtilities.h"
#include "Core/AccelByteUtilities.h"
#include "Misc/Base64.h"

namespace AccelByte
{
namespace Api
{
void Oauth2::GetTokenWithAuthorizationCode(const FString& ClientId, const FString& ClientSecret, const FString& AuthorizationCode, const FString& RedirectUri, const THandler<FOauth2Token>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(
		FString(__FUNCTION__),
		TEXT("will be deprecated soon, please use GetTokenWithAuthorizationCodeV3 instead!!"));
	
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/oauth/token"), *FRegistry::Settings.IamServerUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(FString::Printf(TEXT("grant_type=authorization_code&code=%s&redirect_uri=%s"), *AuthorizationCode, *RedirectUri));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
	
void Oauth2::GetTokenWithPasswordCredentials(const FString& ClientId, const FString& ClientSecret, const FString& Username, const FString& Password, const THandler<FOauth2Token>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(
		FString(__FUNCTION__),
		TEXT("will be deprecated soon, please use GetTokenWithPasswordCredentialsV3 instead!!"));
	
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/oauth/token"), *FRegistry::Settings.IamServerUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	const TCHAR Format[] = TEXT("grant_type=password&username=%s&password=%s&device_id=%s");
	const FString EncodedUsername = FGenericPlatformHttp::UrlEncode(Username);
	const FString EncodedPassword =  FGenericPlatformHttp::UrlEncode(Password);
	const FString EncodedDeviceId = FGenericPlatformHttp::UrlEncode(FAccelByteUtilities::GetDeviceId());
	Request->SetContentAsString(FString::Printf(Format, *EncodedUsername, *EncodedPassword, *EncodedDeviceId));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::GetTokenWithPasswordCredentials(const FString& ClientId, const FString& ClientSecret, const FString& Username, const FString& Password, const THandler<FOauth2Token>& OnSuccess, const FCustomErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(
		FString(__FUNCTION__),
		TEXT("will be deprecated soon, please use GetTokenWithPasswordCredentialsV3 instead!!"));
	
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/oauth/token"), *FRegistry::Settings.IamServerUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetHeader(TEXT("cookie"), TEXT("device_token=" + FGenericPlatformHttp::UrlEncode(FAccelByteUtilities::GetDeviceId())));
	
	const TCHAR Format[] = TEXT("grant_type=password&username=%s&password=%s&device_id=%s");
	const FString EncodedUsername = FGenericPlatformHttp::UrlEncode(Username);
	const FString EncodedPassword =  FGenericPlatformHttp::UrlEncode(Password);
	const FString EncodedDeviceId = FGenericPlatformHttp::UrlEncode(FAccelByteUtilities::GetDeviceId());
	Request->SetContentAsString(FString::Printf(Format, *EncodedUsername, *EncodedPassword, *EncodedDeviceId));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::GetTokenWithClientCredentials(const FString& ClientId, const FString& ClientSecret, const THandler<FOauth2Token>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/token"), *FRegistry::Settings.IamServerUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(FString::Printf(TEXT("grant_type=client_credentials")));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

//
// Custom grant types 
//

void Oauth2::GetTokenWithDeviceId(const FString& ClientId, const FString& ClientSecret, const THandler<FOauth2Token>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/platforms/device/token"), *FRegistry::Settings.IamServerUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));

	Request->SetContentAsString(FString::Printf(TEXT("device_id=%s"), *FGenericPlatformHttp::UrlEncode(*FAccelByteUtilities::GetDeviceId())));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::GetTokenWithOtherPlatformToken(const FString& ClientId, const FString& ClientSecret, const FString& PlatformId, const FString& PlatformToken, const THandler<FOauth2Token>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(
		FString(__FUNCTION__),
		TEXT("When 2FA enabled This method should change to Oauth2::GetTokenWithOtherPlatformToken(const FString& ClientId, const FString& ClientSecret, const FString& PlatformId, const FString& PlatformToken, const THandler<FOauth2Token>& OnSuccess, const FCustomErrorHandler& OnError)"));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/platforms/%s/token"), *FRegistry::Settings.IamServerUrl, *PlatformId));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(FString::Printf(TEXT("platform_token=%s"), *FGenericPlatformHttp::UrlEncode(PlatformToken)));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
	
void Oauth2::GetTokenWithOtherPlatformToken(const FString& ClientId, const FString& ClientSecret, const FString& PlatformId, const FString& PlatformToken,
	const THandler<FOauth2Token>& OnSuccess, const FCustomErrorHandler& OnError, bool bCreateHeadless )
{
	FReport::Log(FString(__FUNCTION__));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/platforms/%s/token"), *FRegistry::Settings.IamServerUrl, *PlatformId));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetHeader(TEXT("cookie"), TEXT("device-token=" + FGenericPlatformHttp::UrlEncode(FAccelByteUtilities::GetDeviceId())));
	Request->SetContentAsString(FString::Printf(TEXT("platform_token=%s&createHeadless=%s"), *FGenericPlatformHttp::UrlEncode(PlatformToken),
		bCreateHeadless ? TEXT("true") : TEXT("false")));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::GetTokenWithRefreshToken(const FString& ClientId, const FString& ClientSecret, const FString& RefreshId, const THandler<FOauth2Token>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/token"), *FRegistry::Settings.IamServerUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(FString::Printf(TEXT("grant_type=refresh_token&refresh_token=%s"), *RefreshId));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::RevokeToken(const FString& AccessToken, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(
		FString(__FUNCTION__),
		TEXT("This method uses wrong auth type and will be deprecated. Use RevokeUserToken instead!"));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/revoke"), *FRegistry::Settings.IamServerUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Bearer " + AccessToken));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(FString::Printf(TEXT("token=%s"), *FGenericPlatformHttp::UrlEncode(*AccessToken)));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(FVoidHandler::CreateLambda([OnSuccess]() { 
		OnSuccess.ExecuteIfBound();
	}), OnError), FPlatformTime::Seconds());
}

void Oauth2::RevokeUserToken(const FString& ClientId, const FString& ClientSecret, const FString& AccessToken, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/revoke"), *FRegistry::Settings.IamServerUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(FString::Printf(TEXT("token=%s"), *FGenericPlatformHttp::UrlEncode(*AccessToken)));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(FVoidHandler::CreateLambda([OnSuccess]() { 
	OnSuccess.ExecuteIfBound();
	}), OnError), FPlatformTime::Seconds());
}

void Oauth2::GetTokenWithAuthorizationCodeV3(const FString& ClientId, const FString& ClientSecret, const FString& AuthorizationCode, const FString& RedirectUri, const THandler<FOauth2Token>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/token"), *FRegistry::Settings.IamServerUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(FString::Printf(TEXT("grant_type=authorization_code&code=%s&redirect_uri=%s"), *AuthorizationCode, *RedirectUri));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::GetTokenWithPasswordCredentialsV3(const FString& ClientId, const FString& ClientSecret, const FString& Username, const FString& Password, const THandler<FOauth2Token>& OnSuccess, const FErrorHandler& OnError, bool bRememberMe)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(
		FString(__FUNCTION__),
		TEXT("When 2FA enabled This method should change to Oauth2::GetTokenWithPasswordCredentialsV3(const FString& ClientId, const FString& ClientSecret, const FString& Username, const FString& Password, const THandler<FOauth2Token>& OnSuccess, const FCustomErrorHandler& OnError, bool RememberMe)"));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/token"), *FRegistry::Settings.IamServerUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));

	const TCHAR Format[] = TEXT("grant_type=password&username=%s&password=%s&namespace=%s&extend_exp=%s");
	const FString EncodedUsername = FGenericPlatformHttp::UrlEncode(Username);
	const FString EncodedPassword =  FGenericPlatformHttp::UrlEncode(Password);
	const FString Namespace =  FGenericPlatformHttp::UrlEncode(*FRegistry::Settings.Namespace);
	const FString Extend_exp = bRememberMe ? FString::Printf(TEXT("true")) : FString::Printf(TEXT("false"));
	
	Request->SetContentAsString(FString::Printf(Format, *EncodedUsername, *EncodedPassword, *Namespace, *Extend_exp));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::GetTokenWithPasswordCredentialsV3(const FString& ClientId, const FString& ClientSecret, const FString& Username, const FString& Password, const THandler<FOauth2Token>& OnSuccess, const FCustomErrorHandler& OnError, bool bRememberMe)
{
	FReport::Log(FString(__FUNCTION__));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/token"), *FRegistry::Settings.IamServerUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetHeader(TEXT("cookie"), TEXT("device-token=" + FGenericPlatformHttp::UrlEncode(FAccelByteUtilities::GetDeviceId())));

	const TCHAR Format[] = TEXT("grant_type=password&username=%s&password=%s&namespace=%s&extend_exp=%s");
	const FString EncodedUsername = FGenericPlatformHttp::UrlEncode(Username);
	const FString EncodedPassword =  FGenericPlatformHttp::UrlEncode(Password);
	const FString Namespace =  FGenericPlatformHttp::UrlEncode(*FRegistry::Settings.Namespace);
	const FString Extend_exp = bRememberMe ? FString::Printf(TEXT("true")) : FString::Printf(TEXT("false"));
	
	Request->SetContentAsString(FString::Printf(Format, *EncodedUsername, *EncodedPassword, *Namespace, *Extend_exp));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::VerifyAndRememberNewDevice(const FString& ClientId, const FString& ClientSecret, const FString& MfaToken, EAccelByteLoginAuthFactorType AuthFactorType, const FString& Code, 
	const THandler<FOauth2Token>& OnSuccess, const FCustomErrorHandler& OnError, bool bRememberDevice)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Factor = FAccelByteUtilities::GetAuthenticatorString(AuthFactorType);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/mfa/verify"), *FRegistry::Settings.IamServerUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetHeader(TEXT("cookie"), TEXT("device-token=" + FGenericPlatformHttp::UrlEncode(FAccelByteUtilities::GetDeviceId())));
	const FString RememberThisDevice = bRememberDevice ? FString::Printf(TEXT("true")) : FString::Printf(TEXT("false"));
	Request->SetContentAsString(FString::Printf(TEXT("mfaToken=%s&factor=%s&code=%s&rememberDevice=%s"), *MfaToken, *Factor, *Code, *RememberThisDevice));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::CreateHeadlessAccountAndResponseToken(const FString& ClientId, const FString& ClientSecret, const FString& LinkingToken, const THandler<FOauth2Token>& OnSuccess, const FCustomErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST")); 
	Request->SetURL(FString::Printf(TEXT("%s/v3/headless/token"), *FRegistry::Settings.IamServerUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(FString::Printf(TEXT("linkingToken=%s&client_id=%s"), *LinkingToken, *ClientId));
	
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds()); 
}

void Oauth2::AuthenticationWithPlatformLink(const FString& ClientId, const FString& ClientSecret, const FString& Username, const FString& Password, const FString& LinkingToken, const THandler<FOauth2Token>& OnSuccess, const FCustomErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/authenticateWithLink"), *FRegistry::Settings.IamServerUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(FString::Printf(TEXT("username=%s&password=%s&linkingToken=%s&client_id=%s"),
		*FGenericPlatformHttp::UrlEncode(Username),
		*FGenericPlatformHttp::UrlEncode(Password),
		*LinkingToken, *ClientId));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
	
void Oauth2::VerifyToken(const FString& ClientId, const FString& ClientSecret, const FString& Token, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/verify"), *FRegistry::Settings.IamServerUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetHeader(TEXT("cookie"), TEXT("device-token=" + FGenericPlatformHttp::UrlEncode(FAccelByteUtilities::GetDeviceId())));
	Request->SetContentAsString(FString::Printf(TEXT("token=%s"), *Token));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request,
		CreateHttpResultHandler(
			THandler<FJsonObject>::CreateLambda([OnSuccess, Token](FJsonObject const& JsonObject)
			{
				FString CheckedToken;
				JsonObject.TryGetStringField("access_token", CheckedToken);
				if (Token.Equals(CheckedToken))
				{
					OnSuccess.ExecuteIfBound();
				}
			}),
			OnError),
		FPlatformTime::Seconds());
}

} // Namespace Api
} // Namespace AccelByte
