#include "su_jake_hackrf_Hackrf.h"
#include <libhackrf/hackrf.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static hackrf_device* device;

static JNIEnv    *callbackEnv;
static jclass    callbackClass;
static jmethodID callbackMethod;

int open_device();
int rx_callback(hackrf_transfer*);

JNIEXPORT jint JNICALL Java_su_jake_hackrf_Hackrf_open(JNIEnv *env, jclass class)
{
  if (device != NULL) {
    fprintf(stderr, "device already open, you flippin doofie.");
    return EXIT_FAILURE;
  }

  int result = open_device();
  if (result == EXIT_FAILURE) {
    device = NULL;
    fprintf(stderr, "open_device() returned a failure. clearing device pointer.");
  }
  return EXIT_SUCCESS;
}

JNIEXPORT jint JNICALL Java_su_jake_hackrf_Hackrf_close(JNIEnv *env, jclass class)
{
  if (device == NULL) {
    fprintf(stderr, "no open hackrf devices");
    return EXIT_FAILURE;
  }

  int result = hackrf_close(device);
  if (result != HACKRF_SUCCESS) {
    fprintf(stderr, "hackrf_close() failed: %s (%d)\n",
        hackrf_error_name(result), result);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

JNIEXPORT jint JNICALL Java_su_jake_hackrf_Hackrf_setFrequency
  (JNIEnv *env, jclass class, jint freq_hz)
{
  if (device == NULL) {
    fprintf(stderr, "no open hackrf devices");
    return EXIT_FAILURE;
  }
  int result = hackrf_set_freq(device, freq_hz);
  if (result != HACKRF_SUCCESS) {
    printf("hackrf_set_freq() failed: %s (%d)\n", hackrf_error_name(result), result);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

JNIEXPORT jint JNICALL Java_su_jake_hackrf_Hackrf_setSampleRate
  (JNIEnv *env, jclass class, jint freq_hz)
{
  int result = hackrf_set_sample_rate_manual(device, freq_hz, 1);
	if (result != HACKRF_SUCCESS) {
		printf("hackrf_sample_rate_set() failed: %s (%d)\n", hackrf_error_name(result), result);
		return EXIT_FAILURE;
	}

  return EXIT_SUCCESS;
}

JNIEXPORT jint JNICALL Java_su_jake_hackrf_Hackrf_startRx
  (JNIEnv *env, jclass class, jobject callback)
{
  callbackEnv = env;
  callbackClass  = (*env)->GetObjectClass(env, callback);
  if (NULL == callbackClass) {
    printf("callback method could not be found");
    return EXIT_FAILURE;
  }

  callbackMethod = (*env)->GetMethodID(env, callbackClass, "onData", "([B)V");
  if (NULL == callbackMethod) {
    printf("callback method could not be found");
    return EXIT_FAILURE;
  }

  int result = hackrf_start_rx(device, rx_callback, NULL);
  if (result != HACKRF_SUCCESS) {
    printf("hackrf_start_rx() failed: %s (%d)\n", hackrf_error_name(result), result);
    return EXIT_FAILURE;
  }

    printf("hackrf_start_rx() done\n");
  return EXIT_SUCCESS;
}

JNIEXPORT jint JNICALL Java_su_jake_hackrf_Hackrf_stopRx
  (JNIEnv *env, jclass class)
{
  int result = hackrf_stop_rx(device);
  if (result != HACKRF_SUCCESS) {
    printf("hackrf_stop_rx() failed: %s (%d)\n", hackrf_error_name(result), result);
  } else {
    printf("hackrf_stop_rx() done\n");
  }
  return EXIT_SUCCESS;
}

int open_device()
{
	int result = HACKRF_SUCCESS;
	uint8_t board_id = BOARD_ID_INVALID;
	char version[255 + 1];
	read_partid_serialno_t read_partid_serialno;
	hackrf_device_list_t *list;

	result = hackrf_init();
	if (result != HACKRF_SUCCESS) {
		fprintf(stderr, "hackrf_init() failed: %s (%d)\n",
				hackrf_error_name(result), result);
		return EXIT_FAILURE;
	}
	
	list = hackrf_device_list();
	
	if (list->devicecount < 1 ) {
		printf("No HackRF boards found.\n");
		return EXIT_FAILURE;
	}
	
  int i = 0;

  printf("Found HackRF board %d:\n", i);

  if (list->serial_numbers[i])
    printf("USB descriptor string: %s\n", list->serial_numbers[i]);

  result = hackrf_device_list_open(list, i, &device);
  if (result != HACKRF_SUCCESS) {
    fprintf(stderr, "hackrf_open() failed: %s (%d)\n",
        hackrf_error_name(result), result);
    return EXIT_FAILURE;
  }

  result = hackrf_board_id_read(device, &board_id);
  if (result != HACKRF_SUCCESS) {
    fprintf(stderr, "hackrf_board_id_read() failed: %s (%d)\n",
        hackrf_error_name(result), result);
    return EXIT_FAILURE;
  }
  printf("Board ID Number: %d (%s)\n", board_id,
      hackrf_board_id_name(board_id));

  result = hackrf_version_string_read(device, &version[0], 255);
  if (result != HACKRF_SUCCESS) {
    fprintf(stderr, "hackrf_version_string_read() failed: %s (%d)\n",
        hackrf_error_name(result), result);
    return EXIT_FAILURE;
  }
  printf("Firmware Version: %s\n", version);

  result = hackrf_board_partid_serialno_read(device, &read_partid_serialno);	
  if (result != HACKRF_SUCCESS) {
    fprintf(stderr, "hackrf_board_partid_serialno_read() failed: %s (%d)\n",
        hackrf_error_name(result), result);
    return EXIT_FAILURE;
  }
  printf("Part ID Number: 0x%08x 0x%08x\n", 
        read_partid_serialno.part_id[0],
        read_partid_serialno.part_id[1]);
  printf("Serial Number: 0x%08x 0x%08x 0x%08x 0x%08x\n", 
        read_partid_serialno.serial_no[0],
        read_partid_serialno.serial_no[1],
        read_partid_serialno.serial_no[2],
        read_partid_serialno.serial_no[3]);

	hackrf_device_list_free(list);

	return EXIT_SUCCESS;
}

int rx_callback(hackrf_transfer* transfer) {
  /* printf("got data of size %d\n", transfer->valid_length); */

  if (callbackEnv == NULL) printf("callbackEnv was null.");
  jbyteArray byteArray = (*callbackEnv)->NewByteArray(callbackEnv, transfer->valid_length);
  jbyte      *buf      = (*callbackEnv)->GetByteArrayElements(callbackEnv, byteArray, NULL);

  memcpy(buf, transfer->buffer, transfer->valid_length);

  (*callbackEnv)->ReleaseByteArrayElements(callbackEnv, byteArray, buf, 0);
  (*callbackEnv)->CallVoidMethod(callbackEnv, callbackClass, callbackMethod, byteArray);

  return 0;
}

