#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "EdcCloudClient.h"

#define TEST_ACCOUNT_NAME		"bb-europe"
#define TEST_BROKER_URL			"tcp://broker-sandbox.everyware-cloud.com:1883/"

#define TEST_CLIENT_ID		"eurotech_test"
#define TEST_ASSET_ID		TEST_CLIENT_ID
#define TEST_USERNAME		"bb-europe_broker"
#define TEST_PASSWORD		"bbeuropeC!oud1"

#define	DATA_SEMANTIC_TOPIC		"nativeclient/data"
#define PUBLISH_PERIOD			10000
#define	MAX_PUBLISH				10

#define LATITUDE				46.369079
#define LONGITUDE				13.076729
#define ALTITUDE				320.0
#define PUBLISH_TIMEOUT			50000L			


//helper function to create a sample payload
EdcPayload *createPayload();

//helper function to display a payload
unsigned char displayPayload(EdcPayload *payload);

//the client instance
EdcCloudClient edcCloudClient;

int main() {

	printf ("EDC test start\r\n");

	int rc = EDCCLIENT_SUCCESS;
	char *pubTSemanticTopic = DATA_SEMANTIC_TOPIC;

	//Create client configuration, and set its properties
    EdcConfiguration conf;
	conf.m_accountName = TEST_ACCOUNT_NAME;
	conf.m_brokerUrl = TEST_BROKER_URL;
	conf.m_clientId = TEST_CLIENT_ID;
	conf.m_assetId = TEST_ASSET_ID;
	conf.m_username = TEST_USERNAME;
	conf.m_password = TEST_PASSWORD;

	//Create device profile and set its properties
    EdcDeviceProfile prof;
	prof.displayName = TEST_ASSET_ID;
	prof.modelName = "Native EDC Client";

	//set GPS position in device profile - this is sent only once, with the birth certificate
	prof.longitude = LONGITUDE;
	prof.latitude = LATITUDE;
	prof.altitude = ALTITUDE;

	//Create cloud client instance
	init__EdcCloudClient(&edcCloudClient, &conf, &prof);

	//Start the session.
	printf ("Start session...\r\n");
	rc = startSession(&edcCloudClient);

	if (rc != EDCCLIENT_SUCCESS) {
		printf("startSession failed with error code %d\r\n", rc);
		terminate(&edcCloudClient);
		return rc;
	}
	
	// PUBLISH DATA SECTION //
	int qos = 1;
	int i;
	for (i = 0; i < MAX_PUBLISH; i++) {

		EdcPayload *payload = createPayload();

		rc = publish(&edcCloudClient, pubTSemanticTopic, payload, qos, 0, 
				PUBLISH_TIMEOUT);
		
		if (rc != EDCCLIENT_SUCCESS) {
			printf("Publish #%d failed with error code %d\r\n", i, rc);
			//TODO
			//delete payload;
			goto exit;
		}
		printf("Publish #%d succeeded, semantic topic=%s\r\n", i+1, pubTSemanticTopic);

		displayPayload(payload);

		//TODO
		//delete payload;
		EdcCloudClientSleep(1 * 1000);
	}

	//sleep to allow receipt of more publishes, then terminate connection
	EdcCloudClientSleep(5 * 1000);

exit:
	rc = stopSession(&edcCloudClient);
	
	if (rc != EDCCLIENT_SUCCESS){
		printf("stopSession with error code %d", rc);
	}

	terminate(&edcCloudClient);
	printf ("EDC test completed\r\n");
	return rc;
}


/* USES MALLOC */
EdcPayload *createPayload() {
	static int counter = 0;
	counter++;

	EdcPayload edcPayload = EDC_PAYLOAD__INIT;
	edcPayload.n_metric = 1;
	edcPayload.metric = malloc(sizeof(EdcPayload__EdcMetric *) * 1);

	EdcPayload__EdcMetric edcMetric = EDC_PAYLOAD__EDC_METRIC__INIT;
	edcMetric.name = "counter";
	edcMetric.type = 3;
	edcMetric.has_int_value = 1;
	edcMetric.int_value = counter;

	EdcPayload__EdcMetric *edcMetricP = malloc(sizeof(EdcPayload__EdcMetric));
	*edcMetricP = edcMetric;
	edcPayload.metric[0] = edcMetricP;

	time_t capturedOn;
	time(&capturedOn);
	edcPayload.has_timestamp = 1;
	edcPayload.timestamp = 1000 * capturedOn;

	EdcPayload *edcPayloadP = malloc(sizeof(EdcPayload));
	*edcPayloadP = edcPayload;

	return edcPayloadP;
}


unsigned char displayPayload(EdcPayload *payload) {
	if (payload == 0) {
		return 0;
	}

	if (payload->has_timestamp) {
		printf ("  timestamp: %lld\r\n", payload->timestamp);
	}

	if (payload->position) {
		EdcPayload__EdcPosition edcPosition = *(payload->position);

		printf("  position latitude: %f\r\n", edcPosition.latitude);
		printf("  position longitude: %f\r\n", edcPosition.longitude);

		if (edcPosition.has_altitude) {
			printf("  position altitude: %f\r\n", edcPosition.altitude);
		}
		if (edcPosition.has_precision) {
			printf("  position precision: %f\r\n", edcPosition.precision);
		}
		if (edcPosition.has_heading) {
			printf("  position heading: %f\r\n", edcPosition.heading);
		}
		if (edcPosition.has_speed) {
			printf("  position speed: %f\r\n", edcPosition.speed);
		}
		if (edcPosition.has_timestamp) {
			printf("  position timestamp: %lld\r\n", edcPosition.timestamp);
		}
		if (edcPosition.has_satellites) {
			printf("  position satellites: %d\r\n", edcPosition.satellites);
		}
		if (edcPosition.has_status) {
			printf("  position status: %d\r\n", edcPosition.status);
		}
	}

	uint16_t i;
	for (i = 0; i < payload->n_metric; i++) {
		EdcPayload__EdcMetric metrictmp = *((payload->metric)[i]);

		printf ("  metric #%d name: %s\r\n", i, metrictmp.name);
		printf ("  metric #%d type: %d\r\n", i, metrictmp.type);

		if (metrictmp.has_double_value) {
			printf("  metric #%d double_value: %f\r\n", i, metrictmp.double_value);
		}
		if (metrictmp.has_float_value) {
			printf("  metric #%d float_value: %f\r\n", i, metrictmp.float_value);
		}
		if (metrictmp.has_long_value) {
			printf("  metric #%d long_value: %lld\r\n", i, metrictmp.long_value);
		}
		if (metrictmp.has_int_value) {
			printf("  metric #%d int_value: %d\r\n", i, metrictmp.int_value);
		}
		if (metrictmp.has_bool_value) {
			printf("  metric #%d bool_value: %d\r\n", i, metrictmp.bool_value);
		}
		if (metrictmp.string_value) {
			printf("  metric #%d string_value: %s\r\n", i, metrictmp.string_value);
		}
	}
	return 1;
}

