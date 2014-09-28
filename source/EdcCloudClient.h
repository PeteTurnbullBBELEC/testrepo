#ifndef _EDCCLOUDCLIENT_H
#define _EDCCLOUDCLIENT_H

#include "EdcCloudHelpers.h"
#include "edcpayload.pb-c.h"
#include "MQTTClient.h"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef _countof
#define _countof(a) (sizeof((a))/sizeof((a)[0]))
#endif /* _countof */

#define EDCCLIENT_SUCCESS MQTTCLIENT_SUCCESS

void EdcCloudClientSleep (unsigned long milliseconds) {
	usleep(milliseconds*1000);
}

#define EDCCLIENT_PUBLISH_TIMEOUT	50000L
#define EDCCLIENT_QOS				1
#define EDCCLIENT_RETAIN			0
#define EDCCLIENT_RECONNECT_TIMEOUT	1000

const char *EdcBirthPayloadNames[] = {
	"uptime",
	"display_name",
	"model_name",
	"model_id",
	"part_number",
	"serial_number",
	"firmware_version",
	"bios_version",
	"os_version",
	"jvm_name",
	"jvm_version",
	"jvm_profile",
	"connection_interface",
	"connection_ip",
};

const char *EdcBirthPayloadValues[] = {
	"123",
	"C client",
	"test model_name",
	"test model_id",
	"test part_number",
	"test serial_number",
	"test firmware_version",
	"test bios_version",
	"test_os_version",
	"test jvm_name",
	"test jvm_version",
	"test jvm_profile",
	"test connection_interface",
	"test connection_ip",
};


typedef struct
{
	MQTTClient 			m_MQTTClient;
	EdcConfiguration  * m_edcConfiguration;
	EdcDeviceProfile  * m_edcDeviceProfile;
	unsigned char 		m_Connected;

} EdcCloudClient;


/* Internal publishing mechanism */
/* USES MALLOC */
int _publish(EdcCloudClient *obj,
				char *topic, EdcPayload *payload, int qos,
				unsigned char retain, unsigned long timeout) {
	int rc;
	MQTTClient_deliveryToken deliveryToken;
	MQTTClient_message pubmsg = { "", 0, 0, NULL, 0, 0, 0, 0 };

	memcpy(&pubmsg.struct_id, "MQTM", strlen("MQTM"));

	unsigned size = edc_payload__get_packed_size(payload);
	void *data = malloc(sizeof(char) * size);

	//TODO
	edc_payload__pack(payload, data);
	//

	pubmsg.payload = data;
	pubmsg.payloadlen = size;

	pubmsg.qos = qos;
	pubmsg.retained = retain;

	char *ctopic = malloc(sizeof(char) * (strlen(topic)+1));
	strcpy(ctopic, topic);

	rc = MQTTClient_publishMessage(obj->m_MQTTClient, ctopic, &pubmsg, &deliveryToken);

	//free (data);
	//free (ctopic);

	return rc;
}


/* USES MALLOC */
char *_buildTopic(EdcCloudClient *obj, char *semanticTopic) {
	char *accName = (obj->m_edcConfiguration)->m_accountName;
	char *dassetId = (obj->m_edcConfiguration)->m_assetId;

	int size = strlen(accName) + strlen(dassetId) + strlen(semanticTopic) + 4;
	char *topic = malloc(sizeof(char) * size);
	sprintf(topic, "%s/%s/%s", accName, dassetId, semanticTopic);
	return topic;
}

/* USES MALLOC */
char *_buildBirthCertificateTopic(EdcCloudClient *obj) {
	char *accName = (obj->m_edcConfiguration)->m_accountName;
	char *dassetId = (obj->m_edcConfiguration)->m_assetId;

	char *h1 = "$EDC/";
	char *h2 = "/MQTT/BIRTH";

	int size = strlen(accName) + strlen(dassetId) + strlen(h1) + strlen(h2) + 3;
	char *topic = malloc(sizeof(char) * size);
	sprintf(topic, "%s%s/%s%s", h1, accName, dassetId, h2);
	return topic;
}

