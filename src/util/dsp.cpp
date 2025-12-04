#include <vector>
#include <cmath>
#include <algorithm>
#include <fftw3.h>



// Thanks to GPT for generating some DSP for us!

// ---------------- utilities ----------------

static inline double hann(int n, int N) {
    return 0.5 * (1.0 - cos(2.0 * M_PI * n / (N - 1)));
}

static inline double princarg(double x) {
    x = fmod(x + M_PI, 2.0 * M_PI);
    if (x < 0) x += 2.0 * M_PI;
    return x - M_PI;
}

static std::vector<double> linear_resample(const std::vector<double>& in, size_t outLen) {
    std::vector<double> out(outLen, 0.0);
    if (in.empty()) return out;
    if (outLen == 1) { out[0] = in[0]; return out; }

    double maxIndex = double(in.size() - 1);
    for (size_t i = 0; i < outLen; ++i) {
        double t = double(i) * maxIndex / double(outLen - 1);
        size_t i0 = size_t(floor(t));
        size_t i1 = std::min(i0 + 1, in.size() - 1);
        double frac = t - double(i0);
        out[i] = (1.0 - frac) * in[i0] + frac * in[i1];
    }
    return out;
}

static std::vector<int> find_peaks(const std::vector<double>& mag) {
    std::vector<int> peaks;
    int n = mag.size();
    for (int i = 1; i < n - 1; ++i) {
        if (mag[i] > mag[i-1] && mag[i] > mag[i+1] && mag[i] > 1e-8)
            peaks.push_back(i);
    }
    if (n > 0 && mag[0] > 1e-8) peaks.insert(peaks.begin(), 0);
    return peaks;
}

// ---------------- phase vocoder (single channel) ----------------

std::vector<double> phase_vocoder(const std::vector<double>& x, double pitch)
{
    const int N = 4096;
    const int Ha = N / 8;
    const double timeStretch = pitch;
    const int Hs = std::max(1, int(std::round(Ha * timeStretch)));

    size_t frames = x.size();
    size_t nFrames = (frames <= N) ? 1 : 1 + (frames - N) / Ha;
    size_t outEstimate = nFrames * Hs + N;

    std::vector<double> y(outEstimate, 0.0);
    std::vector<double> norm(outEstimate, 0.0);

    std::vector<double> window(N);
    for (int i = 0; i < N; ++i) window[i] = hann(i, N);

    int outBins = N/2 + 1;

    std::vector<double> inFrame(N), outFrame(N);
    std::vector<fftw_complex> spec(outBins), newSpec(outBins);

    fftw_plan fwd = fftw_plan_dft_r2c_1d(N, inFrame.data(), spec.data(), FFTW_MEASURE);
    fftw_plan inv = fftw_plan_dft_c2r_1d(N, spec.data(), outFrame.data(), FFTW_MEASURE);

    std::vector<double> prevPhase(outBins, 0.0);
    std::vector<double> sumPhase(outBins, 0.0);

    size_t outPos = 0;

    for (size_t pos = 0; pos + N <= frames; pos += Ha) {

        for (int i = 0; i < N; ++i) inFrame[i] = x[pos + i] * window[i];

        fftw_execute(fwd);

        std::vector<double> mag(outBins), phase(outBins);

        for (int k = 0; k < outBins; ++k) {
            double re = spec[k][0], im = spec[k][1];
            mag[k] = hypot(re, im);
            phase[k] = atan2(im, re);
        }

        for (int k = 0; k < outBins; ++k) {
            double omega = 2.0 * M_PI * k / N;
            double expected = omega * Ha;
            double delta = princarg(phase[k] - prevPhase[k] - expected);
            double instFreq = omega + delta / Ha;
            sumPhase[k] += instFreq * Hs;
            prevPhase[k] = phase[k];
        }

        std::vector<int> peaks = find_peaks(mag);
        const int lockW = 3;

        for (int k = 0; k < outBins; ++k) {
            newSpec[k][0] = 0;
            newSpec[k][1] = 0;
        }

        if (peaks.empty()) {
            for (int k = 0; k < outBins; ++k) {
                newSpec[k][0] = mag[k] * cos(sumPhase[k]);
                newSpec[k][1] = mag[k] * sin(sumPhase[k]);
            }
        } else {
            std::vector<int> assign(outBins, -1);
            for (int p : peaks) {
                int lo = std::max(0, p - lockW);
                int hi = std::min(outBins - 1, p + lockW);
                for (int b = lo; b <= hi; ++b) assign[b] = p;
            }

            for (int k = 0; k < outBins; ++k) {
                int pk = assign[k];
                double ph = (pk >= 0) ? sumPhase[pk] : sumPhase[k];
                newSpec[k][0] = mag[k] * cos(ph);
                newSpec[k][1] = mag[k] * sin(ph);
            }
        }

        for (int k = 0; k < outBins; ++k) {
            spec[k][0] = newSpec[k][0];
            spec[k][1] = newSpec[k][1];
        }

        fftw_execute(inv);

        for (int i = 0; i < N; ++i) {
            double s = (outFrame[i] / N) * window[i];
            if (outPos + i >= y.size()) break;
            y[outPos + i] += s;
            norm[outPos + i] += window[i] * window[i];
        }

        outPos += Hs;
    }

    y.resize(outPos + N);
    norm.resize(outPos + N);

    for (size_t i = 0; i < y.size(); ++i)
        if (norm[i] > 1e-8) y[i] /= norm[i];

    fftw_destroy_plan(fwd);
    fftw_destroy_plan(inv);

    return y;
}

// ---------------- stereo wrapper ----------------

std::pair<std::vector<double>, std::vector<double>>
pv_pitch_shift_stereo(const std::vector<double>& L,
                      const std::vector<double>& R,
                      double pitch)
{
    auto Ls = phase_vocoder(L, pitch);
    auto Rs = phase_vocoder(R, pitch);

    // resample back to original length
    size_t target = L.size();

    auto Lout = linear_resample(Ls, target);
    auto Rout = linear_resample(Rs, target);

    return {Lout, Rout};
}

std::vector<double> pv_pitch_shift_mono(const std::vector<double>& audio,
                      double pitch)
{
    auto in = phase_vocoder(audio, pitch);

    // resample back to original length
    size_t target = audio.size();

    auto out = linear_resample(in, target);

    return out;
}
