#define MINIAUDIO_IMPLEMENTATION
#include "audio/audio_capture.h"
#include "speech/speech_recognition.h"
#include <stdio.h>

#define SAMPLE_RATE 16000

// Callback function
void data_callback(ma_device *pDevice, void *pOutput, const void *pInput,
                   ma_uint32 frameCount) {
  SpeechContext *rec = (SpeechContext *)pDevice->pUserData;
  // 16-bit mono → 2 bytes per frame
  int bytes = frameCount * 2;

  if (speech_process_audio(rec, pInput, bytes)) {
    printf("%s\n", speech_get_result(rec));
  } else {
    printf("%s\n", speech_get_partial(rec));
  }

  (void)pOutput; // unused
}

int main(int argc, char *argv[]) {
  // Path
  char *model_path = "resources/model";

  SpeechContext *speech_ctx = create_speech(model_path, SAMPLE_RATE);

  // Check context is loaded
  if (speech_ctx == NULL) {
    fprintf(stderr,
            "ERROR: could not load model or recognizer not initialized.\n");
    return -1;
  }

  AudioCapture *capture =
      create_capture(ma_format_s16, 1, SAMPLE_RATE, data_callback, speech_ctx);

  // Check device is created
  if (capture->status != MA_SUCCESS) {
    int error = capture->status;
    fprintf(stderr, "ERROR: could not open capture device. Code: %i\n", error);
    delete_capture(capture);
    delete_speech(speech_ctx);
    return error;
  }
  printf("Press Space to stop...");

  // Check device is started
  start_capture(capture);
  if (capture->status != MA_SUCCESS) {
    int error = capture->status;
    fprintf(stderr, "ERROR: could not start capture device. Code: %i\n", error);
    delete_capture(capture);
    delete_speech(speech_ctx);
    return error;
  }
  getchar();

  delete_capture(capture);
  delete_speech(speech_ctx);
  return 0;
}