/* USES MALLOC */
char *_buildDisconnectCertificateTopic(EdcCloudClient *obj) {
	char *accName = (obj->m_edcConfiguration)->m_accountName;
	char *dassetId = (obj->m_edcConfiguration)->m_assetId;

	char *h1 = "$EDC/";
	char *h2 = "/MQTT/DC";

	int size = strlen(accName) + strlen(dassetId) + strlen(h1) + strlen(h2) + 3;
	char *topic = malloc(sizeof(char) * size);
	sprintf(topic, "%s%s/%s%s", h1, accName, dassetId, h2);
	return topic;
}



/* USES MALLOC */
EdcPayload *_buildBirthCertificatePayload(EdcCloudClient *obj) {

	EdcPayload edcPayload = EDC_PAYLOAD__INIT;
	
	int noMetrics = _countof(EdcBirthPayloadNames);
	edcPayload.n_metric = noMetrics;
	edcPayload.metric = malloc(sizeof(EdcPayload__EdcMetric *) * noMetrics);

	int i;
	for (i = 0; i < noMetrics; i++) {
		EdcPayload__EdcMetric edcMetric = EDC_PAYLOAD__EDC_METRIC__INIT;
		edcMetric.name = EdcBirthPayloadNames[i];
		edcMetric.type = 5;
		edcMetric.string_value = EdcBirthPayloadValues[i];

		EdcPayload__EdcMetric *edcMetricP = malloc(sizeof(EdcPayload__EdcMetric));
		*edcMetricP = edcMetric;
		edcPayload.metric[i] = edcMetricP;
	}

	EdcPayload__EdcPosition edcPosition = EDC_PAYLOAD__EDC_POSITION__INIT;	
	edcPosition.latitude = (obj->m_edcDeviceProfile)->latitude;
	edcPosition.longitude = (obj->m_edcDeviceProfile)->longitude;
	edcPosition.altitude = (obj->m_edcDeviceProfile)->altitude;

	EdcPayload__EdcPosition *edcPositionP = malloc(sizeof(EdcPayload__EdcPosition));
	*edcPositionP = edcPosition;
	edcPayload.position = edcPositionP;

	EdcPayload *edcPayloadP = malloc(sizeof(EdcPayload));
	*edcPayloadP = edcPayload;

	return edcPayloadP;
}


EdcPayload *_buildDisconnectCertificatePayload(EdcCloudClient *obj) {
	EdcPayload edcPayload = EDC_PAYLOAD__INIT;
	
	int noMetrics = 2;
	edcPayload.n_metric = noMetrics;
	edcPayload.metric = malloc(sizeof(EdcPayload__EdcMetric *) * noMetrics);

	int i;
	for (i = 0; i < noMetrics; i++) {
		EdcPayload__EdcMetric edcMetric = EDC_PAYLOAD__EDC_METRIC__INIT;
		edcMetric.name = EdcBirthPayloadNames[i];
		edcMetric.type = 5;
		edcMetric.string_value = EdcBirthPayloadValues[i];

		EdcPayload__EdcMetric *edcMetricP = malloc(sizeof(EdcPayload__EdcMetric));
		*edcMetricP = edcMetric;
		edcPayload.metric[i] = edcMetricP;
	}

	EdcPayload *edcPayloadP = malloc(sizeof(EdcPayload));
	*edcPayloadP = edcPayload;

	return edcPayloadP;
}

int _publishDisconnectCertificate(EdcCloudClient *obj) {
	EdcPayload *payload = _buildDisconnectCertificatePayload(obj);
	char *topic = _buildDisconnectCertificateTopic(obj);

	int ret = _publish(obj, topic, payload, EDCCLIENT_QOS, 
					EDCCLIENT_RETAIN, EDCCLIENT_PUBLISH_TIMEOUT);
	//TODO
	//delete payload;
	//
	return ret;
}

