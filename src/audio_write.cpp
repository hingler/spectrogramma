#include <audio_write.hpp>
#include <AudioFile.h>

#include <vector>

#include <Macro.hpp>

namespace audio_write {
  void WriteSamplesToFile(const std::string& dest, const std::vector<float>& samples) {
    AudioFile<float> res;
    AudioFile<float>::AudioBuffer channels;
    channels.push_back(samples);

    res.setSampleRate(44100);
    res.setBitDepth(16);
    res.setAudioBuffer(channels);
    res.save(dest, AudioFileFormat::Wave);
  }
}