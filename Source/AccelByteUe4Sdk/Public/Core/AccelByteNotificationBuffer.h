﻿// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Models/AccelByteLobbyModels.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteNotificationBuffer, Log, All);
namespace AccelByte
{
	class ACCELBYTEUE4SDK_API FAccelByteNotificationBuffer
	{
	public:
		/**
		 * @brief Try buffer incoming notification if needed.
		 *	Internally it will determine if incoming notification need to be buffered due to missing notification.
		 *
		 * @param InNotification Incoming notification.
		 * @return true if missing notification detected and the incoming notification buffered.
		 *	false means no missing notification detected, incoming notification not buffered and internal lastSequenceId & lastSequenceNumber updated
		 */
		bool TryAddBuffer(const FAccelByteModelsUserNotification& InNotification);

		/**
		 * @brief Add missing notification (only can be called when buffering).
		 *
		 * @param Notifications The missing notifications
		 * @return true if missing notification added to buffer.
		 */
		bool AddMissingNotifications(const TArray<FAccelByteModelsUserNotification>& Notifications);

		/**
		 * @brief Get the time when last valid notification received.
		 *
		 * @return The FDateTime object representing the time last notification received.
		 */
		FDateTime GetLastNotificationReceivedTime();

		/**
		 * @brief Get the sequence ID of the last valid notification.
		 *
		 * @return String representing last valid sequence ID.
		 */
		FString GetLastNotificationSequenceID();

		/**
		 * @brief Get the sequence number of the last valid notification.
		 *
		 * @return integer representing last valid sequence number.
		 */
		int32 GetLastNotificationSequenceNumber();

		/**
		 * @brief Clear all buffered notifications.
		 */
		void Clear();

		/**
		 * @brief Get all sorted buffer based on it's SentAt field.
		 *
		 * @return Array of sorted notifications buffer
		 */
		TArray<FAccelByteModelsUserNotification> GetSortedBuffer();

		/**
		 * @brief Check if this buffer is in process of retrieving missing notification.
		 *
		 * @return true if buffering.
		 */
		bool IsBuffering();

	private:
		bool bBuffering{false};

		FString LastSequenceID{};
		int32 LastSequenceNumber{0};
		FDateTime LastSentAt{0};

		mutable FCriticalSection BufferLock;
		TArray<FAccelByteModelsUserNotification> Buffer;

		void UpdateLastSequence(const FAccelByteModelsUserNotification& InNotification);
		bool HasValidSequence(const FAccelByteModelsUserNotification& InNotification);
		bool IsNotificationMissing(const FAccelByteModelsUserNotification& InNotification);
		bool IsDuplicateNotification(const FAccelByteModelsUserNotification& InNotification);
		bool IsExistInBuffer(const FString& InSequenceID, const int32& InSequenceNumber);
		void AddToBuffer(const FAccelByteModelsUserNotification& InNotification);
		void SortBuffer();
	};
}