int _publishBirthCertificate(EdcCloudClient *obj) {	
	EdcPayload *payload = _buildBirthCertificatePayload(obj);
	char *topic = _buildBirthCertificateTopic(obj);

	int ret = _publish(obj, topic, payload, EDCCLIENT_QOS, 
					EDCCLIENT_RETAIN, EDCCLIENT_PUBLISH_TIMEOUT);
	//TODO
	//delete payload;
	//
	return ret;
}


//***********************PUBLIC METHODS**************************//
//***************************************************************//

void init_stub__EdcCloudClient(EdcCloudClient *obj) {
	obj->m_Connected = 0;
}

void init__EdcCloudClient(EdcCloudClient *obj, EdcConfiguration
		*edcConfiguration, EdcDeviceProfile *edcDeviceProfile) {	
	obj->m_edcConfiguration = edcConfiguration;
	obj->m_edcDeviceProfile = edcDeviceProfile;
	obj->m_Connected = 0;
}


/* USES MALLOC */
int startSession(EdcCloudClient *obj) {
	char *brokerUrlo = (obj->m_edcConfiguration)->m_brokerUrl;
	char *brokerUrl = malloc(sizeof(char) * (strlen(brokerUrlo)+1));
	strcpy(brokerUrl, brokerUrlo);
	
	char *clientIdo = (obj->m_edcConfiguration)->m_clientId;
	char *clientId = malloc(sizeof(char) * (strlen(clientIdo)+1));
	strcpy(clientId, clientIdo);

	//setup MQTT connection parameters
	int rc = MQTTClient_create(&(obj->m_MQTTClient), brokerUrl, clientId, 
			MQTTCLIENT_PERSISTENCE_NONE, NULL);

	if (rc != MQTTCLIENT_SUCCESS) {
		free(brokerUrl);
		free(clientId);
		return rc;
	}


	//connect to broker
	MQTTClient_connectOptions conn_opts = { "", 0, 60, 1, 1, NULL, NULL, NULL, 30, 20 };

	memcpy(&conn_opts.struct_id, "MQTC", strlen("MQTC"));
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;

	char *usernameo = (obj->m_edcConfiguration)->m_username;
	conn_opts.username = malloc(sizeof(char) * (strlen(usernameo)+1));
	strcpy(conn_opts.username, usernameo);

	char *passwordo = (obj->m_edcConfiguration)->m_password;
	conn_opts.password = malloc(sizeof(char) * (strlen(passwordo)+1));
	strcpy(conn_opts.password, passwordo);

	rc =  MQTTClient_connect(obj->m_MQTTClient, &conn_opts);

	if (rc != MQTTCLIENT_SUCCESS) {
		free(brokerUrl);
		free(clientId);
		free(conn_opts.username);
		free(conn_opts.password);
		return rc;
	}

	obj->m_Connected = 1;

	//publish birth certificate
	rc = _publishBirthCertificate(obj);
	
	free(brokerUrl);
	free(clientId);
	free(conn_opts.username);
	free(conn_opts.password);

	return rc;
}


int stopSession(EdcCloudClient *obj) {
	int	rc = _publishDisconnectCertificate(obj);
	rc = MQTTClient_disconnect(obj->m_MQTTClient, EDCCLIENT_PUBLISH_TIMEOUT);

	return rc;
}


/* USES FREE */
int publish(EdcCloudClient *obj, char *topic, EdcPayload *payload, int qos,
		unsigned char retain, unsigned long timeout) {
	char *fullTopic = _buildTopic(obj, topic);
	int ret = _publish(obj, fullTopic, payload, qos, retain, timeout);
	//free (fullTopic);
	
	return ret;
}


void terminate(EdcCloudClient *obj) {
	MQTTClient_destroy(&(obj->m_MQTTClient));
}
#endif
