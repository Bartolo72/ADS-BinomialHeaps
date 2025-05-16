import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import math

# Load data
df = pd.read_csv("output.csv")


# Theoretical model functions
def theoretical_links(n, c):
    return n * math.log(n)  # Rough upper-bound


def theoretical_swaps(n, c):
    m = c * math.log(n) * n
    return m * math.log(n)  # Rough estimate for decrease-key heavy ops


def theoretical_extracts(n, c):
    return n  # One extract-min per node


# Add theoretical values
df["theory_links"] = df.apply(lambda row: theoretical_links(row["n"], row["c"]), axis=1)
df["theory_swaps"] = df["avg_decrease"] * np.log(df["n"])
df["theory_extracts"] = df["n"]


# Plotting function
def plot_metric(metric, var_metric, ylabel, logscale=False):
    plt.figure(figsize=(10, 6))
    for c_val in sorted(df["c"].unique()):
        df_c = df[df["c"] == c_val].sort_values("n")
        n_vals = df_c["n"]
        mean_vals = df_c[metric]
        std_vals = np.sqrt(df_c[var_metric])
        theory_vals = df_c[f"theory_{metric.split('avg_')[1]}"]

        # Empirical mean with error band
        plt.plot(n_vals, mean_vals, marker="o", label=f"Empirical c={c_val}")
        plt.fill_between(n_vals, mean_vals - std_vals, mean_vals + std_vals, alpha=0.2)

        # Theoretical line
        plt.plot(
            n_vals, theory_vals, linestyle="--", color="gray", label=f"Theory c={c_val}"
        )

    plt.xlabel("n (number of vertices)")
    plt.ylabel(ylabel)
    plt.title(f"{ylabel} vs n")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    if logscale:
        plt.yscale("log")
    plt.show()


# Plot all metrics
plot_metric("avg_links", "var_links", "Number of Links")
plot_metric("avg_swaps", "var_swaps", "Number of Swaps", logscale=True)
plot_metric("avg_extracts", "var_extracts", "Number of Extract-Min Operations")
