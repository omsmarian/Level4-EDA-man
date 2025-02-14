/**
 * EDA-Man
 *
 * @file MQTTClient.h
 *
 * @copyright Copyright (C) 2022
 *
 * @authors Tiago Nanni, Mariano Oms , Tomas Whickham and Cristian Meichtry
 *
 */

#ifndef _MQTTCLIENT_H
#define _MQTTCLIENT_H

#include <string>
#include <vector>

#include <mosquitto.h>

 /**
  * @brief
  *
  */
struct MQTTMessage
{
	std::string topic;
	std::vector<char> payload;
};

/**
 * Connects to an MQTT client
 */
class MQTTClient
{
public:
	MQTTClient(std::string clientId);
	~MQTTClient();

	bool connect(std::string host, int port, std::string username, std::string password);
	bool isConnected();
	void disconnect();

	bool publish(std::string topic, std::vector<char>& payload);

	bool subscribe(std::string topic);
	bool unsubscribe(std::string topic);

	std::vector<MQTTMessage> getMessages();

private:
	struct mosquitto* mosquittoInstance;
	bool connected;

	std::vector<MQTTMessage> lastMessages;

	friend void onMQTTMessage(struct mosquitto* mosquittoClient,
		void* context,
		const struct mosquitto_message* message);
};

#endif
