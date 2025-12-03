# Measurement Results

Each measurement is ran three times and averaged. `-O3` flag is used for optimization.

Note: The folder containing the .wav files used for measurement testing is too big for GitHub. The zipped version can be downloaded [here](https://drive.google.com/file/d/1EDftVz8lmKevOPQ8x0MFaowk3A-2bbnf/view?usp=sharing) (LionMail required, request access if needed). Place this folder under audio_samples.

## `TrackManager::combineAll` Measurements

### Time to combine three .wav files of increasing duration
| Audio Duration (seconds) | Time (ms) |
| ------------------------ | --------  |
| **10**                   | 4.79158   |
| **30**                   | 10.9303   |
| **60**                   | 22.6066   |
| **120**                  | 47.8018   |
| **300**                  | 133.816   |
| **600**                  | 774.993   |

<img src="measurement_results/durationResults.png" alt="durationResults.png" width="500"/>


### Time to combine increasing number of .wav files
| Num WAV Files | Time (ms) |
| ------------  | --------  |
| **1**         | 18.1297   |
| **2**         | 19.4643   |
| **5**         | 24.3264   |
| **10**        | 32.1325   |
| **20**        | 59.8828   |
| **50**        | 403.936   |

<img src="measurement_results/numFilesResults.png" alt="numFilesResults.png" width="500"/>

