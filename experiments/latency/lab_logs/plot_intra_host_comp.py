import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# Define the folder and file structure
folders = ["no_proxy", "http1_notls", "http1_tls"]
files = [
    "amf_nrf_disc_request_times.csv",
    "amf_pdu_session_request_times.csv",
    "amf_smf_modify_request_times.csv",
    "amf_to_ausf_aka_request_times.csv"
]

# Label mapping for x-axis
label_mapping = {
    "amf_nrf_disc_request_times.csv": "SMFDiscovery",
    "amf_pdu_session_request_times.csv": "PDUSessionSetup",
    "amf_smf_modify_request_times.csv": "PDUSessionModify",
    "amf_to_ausf_aka_request_times.csv": "5GAKAChain"
}

# Initialize a DataFrame to hold all the data
data = pd.DataFrame()

# Read each file from each folder and append to the DataFrame
for folder in folders:
    for file in files:
        file_path = os.path.join(folder, file)
        scenario = folder
        request_type = label_mapping[file]

        # Read the CSV file
        df = pd.read_csv(file_path, header=None, names=["latency"])
        df["scenario"] = scenario
        df["request_type"] = request_type

        # Append to the main DataFrame
        data = data.append(df, ignore_index=True)

# Calculate the means for each request type and scenario
mean_data = data.groupby(['request_type', 'scenario']).mean().reset_index()
mean_data['request_type'] = pd.Categorical(mean_data['request_type'],
                                            categories=label_mapping.values(),
                                            ordered=True)
mean_data = mean_data.sort_values('request_type')

# Set up the matplotlib figure
plt.figure(figsize=(8, 4))

# Define the bar width
bar_width = 0.2

# Define colors
colors = ['gray', 'white', 'black']

# Create the bar plot
for i, (scenario, color) in enumerate(zip(folders, colors)):
    # Extract means for the current scenario
    scenario_means = mean_data[mean_data['scenario'] == scenario]
    # Create an array with the positions of the bars on the x-axis
    index = np.arange(len(files)) + i * bar_width
    # Plot with the specified color and black edge
    plt.bar(index, scenario_means['latency'], width=bar_width, label=scenario, color=color, edgecolor='black')

# Apply hatching patterns to each bar
#hatches = ['///', '\\\\', '||']
#for bar, hatch in zip(plt.gca().containers, hatches):
#    for patch in bar.patches:
#        patch.set_hatch(hatch)
#        patch.set_edgecolor('black')  # Ensure the edge is black

# Customizing the plot
plt.ylabel('Latency (ms)', fontsize=14)
plt.xticks(np.arange(len(files)) + bar_width, label_mapping.values(), fontsize=14, rotation=20, ha='right')
plt.yticks(fontsize=14)

custom_legend_labels = ['No Proxy', 'HTTP1 - NO TLS', 'HTTP1 - TLS']

plt.legend(labels=custom_legend_labels, fontsize=12)

# Add text on top of each bar
for bar_container in plt.gca().containers:
    for bar in bar_container:
        height = bar.get_height()
        plt.gca().text(bar.get_x() + bar.get_width() / 2, height, f'{height:.2f}',
                       ha='center', va='bottom', fontsize=10, rotation=0)

plt.grid()

# Adjust the layout
plt.tight_layout()

# Save the plot to a PNG file
output_path = "http_req_overhead_comparison_mod.png"
output_path = "http_req_overhead_comparison_mod.eps"
plt.savefig(output_path)
plt.close()

