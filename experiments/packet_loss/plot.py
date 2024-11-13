import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os

# Define the path to your data directory
data_dir = ''

# Define HTTP versions and packet loss rates
http_versions = ['http1', 'http2', 'http3']
packet_loss_rates = ['1percent.csv', '5percent.csv', '10percent.csv', '20percent.csv']
packet_loss_labels = ['1%', '5%', '10%', '20%']
markers = ['o', 's', '^']
colors = ['b', 'g', 'r'] 

# Function to load and filter data
def load_data(http_version, packet_loss_rate):
    file_path = os.path.join(data_dir, http_version, packet_loss_rate)
    data = pd.read_csv(file_path)
    return data[data['Success'] == 1]  # Only successful requests

# Function to plot CDF with sparse markers
def plot_cdf(data, label, color, marker, interval=10):
    sorted_data = np.sort(data)
    yvals = np.arange(1, len(sorted_data) + 1) / float(len(sorted_data))
    plt.plot(sorted_data, yvals, label=label, color=color, linewidth=2.5)
    plt.plot(sorted_data[::interval], yvals[::interval], marker=marker, color=color, linestyle='None', markersize=4)

# Plot CDF for response times, one plot for each packet loss rate
for i, packet_loss_rate in enumerate(packet_loss_rates):
    plt.figure(figsize=(5, 4))
    for j, http_version in enumerate(http_versions):
        data = load_data(http_version, packet_loss_rate)
        label = f'{http_version.upper()}'
        plot_cdf(data['Response Time (ms)'], label, colors[j], markers[j])
    plt.xlabel('Response Time (ms)')
    plt.ylabel('Cumulative Probability')
    plt.title(f'CDF of Response Time for Successful Requests ({packet_loss_labels[i]} Packet Loss)')
    plt.legend()
    plt.grid(True)
    plt.savefig(f'cdf_response_time_{packet_loss_labels[i]}.png')

# Calculate success rates and plot bar chart
success_rates = {http_version: [] for http_version in http_versions}

for http_version in http_versions:
    for packet_loss_rate in packet_loss_rates:
        file_path = os.path.join(data_dir, http_version, packet_loss_rate)
        data = pd.read_csv(file_path)
        total_requests = len(data)
        successful_requests = data['Success'].sum()
        success_rate = (successful_requests / total_requests) * 100
        success_rates[http_version].append(success_rate)

# Bar plot of success rates
x = np.arange(len(packet_loss_rates))  # The label locations
width = 0.2  # The width of the bars

fig, ax = plt.subplots(figsize=(10, 6))
rects1 = ax.bar(x - width, success_rates['http1'], width, label='HTTP/1.1')
rects2 = ax.bar(x, success_rates['http2'], width, label='HTTP/2')
rects3 = ax.bar(x + width, success_rates['http3'], width, label='HTTP/3')

# Add some text for labels, title and custom x-axis tick labels, etc.
ax.set_xlabel('Packet Loss Rate')
ax.set_ylabel('Success Rate (%)')
ax.set_title('Success Rate by HTTP Version and Packet Loss Rate')
ax.set_xticks(x)
ax.set_xticklabels(packet_loss_labels)
ax.legend()

fig.tight_layout()
plt.grid(True)
plt.savefig('success_rate_comparison.png')
