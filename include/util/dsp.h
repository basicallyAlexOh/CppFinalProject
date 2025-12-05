#ifndef PV_PITCH_SHIFT_H
#define PV_PITCH_SHIFT_H

#include <vector>
// #include <utility>

std::vector<double> pv_pitch_shift_mono(const std::vector<double>& x,
                                        double pitch, int N=4096);

std::pair<std::vector<double>, std::vector<double>>
pv_pitch_shift_stereo(const std::vector<double>& L,
                      const std::vector<double>& R,
                      double pitch, int N=4096);

std::vector<double> pv_time_stretch_mono(const std::vector<double>& audio, double speed, int N=4096);

std::pair<std::vector<double>, std::vector<double>> pv_time_stretch_stereo(const std::vector<double>& L,
                       const std::vector<double>& R,
                       double speed, int N=4096);

#endif // PV_PITCH_SHIFT_H
