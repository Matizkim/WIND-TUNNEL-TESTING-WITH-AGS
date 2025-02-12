import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# Step 1: Load velocity data from a file
# Replace 'velocity_data.csv' with your actual file
# Ensure the file contains a single column of instantaneous velocity measurements (u(t)).
data = pd.read_csv('velocity_data.csv')  # Example file with velocity data
velocity = data['velocity'].values      # Column name in CSV file

# Step 2: Calculate mean velocity (ū)
mean_velocity = np.mean(velocity)

# Step 3: Calculate velocity fluctuations (u')
velocity_fluctuations = velocity - mean_velocity

# Step 4: Compute RMS of fluctuations (√(u'^2))
rms_fluctuations = np.sqrt(np.mean(velocity_fluctuations**2))

# Step 5: Calculate turbulence intensity (I)
turbulence_intensity = (rms_fluctuations / mean_velocity) * 100

# Step 6: Print results
print(f"Mean Velocity (ū): {mean_velocity:.2f} m/s")
print(f"RMS of Fluctuations (√(u'^2)): {rms_fluctuations:.2f} m/s")
print(f"Turbulence Intensity (I): {turbulence_intensity:.2f}%")

# Step 7: Plot velocity fluctuations over time
plt.figure(figsize=(10, 6))
plt.plot(velocity, label='Instantaneous Velocity (u(t))')
plt.axhline(mean_velocity, color='red', linestyle='--', label='Mean Velocity (ū)')
plt.title('Velocity Fluctuations Over Time')
plt.xlabel('Time (samples)')
plt.ylabel('Velocity (m/s)')
plt.legend()
plt.grid(True)
plt.show()
