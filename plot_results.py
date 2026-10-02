from pathlib import Path
import pandas as pd
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np
import zipfile

ROOT = Path(__file__).resolve().parent
OUT = ROOT / "output"
OUT.mkdir(exist_ok=True)
frames = []
for name, expected_rows in [("insert_extract", 90), ("batch_decrease", 180), ("large_decrease", 180)]:
    df = pd.read_csv(ROOT / "resultsUsedInReport" / (name + ".csv"))
    assert len(df) == expected_rows
    assert df.workload.eq(name).all()
    if "updates_per_item" not in df:
        df["updates_per_item"] = 0
        df["total_ms"] = df["elapsed_ms"]
    frames.append(df)
data = pd.concat(frames, ignore_index=True)
keys = ["workload", "heap", "n", "seed", "updates_per_item"]
assert not data.duplicated(keys + ["repetition"]).any()
assert data.groupby(keys).size().eq(5).all()
assert data.groupby(keys).repetition.apply(lambda x: set(x) == {1,2,3,4,5}).all()
assert np.isfinite(data.total_ms).all() and data.total_ms.gt(0).all()
updates = data[data.updates_per_item > 0]
assert np.isfinite(updates.update_ms).all()
assert updates.update_ms.gt(0).all() and updates.update_ms.le(updates.total_ms).all()
med = data.groupby(keys)[["total_ms", "update_ms"]].median().reset_index()
med.to_csv(OUT / "seed_medians.csv", index=False)
summary = med.groupby(["workload","heap","n","updates_per_item"])[["total_ms","update_ms"]].median().reset_index()
summary.to_csv(OUT / "summary.csv", index=False)
plt.rcParams.update({"font.size":10, "axes.spines.top":False, "axes.spines.right":False})
colors = {"FibonacciHeap":"#2563a6", "IndexedBinaryHeap":"#dc7027"}
labels = {"FibonacciHeap":"Fibonacci heap", "IndexedBinaryHeap":"Indexed binary heap"}

def panel(ax, workload, ratio, metric, title):
    subset = med[(med.workload == workload) & (med.updates_per_item == ratio)]
    for heap, color in colors.items():
        sub = subset[subset.heap == heap]
        grouped = sub.groupby("n")[metric]
        center = grouped.median()
        ax.plot(center.index, center.values, color=color, label=labels[heap], linewidth=1.8)
        ax.errorbar(center.index, center.values,
                    yerr=np.vstack([center.values-grouped.min().values,
                                    grouped.max().values-center.values]),
                    fmt="none", ecolor=color, elinewidth=2.8,
                    capsize=8, capthick=2.8, zorder=4)
        for seed, marker in [(42,"o"),(123,"s"),(2026,"^")]:
            s = sub[sub.seed == seed].sort_values("n")
            ax.scatter(s.n, s[metric], marker=marker, facecolors="none",
                       edgecolors=color, linewidths=1, s=22, zorder=5)
    ax.set_xscale("log")
    ax.set_yscale("log")
    ax.set_xticks([1000,10000,100000],["1,000","10,000","100,000"])
    ax.set_xlabel("Items (N)")
    ax.set_ylabel("Time (ms, log scale)")
    ax.set_title(title)
    ax.grid(True, which="major", alpha=.2)

def save(fig, name):
    fig.savefig(OUT / (name+".png"), dpi=240, facecolor="white")
    fig.savefig(OUT / (name+".pdf"), facecolor="white")
    plt.close(fig)

fig, ax = plt.subplots(figsize=(7.5,5))
panel(ax,"insert_extract",0,"total_ms","Insert and extract all items")
ax.legend()
fig.text(.5,.025,"Points: seed medians (5 repetitions each). Lines: median of seed medians.\nWhiskers: min–max of seed medians; not confidence intervals.",ha="center",fontsize=9)
fig.tight_layout(rect=[0,.10,1,1])
save(fig,"01_insert_extract")
for metric, name, title in [("update_ms","02_update_phase","Decrease-key phase"),("total_ms","03_total_workload","Complete decrease workload")]:
    fig, axes = plt.subplots(2,2,figsize=(11,8))
    for row, workload in enumerate(["batch_decrease","large_decrease"]):
        for col, ratio in enumerate([1,10]):
            panel(axes[row,col],workload,ratio,metric,("Small decreases" if row == 0 else "Large decreases")+f" — {ratio}N updates")
    handles, legend_labels = axes[0,0].get_legend_handles_labels()
    fig.legend(handles,legend_labels,loc="upper center",bbox_to_anchor=(.5,.94),ncol=2)
    fig.suptitle(title,fontsize=16,y=.99)
    fig.text(.5,.02,"Points: seed medians (circle 42; square 123; triangle 2026). Lines: median of seed medians.\nWhiskers: min–max of seed medians; not confidence intervals. Both axes use logarithmic scales.",ha="center",fontsize=9)
    fig.tight_layout(rect=[0,.08,1,.91])
    save(fig,name)
with zipfile.ZipFile(ROOT / "heap_report_figures.zip","w",zipfile.ZIP_DEFLATED) as z:
    for p in sorted(ROOT.rglob("*")):
        if p.is_file() and p.suffix in {".csv",".png",".pdf",".py"}:
            z.write(p,p.relative_to(ROOT))
print(summary[summary.n == 100000].to_string(index=False))
