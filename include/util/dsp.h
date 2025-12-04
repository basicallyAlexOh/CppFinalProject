#ifndef PV_PITCH_SHIFT_H
#define PV_PITCH_SHIFT_H

#include <vector>
// #include <utility>

std::vector<double> pv_pitch_shift_mono(const std::vector<double>& x,
                                        double pitch);

std::pair<std::vector<double>, std::vector<double>>
pv_pitch_shift_stereo(const std::vector<double>& L,
                      const std::vector<double>& R,
                      double pitch);

std::vector<double> pv_time_stretch_mono(const std::vector<double>& audio, double speed);

std::pair<std::vector<double>, std::vector<double>> pv_time_stretch_stereo(const std::vector<double>& L,
                       const std::vector<double>& R,
                       double speed);

#endif // PV_PITCH_SHIFT_H
