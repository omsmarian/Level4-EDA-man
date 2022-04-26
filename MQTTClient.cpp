/**
 * EDA-Man
 *
 * @file MQTTClient.cpp
 *
 * @copyright Copyright (C) 2022
 *
 * @authors Tiago Nanni, Mariano Oms , Tomas Whickham and Cristian Meichtry
 *
 */


#include <cstring>
#include <iostream>

static void onMQTTMessage(struct mosquitto* mosquittoClient,
	void* context,
	const struct mosquitto_message* message);

#include "MQTTClient.h"

using namespace std;

// Awful but necessary global variable:
static bool isMosquittoInitialized = false;

/**
 * @brief MQTT message callback.
 *
 */
static void onMQTTMessage(struct mosquitto* mosquittoClient,
	void* context,
	const struct mosquitto_message* message)
{
	MQTTClient* mqttClient = (MQTTClient*)context;

	MQTTMessage mqttMessage;
	mqttMessage.topic = message->topic;
	mqttMessage.payload.resize(message->payloadlen);
	memcpy(mqttMessage.payload.data(), message->payload, message->payloadlen);

	mqttClient->lastMessages.push_back(mqttMessage);
}

/**
 * @brief Construct a new MQTTClient::MQTTClient object
 *
 * @param clientId An MQTT client identifier
 */
MQTTClient::MQTTClient(string clientId)
{
	if (!isMosquittoInitialized)
	{
		mosquitto_lib_init();

		isMosquittoInitialized = true;
	}

	const bool cleanSession = true;

	mosquittoInstance = mosquitto_new(clientId.c_str(), cleanSession, this);

	mosquitto_message_callback_set(mosquittoInstance, onMQTTMessage);

	connected = false;
}

/**
 * @brief Destroy the MQTTClient::MQTTClient object
 *
 */
MQTTClient::~MQTTClient()
{
	mosquitto_destroy(mosquittoInstance);
}

/**
 * @brief Connects to an MQTT server without encryption.
 *
 * @param host Host to connect to (IP address or domain name)
 * @param port TCP port of MQTT server
 * @param username MQTT username
 * @param password MQTT password
 * @return true Call successful
 * @return false Call failed
 */
bool MQTTClient::connect(string host, int port, string username, string password)
{
	int errorCode;

	mosquitto_username_pw_set(mosquittoInstance,
		username.c_str(),
		password.c_str());

	const int keepalive = 60;

	errorCode = mosquitto_connect(mosquittoInstance,
		host.c_str(),
		port,
		keepalive);

	if (errorCode == MOSQ_ERR_SUCCESS)
		connected = true;

	return connected;
}

/**
 * @brief Connection up?
 *
 * @return true Connection is up
 * @return false Connection is down
 */
bool MQTTClient::isConnected()
{
	return connected;
}

/**
 * @brief Disconnects from the MQTT server.
 *
 */
void MQTTClient::disconnect()
{
	mosquitto_disconnect(mosquittoInstance);

	connected = false;
}

/**
 * @brief Publishes an MQTT message on the server.
 *
 * @param topic The MQTT topic
 * @param payload The data to be sent
 * @return true Message sent
 * @return false Message not sent
 */
bool MQTTClient::publish(string topic, vector<char>& payload)
{
	const int qos = 0;
	const bool retain = false;

	int errorCode = mosquitto_publish(mosquittoInstance,
		NULL,
		topic.c_str(),
		(int)payload.size(),
		payload.data(),
		qos,
		retain);

	if (errorCode == MOSQ_ERR_NO_CONN)
		connected = false;

	return (errorCode == MOSQ_ERR_SUCCESS);
}

/**
 * @brief Sends an MQTT subscription request.
 *
 * @param topic The MQTT topic
 * @return true Call successful
 * @return false Call failed
 */
bool MQTTClient::subscribe(string topic)
{
	const int qos = 0;

	int errorCode = mosquitto_subscribe(mosquittoInstance,
		NULL,
		topic.c_str(),
		qos);

	if (errorCode == MOSQ_ERR_NO_CONN)
		connected = false;

	return (errorCode == MOSQ_ERR_SUCCESS);
}

/**
 * @brief Send an MQTT unsubscription request. Should match a previous subscription request.
 *
 * @param topic The MQTT topic
 * @return true Call successfull
 * @return false Call failed
 */
bool MQTTClient::unsubscribe(string topic)
{
	int errorCode = mosquitto_unsubscribe(mosquittoInstance,
		NULL,
		topic.c_str());

	if (errorCode == MOSQ_ERR_NO_CONN)
		connected = false;

	return (errorCode == MOSQ_ERR_SUCCESS);
}

/**
 * @brief Retrieves MQTT messages. Should be called frequently.
 *
 * @return vector<MQTTMessage> The retrieved messages
 */
vector<MQTTMessage> MQTTClient::getMessages()
{
	vector<MQTTMessage> messages;

	while (true)
	{
		const int timeout = 0;
		const int maxPackets = 1;

		int errorCode = mosquitto_loop(mosquittoInstance, timeout, maxPackets);

		if ((errorCode == MOSQ_ERR_NO_CONN) ||
			(errorCode == MOSQ_ERR_CONN_LOST))
			connected = false;

		if (!lastMessages.size())
			break;

		// Moves lastMessages to messages
		messages.insert(messages.end(), lastMessages.begin(), lastMessages.end());
		lastMessages.clear();
	}

	return messages;
}
