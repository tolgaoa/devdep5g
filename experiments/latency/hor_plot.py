import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
from matplotlib.patches import Patch

# Define your directories and files
environments = ['cloud_logs', 'lab_logs']
http_versions = ['http1_tls', 'http2', 'http3']
http_labels = ['HTTP1', 'HTTP2', 'HTTP3']
new_titles = [
    'AWS Deployment',
    'Lab Network'
]
http_requests_files = [
    'amf_nrf_disc_request_times',
    'amf_pdu_session_request_times',
    'amf_to_ausf_aka_request_times'
]

# New HTTP request labels
http_requests_labels = [
    'SMFDiscovery',
    'PDUSessionSetup',
    '5GAKAChain'
]

ytick_labelsize = 20
xtick_labelsize = 20
xlabelsize = 20
title_fontsize = 16
legend_fontsize = 20

group_spacing = 1.5

# Create plots for each environment
for environment, title in zip(environments, new_titles):
    # Set up the matplotlib figure and axes
    fig, ax = plt.subplots(figsize=(10, 6))

    ytick_positions = []

    for j, http_request in enumerate(http_requests_files):
        box_data = []
        for version in http_versions:
            file_path = f'./{environment}/{version}/{http_request}.csv'
            data = pd.read_csv(file_path, header=None).squeeze()
            box_data.append(data)

        # Calculate positions with additional spacing between groups
        positions = np.arange(j * len(http_versions) * group_spacing,
                              j * len(http_versions) * group_spacing + len(http_versions))
        ytick_positions.append(np.mean(positions))

        boxplot_elements = ax.boxplot(box_data, positions=positions, widths=0.6, patch_artist=True, showfliers=True, vert=False)

        colors = ['white', 'black', 'gray']

        for patch, median, color in zip(boxplot_elements['boxes'], boxplot_elements['medians'], colors):
            patch.set_facecolor(color)
            patch.set_edgecolor('black')
            median.set_color('black')

        for flier in boxplot_elements['fliers']:
            flier.set(marker='o', color='black', alpha=0.5, markersize=3)

    # Set the x-axis limit
    #ax.set_xlim(0, 200)
    ax.set_title(title, fontsize=title_fontsize)
    ax.set_xlabel('Latency (ms)', fontsize=xlabelsize)
    ax.tick_params(axis='x', labelsize=xtick_labelsize)
    ax.tick_params(axis='y', labelsize=ytick_labelsize)

    # Set y-tick labels
    ax.set_yticks(ytick_positions)
    ax.set_yticklabels(http_requests_labels, fontsize=ytick_labelsize)

    legend_elements = [
        Patch(facecolor='white', edgecolor='black', label='HTTP1'),
        Patch(facecolor='black', edgecolor='black', label='HTTP2'),
        Patch(facecolor='gray', edgecolor='black', label='HTTP3')
    ]

    ax.legend(handles=legend_elements, loc='lower right', fontsize=legend_fontsize)

    plt.grid()
    plt.tight_layout()
    output_path = f'{environment}_latency_comparison.png'
    output_path = f'{environment}_latency_comparison.eps'
    plt.savefig(output_path)
    plt.savefig(output_path.replace('.png', '.eps'))
    plt.close()

