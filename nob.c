#define NOB_IMPLEMENTATION
#define SRC_FOLDER "src/"
#define BUILD_FOLDER "build/"
#include "src_build/nob.h"

int main(int argc, char **argv) {
  NOB_GO_REBUILD_URSELF(argc, argv);
  Nob_Cmd cmd = {0};
  nob_cmd_append(&cmd, "gcc");
  nob_cmd_append(&cmd, SRC_FOLDER "main.c");
  nob_cmd_append(&cmd, SRC_FOLDER "audio/audio_capture.c");
  nob_cmd_append(&cmd, SRC_FOLDER "audio/audio_playback.c");
  nob_cmd_append(&cmd, SRC_FOLDER "speech/speech_recognition.c");
  nob_cmd_append(&cmd, "-o", BUILD_FOLDER "Mita");
  nob_cmd_append(&cmd, "-Wall", "-Wextra", "-pedantic", "-lm", "-ldl",
                 "-lpthread", "-lvosk");
  if (!nob_cmd_run(&cmd))
    return 1;
  return 0;
}
