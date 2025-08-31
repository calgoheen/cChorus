import numpy as np
import matplotlib.pyplot as plt
from scipy.io import wavfile
from scipy.signal import find_peaks


def plot_taps(filename):
    # Import WAV file
    sr, data = wavfile.read(filename)

    # Normalize
    if data.dtype != np.int32:
        print("WAV file must be encoded with int32")
        quit(1)
    data = data.astype(np.float32) / 2147483648.0

    num_frames = 64
    frame_len = len(data) // num_frames

    # Find peaks
    y = np.zeros((num_frames, 2, 2))
    for n in range(num_frames):
        for ch in range(2):
            frame = data[n * frame_len : (n + 1) * frame_len, ch]
            peak_indices = find_peaks(frame, height=0.01)[0] * 1e3 / sr
            y[n, ch, 0] = peak_indices[0]
            y[n, ch, 1] = peak_indices[1] if len(peak_indices) == 2 else peak_indices[0]

    # Plot delay time per frame
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 10))
    title = filename.rsplit(".", 1)[0]

    ax1.plot(y[:, 0, 0], "o", color="blue")
    ax1.plot(y[:, 0, 1], "o", color="blue")
    ax1.set_title(f"{title} (Left)")

    ax2.plot(y[:, 1, 0], "o", color="orange")
    ax2.plot(y[:, 1, 1], "o", color="orange")
    ax2.set_title(f"{title} (Right)")

    for ax in (ax1, ax2):
        ax.grid(True, alpha=0.3)
        ax.set_xlabel("Frame")
        ax.set_ylabel("Delay (ms)")

    plt.tight_layout()
    plt.savefig(f"{title}.png", dpi=300)
    plt.show()


plot_taps("chorus_0p5hz_100amount.wav")
plot_taps("ensemble_0p5hz_100amount.wav")
