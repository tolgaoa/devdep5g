import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.dates as mdates

# Define the list of files and their labels
files = ["200req.csv", "300req.csv", "500req.csv", "2000req.csv"]
labels = ['200 req/min', '300 req/min', '500 req/min', '2000 req/min']
colors = ['blue', 'green', 'red', 'black']
markers = ['o', 's', 'x', '+']

# Text sizes
axis_label_size = 20
tick_label_size = 20
legend_size = 18

# Function to normalize the timestamp
def normalize_timestamp(df):
    min_timestamp = df['Timestamp'].min()
    df['Timestamp'] = (df['Timestamp'] - min_timestamp).dt.total_seconds() / 60  # Convert to minutes
    return df

def memory_usage_to_ki(memory_usage):
    if 'Ki' in memory_usage:
        return float(memory_usage.replace('Ki', ''))
    elif 'Mi' in memory_usage:
        return float(memory_usage.replace('Mi', '')) * 1024
    else:
        return float(memory_usage)

# CPU Utilization Plot
plt.figure(figsize=(8, 7))
for file, label, color, marker in zip(files, labels, colors, markers):
    df = pd.read_csv(file)
    df['Timestamp'] = pd.to_datetime(df['Timestamp'])
    df = normalize_timestamp(df)  # Normalize timestamps
    df.sort_values('Timestamp', inplace=True)
    df_grouped = df.groupby('Timestamp')['CPU Usage (millicores)'].mean().reset_index()

    # Calculate the average CPU usage
    average_cpu_usage = df_grouped['CPU Usage (millicores)'].mean()

    # Plot the data
    line, = plt.plot(df_grouped['Timestamp'], df_grouped['CPU Usage (millicores)'], label=label, color=color, marker=marker, linestyle='-', markersize=4)

    # Set annotation position based on the label to reduce overlap
    if label == '200 req/min':
        # Position the annotation below for 200 req/min
        vertical_alignment = 'top'
        xytext_position = (0, -20)  # Adjust text position to be below for 200 req/min
    elif label == '300 req/min':
        # Position the annotation above for 300 req/min
        vertical_alignment = 'bottom'
        xytext_position = (0, 10)
    else:
        # Position the annotation above for all other cases
        vertical_alignment = 'bottom'
        xytext_position = (0, 10)

    # Annotate the average CPU usage on the plot
    plt.annotate(f'{average_cpu_usage:.2f}',  # Format to 2 decimal places
                 xy=(max(df_grouped['Timestamp']), average_cpu_usage),
                 xytext=xytext_position, textcoords='offset points',
                 arrowprops=dict(arrowstyle='->', color=color),
                 ha='center', va=vertical_alignment, fontsize=legend_size, color=color,
                 bbox=dict(boxstyle="round,pad=0.3", edgecolor=color, facecolor='white'))

plt.xlabel('Time (Minutes)', fontsize=axis_label_size)
plt.ylabel('CPU Usage (millicores)', fontsize=axis_label_size)
plt.xticks(rotation=45, fontsize=tick_label_size)
plt.yticks(fontsize=tick_label_size)
plt.legend(fontsize=legend_size)
plt.tight_layout()
plt.savefig('cpu_utilization_plot_normalized.eps')
plt.savefig('cpu_utilization_plot_normalized.png')

# Memory Utilization Plot
plt.figure(figsize=(8, 7))
for file, label, color, marker in zip(files, labels, colors, markers):
    df = pd.read_csv(file)
    df['Timestamp'] = pd.to_datetime(df['Timestamp'])
    df = normalize_timestamp(df)  # Normalize timestamps
    df.sort_values('Timestamp', inplace=True)
    df['Memory Usage (Ki)'] = df['Memory Usage (bytes)'].apply(memory_usage_to_ki)  # Convert memory usage to Ki
    df_grouped = df.groupby('Timestamp')['Memory Usage (Ki)'].mean().reset_index()

    # Calculate the average memory usage
    average_memory_usage = df_grouped['Memory Usage (Ki)'].mean()

    # Plot the data
    line, = plt.plot(df_grouped['Timestamp'], df_grouped['Memory Usage (Ki)'], label=label, color=color, marker=marker, linestyle='-', markersize=4)

    # Set annotation position based on the label to reduce overlap
    if label == '200 req/min':
        # Position the annotation below for 200 req/min
        xytext_position = (0, -20)
        vertical_alignment = 'top'
    elif label == '300 req/min':
        # Position the annotation above for 300 req/min
        xytext_position = (0, 10)
        vertical_alignment = 'bottom'
    else:
        # Position the annotation above for all other cases
        xytext_position = (0, 10)
        vertical_alignment = 'bottom'

    # Annotate the average memory usage on the plot
    plt.annotate(f'{average_memory_usage:.2f} Ki',  # Format to 2 decimal places and add Ki for clarity
                 xy=(max(df_grouped['Timestamp']), average_memory_usage),
                 xytext=xytext_position, textcoords='offset points',
                 arrowprops=dict(arrowstyle='->', color=color),
                 ha='center', va=vertical_alignment, fontsize=legend_size, color=color,
                 bbox=dict(boxstyle="round,pad=0.3", edgecolor=color, facecolor='white'))

plt.xlabel('Time (Minutes)', fontsize=axis_label_size)
plt.ylabel('Memory Usage (Ki)', fontsize=axis_label_size)
plt.xticks(rotation=45, fontsize=tick_label_size)
plt.yticks(fontsize=tick_label_size)
plt.legend(fontsize=legend_size)
plt.tight_layout()
plt.savefig('memory_utilization_plot_normalized.eps')
plt.savefig('memory_utilization_plot_normalized.png')
plt.show()

