def generate_rounded_fhss_table():
    num_channels = 50
    # Safe start/end for 250kHz signals (902.15 to 927.85)
    start_freq = 902.15
    end_freq = 927.85
    
    # Ideal math spacing
    ideal_spacing = (end_freq - start_freq) / (num_channels - 1)

    # 1. Generate the list with 2-decimal rounding
    channels = []
    for i in range(num_channels):
        freq = start_freq + (i * ideal_spacing)
        channels.append(round(freq, 2))

    # 2. Calculate true spacing between the rounded numbers
    gaps = []
    for i in range(len(channels) - 1):
        gaps.append(channels[i+1] - channels[i])
    
    true_avg_spacing = sum(gaps) / len(gaps)
    min_spacing = min(gaps)
    
    return channels, true_avg_spacing, min_spacing

# Execute
freq_list, avg_sep, min_sep = generate_rounded_fhss_table()

print(f"// 50 Frequencies spanning 902.15MHz to 927.85MHz")
print(f"// True Avg Spacing (Rounded): {round(avg_sep * 1000, 2)} kHz")
print(f"// Minimum Gap in Table: {round(min_sep * 1000, 2)} kHz")
print(f"// Compliance: Min Gap > 300kHz requirement? {'YES' if min_sep >= 0.3 else 'NO'}")
print("const float hopping_table[50] = {")
for i in range(0, len(freq_list), 5):
    line = ", ".join(f"{f:.2f}f" for f in freq_list[i:i+5])
    suffix = "," if i + 5 < len(freq_list) else ""
    print(f"    {line}{suffix}")
print("};")
