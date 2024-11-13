import matplotlib.pyplot as plt
import numpy as np

# Define the calculate_aws_cost function
def calculate_aws_cost(requests_per_min, network_slices, t4g_small_cost_per_hour, baseline_cpu_millicores):
    baseline_req_per_min = 500  # 500 req/min as baseline
    cpu_usage_factor = requests_per_min / baseline_req_per_min
    total_cpu_millicores = baseline_cpu_millicores * cpu_usage_factor * network_slices
    t4g_small_cpu_millicores = 2000  # 2 vCPUs
    required_instances = np.ceil(total_cpu_millicores / t4g_small_cpu_millicores)  # Always round up
    total_cost = required_instances * t4g_small_cost_per_hour
    return total_cost

# AWS instance details
t4g_small_cost_per_hour = 0.0168
baseline_cpu_millicores = 80.29

# Range of requests per minute to calculate costs for
requests_per_min_range = [10000, 20000, 50000]
slice_counts = [1, 100, 500]

# Dictionary to hold the costs
costs_dict = {}

# Calculate and print the costs
for rpm in requests_per_min_range:
    costs_dict[rpm] = []
    for slices in slice_counts:
        cost = calculate_aws_cost(rpm, slices, t4g_small_cost_per_hour, baseline_cpu_millicores)
        costs_dict[rpm].append(cost)
        #print(f'Requests/min: {rpm}, Slices: {slices}, Cost: ${cost:.2f}')

# Print in a format that can be copied easily to LaTeX
print("Req/min", end="")
for slices in slice_counts:
    print(f" & {slices} slices", end="")
print(" \\\\ \\hline")

for rpm, costs in costs_dict.items():
    print(f"{rpm}", end="")
    for cost in costs:
        print(f" & ${cost:.2f}", end="")
    print(" \\\\ \\hline")

