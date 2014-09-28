#ifndef _EDCCLOUDHELPERS_H
#define _EDCCLOUDHELPERS_H

#include "edcpayload.pb-c.h"

typedef struct
{
	char *				m_accountName;
	char *				m_assetId;
	char *				m_brokerUrl;
	unsigned char		m_cleanStart;
	char *				m_clientId;
	char *				m_username;
	char *				m_password;
	short				m_keepAlive;
	short				m_reconnectInterval;
	char *				m_willTopicSuffix;
	char *				m_willTopic;
	char *				m_willMessage;
	int					m_willQos;
	unsigned char		m_willRetain;
	char *				m_disconnectTopicSuffix;
	char *				m_disconnectTopic;
	char *				m_disconnectEdcMessage;
	int					m_disconnectQos;
	unsigned char		m_disconnectRetain;
	char *				m_birthTopicSuffix;
	char *				m_birthTopic;
	EdcPayload			m_birthEdcPayload;
	int					m_birthQos;
	unsigned char		m_birthRetain;
} EdcConfiguration;


void init__EdcConfiguration(EdcConfiguration *obj,
								char *accountName,
								char *assetId,
								char *brokerUrl,
								char *clientId,
								char *username,
								char *password,
								short keepAlive,
								short reconnectInterval,
								char *willMessage,
								int willQos,
								unsigned char willRetain) {
	obj->m_accountName = accountName;
	obj->m_assetId = assetId;
	obj->m_brokerUrl = brokerUrl;
	obj->m_clientId = clientId;
	obj->m_username = username;
	obj->m_password = password;
	obj->m_keepAlive = keepAlive;
	obj->m_reconnectInterval = reconnectInterval;
	obj->m_willMessage = willMessage;
	obj->m_willQos = willQos;
	obj->m_willRetain = willRetain;
}


void init_clean__EdcConfiguration(EdcConfiguration *obj,
								char *accountName,
								char *assetId,
								char *brokerUrl,
								char *clientId,
								char *username,
								char *password,
								short keepAlive,
								short reconnectInterval,
								char *willTopicSuffix,
                            	char *willTopic,
								char *willMessage,
								int willQos,
								unsigned char willRetain,
								char *disconnectTopicSuffix,
                            	char *disconnectTopic,
                            	char *disconnectEdcMessage,
                            	int disconnectQos,
                           	 	unsigned char disconnectRetain,
                            	char *birthTopicSuffix,
                            	char *birthTopic,
                            	EdcPayload birthEdcPayload,
                            	int birthQos,
                            	unsigned char birthRetain) {
	obj->m_accountName = accountName;
	obj->m_assetId = assetId;
	obj->m_brokerUrl = brokerUrl;
	obj->m_cleanStart = 1;
	obj->m_clientId = clientId;
	obj->m_username = username;
	obj->m_password = password;
	obj->m_keepAlive = keepAlive;
	obj->m_reconnectInterval = reconnectInterval;
	obj->m_willTopicSuffix = willTopicSuffix;
	obj->m_willTopic = willTopic;
	obj->m_willMessage = willMessage;
	obj->m_willQos = willQos;
	obj->m_willRetain = willRetain;

	obj->m_disconnectTopicSuffix = disconnectTopicSuffix;
	obj->m_disconnectTopic = disconnectTopic;
	obj->m_disconnectEdcMessage = disconnectEdcMessage;
	obj->m_disconnectQos = disconnectQos;
	obj->m_disconnectRetain = disconnectRetain;
	obj->m_birthTopicSuffix = birthTopicSuffix;
	obj->m_birthTopic = birthTopic;
	obj->m_birthEdcPayload = birthEdcPayload;
	obj->m_birthQos = birthQos;
	obj->m_birthRetain = birthRetain;
}



typedef struct
{
	char *uptime;
	char *displayName;
	char *modelName;
	char *modelId;
	char *partNumber;
	char *serialNumber;
	char *firmwareVersion;
	char *biosVersion;
	char *os;
	char *osVersion;
	char *jvmName;
	char *jvmVersion;
	char *jvmProfile;
	char *connectionInterface;
	char *connectionIp;
	double latitude;
	double longitude;
	double altitude;
} EdcDeviceProfile;


void init__EdcDeviceProfile(EdcDeviceProfile *obj,
							char *uptime, char *displayName, char *modelName,
							char *modelId, char *partNumber, char *serialNumber,
							char *firmwareVersion, char *biosVersion, char *os,
							char *osVersion, char *jvmName, char *jvmVersion,
							char *jvmProfile, char *connectionInterface, char *connectionIp,
							double latitude, double longitude, double altitude) {
	obj->uptime = uptime;
	obj->displayName = displayName;
	obj->modelName = modelName;
	obj->modelId = modelId;
	obj->partNumber = partNumber;
	obj->serialNumber = serialNumber;
	obj->firmwareVersion = firmwareVersion;
	obj->biosVersion = biosVersion;
	obj->os = os;
	obj->osVersion = osVersion;
	obj->jvmName = jvmName;
	obj->jvmVersion = jvmVersion;
	obj->jvmProfile = jvmProfile;
	obj->connectionInterface = connectionInterface;
	obj->connectionIp = connectionIp;
	obj->latitude = latitude;
	obj->longitude = longitude;
	obj->altitude = altitude;
}
#endif //_EDCCLOUDHELPERS_H
