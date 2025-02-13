import serial
import csv
import time
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Set up serial communication
arduino_port = "COM3"  # Update with your Arduino's port
baud_rate = 115200
ser = serial.Serial(arduino_port, baud_rate, timeout=1)

# Set the output CSV file
csv_file = "Drag force.csv"

# Open the CSV file for writing
with open(csv_file, mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(["Time (ms)", "Weight (g)", "Drag Force (N)", "Velocity (m/s)", "Drag Coefficient"])

    # Create a DataFrame to store data
    data = pd.DataFrame(columns=["Time", "Velocity", "DragForce", "Cd"])

    # Real-time plotting setup
    plt.style.use("default")
    fig, ax = plt.subplots(3, 1, figsize=(10, 12))  # Three subplots

    velocity_plot, = ax[0].plot([], [], label="Velocity (m/s)", color='b')
    drag_plot, = ax[1].plot([], [], label="Drag Force (N)", color='r')
    cd_plot, = ax[2].plot([], [], label="Drag Coefficient (Cd)", color='g')

    ax[0].set_ylabel("Velocity (m/s)")
    ax[1].set_ylabel("Drag Force (N)")
    ax[2].set_ylabel("Drag Coefficient (Cd)")
    ax[2].set_xlabel("Time (s)")

    for axis in ax:
        axis.legend()
        axis.grid(True)

    # Moving average function
    def moving_average(data, window_size):
        return data.rolling(window=window_size, min_periods=1).mean()

    def update(frame):
        global data

        if ser.in_waiting > 0:
            line = ser.readline().decode("utf-8").strip()
            print(f"Received line: {line}")

            try:
                if "Weight:" in line and "Drag Force:" in line and "Velocity:" in line and "Drag Coefficient:" in line:
                    line = ' '.join(line.split())
                    data_parts = line.split(", ")

                    weight = float(data_parts[0].split(":")[1].replace(" g", "").strip())
                    drag_force = float(data_parts[1].split(":")[1].replace(" N", "").strip())
                    velocity = float(data_parts[2].split(":")[1].replace(" m/s", "").strip())
                    drag_coefficient = float(data_parts[3].split(":")[1].strip())

                    elapsed_time = (time.time() - start_time) * 1000

                    if weight >= 0 and drag_force >= 0 and velocity >= 0 and not pd.isna(drag_coefficient):
                        writer.writerow([elapsed_time, weight, drag_force, velocity, drag_coefficient])

                        new_row = {
                            "Time": elapsed_time / 1000,  # Convert ms to seconds
                            "Velocity": velocity,
                            "DragForce": drag_force,
                            "Cd": drag_coefficient
                        }
                        data = pd.concat([data, pd.DataFrame([new_row])], ignore_index=True)

                        # Smooth the data using moving average
                        window_size = 50  # You can adjust this window size for smoother data
                        smoothed_velocity = moving_average(data["Velocity"], window_size)
                        smoothed_drag_force = moving_average(data["DragForce"], window_size)
                        smoothed_cd = moving_average(data["Cd"], window_size)

                        # Update plots only if data is available
                        if not data.empty:
                            ax[0].set_xlim(0, data["Time"].max() + 5)
                            ax[1].set_xlim(0, data["Time"].max() + 5)
                            ax[2].set_xlim(0, data["Time"].max() + 5)

                            ax[0].set_ylim(0, smoothed_velocity.max() + 5)
                            ax[1].set_ylim(0, smoothed_drag_force.max() + 5)
                            ax[2].set_ylim(0, smoothed_cd.max() + 0.1)

                            # Update the plots with smoothed data
                            velocity_plot.set_data(data["Time"], smoothed_velocity)
                            drag_plot.set_data(data["Time"], smoothed_drag_force)
                            cd_plot.set_data(data["Time"], smoothed_cd)

            except ValueError as e:
                print(f"Error parsing line: {line}. Exception: {e}")

        return velocity_plot, drag_plot, cd_plot

    # Animation for real-time plotting
    ani = FuncAnimation(fig, update, interval=100, cache_frame_data=False)

    # Start collecting data
    start_time = time.time()
    plt.tight_layout()
    plt.show()
