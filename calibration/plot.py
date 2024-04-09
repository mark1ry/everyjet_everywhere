import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.collections import PatchCollection
from matplotlib.patches import Rectangle

plt.rcParams['font.family'] = 'sans-serif'
plt.rcParams['font.sans-serif'] = 'Helvetica'
plt.rcParams['font.serif'] = 'Helvetica'
plt.rcParams['font.monospace'] = 'Helvetica'
plt.rcParams['font.size'] = 10
plt.rcParams['axes.labelsize'] = 11
plt.rcParams['xtick.labelsize'] = 10
plt.rcParams['ytick.labelsize'] = 10
plt.rcParams['legend.fontsize'] = 10



def plotFlavourDistribution(ax, data: list, nentries: int, colors: list, labels: list) -> None:
    
    bar_values = [len(x.index) for x in data]
    bar_values = [x/nentries for x in bar_values]
    bar_err =  [np.sqrt(x/nentries) for x in bar_values]
    br = np.arange(len(bar_values))

    ax.yaxis.grid(True, linestyle='--', which='major', color='gray', linewidth=0.8, alpha=0.5, zorder=1)
    ax.hlines(bar_values, xmin=br, xmax=[x+1 for x in br], color=colors, zorder=3)
    errorboxes = [Rectangle((x, y-z), 1, 2*z) for x,y,z in zip(br, bar_values, bar_err)]
    pcs = [PatchCollection([errorboxes[index],], facecolor=c, alpha=0.5, zorder=3) for index, c in enumerate(colors)]
    for pc in pcs:
        ax.add_collection(pc)

    ax.set_xlim(0, 3)
    ax.set_xticks([1, 2])
    ax.set_xticklabels([])
    for index in range(len(labels)):
        ax.text(index+0.5, -0.05, labels[index], ha="center", va="center")
    ax.set_xlabel(r"Flavour", loc="right")

    ax.set_ylabel(r"Probability", loc="top")
    ax.set_ylim(0, 1)
    ax.set_title("Flavour Fraction")

    for index, element in enumerate(bar_values):
        ax.text(index+0.5, element+0.025, f"{element*100:.2f} %", ha="center", va="bottom")

    return

def plotVariableIndependent(ax, data: list, title: str, variable_name: str, nbins: int, colors: list, labels: list,
                             x_max=-99, x_min=-99, xticks=None, xlabels=None, log=False) -> list:

    results = []
    
    if x_max == -99:
        x_max = max([max(x) for x in data])
    if x_min == -99:
        x_min = min([min(x) for x in data])
    for index, element in enumerate(data):
        temp = ax.hist(element, bins=nbins, range=(x_min, x_max), histtype="step", color=colors[index], linewidth=1.5, density=True)
        ax.hist(element, bins=nbins, range=(x_min, x_max), histtype="stepfilled", color=colors[index], alpha=0.25, density=True,  label=labels[index])
        results.append(temp)
    
    padding = (x_max-x_min)/50
    ax.set_xlabel(variable_name, loc="right")
    ax.set_xlim(x_min-padding, x_max+padding)
    
    if xticks:
        ax.set_xticks(xticks)
        ax.set_xticklabels(xlabels)
    
    if log:
        ax.set_yscale("log")
        ax.set_ylabel("Log Entries [Arbitrary]", loc="top")
    else:
        ax.set_ylabel("Entries [Arbitrary]", loc="top")
    ax.set_title(title)
    ax.legend()

    return [results[y][0] for y in range(len(results))], results[0][1]

def plotVariableStacked(ax, data: list, title: str, variable_name: str, nbins: int, colors: list, labels: list,
                             x_max=-99, x_min=-99, xticks=None, xlabels=None, log=False) -> list:

    if x_max == -99:
        x_max = max([max(x) for x in data])
    if x_min == -99:
        x_min = min([min(x) for x in data])
        
    results = ax.hist(data, bins=nbins, range=(x_min, x_max), histtype="step", color=colors, linewidth=1.5, density=True, stacked=True)
    ax.hist(data, bins=nbins, range=(x_min, x_max), histtype="stepfilled", color=colors, alpha=0.25, density=True,  stacked=True, label=labels)

    padding = (x_max-x_min)/50
    ax.set_xlabel(variable_name, loc="right")
    ax.set_xlim(x_min-padding, x_max+padding)
    
    if xticks:
        ax.set_xticks(xticks)
        ax.set_xticklabels(xlabels)
    if log:
        ax.set_yscale("log")
        ax.set_ylabel("Log Entries [Arbitrary]", loc="top")
    else:
        ax.set_ylabel("Entries [Arbitrary]", loc="top")
    ax.set_title(title)
    ax.legend()

    for i in range(len(results[0])):
        if i == 0:
            continue
        else:
            for j in range(i):
                results[0][i] -= results[0][j]

    return results

def plotResiduals(ax, data: list, bins: list, labels: list, colors: list, xticks=None, xlabels=None) -> None:
    
    for i in range(len(data[0])):
        summ = 0
        for j in range(len(data)):
            summ += data[j][i]
        
        if summ != 0:
            for j in range(len(data)):
                data[j][i] = data[j][i]/summ
        
    bottom = np.zeros(len(data[0]))
    stepsize = bins[1] - bins[0]
    x_pos = [x+(stepsize/2) for x in bins[:-1]]
    for i in range(len(data)):
        ax.bar(x_pos, data[i], stepsize,color=colors[i], label=labels[i], bottom=bottom, alpha=0.6)
        bottom += data[i]
    
    x_max = bins[-1]    
    x_min = bins[0]    
    padding = (x_max-x_min)/50
    ax.set_xlim(x_min-padding, x_max+padding)
    
    if xticks:
        ax.set_xticks(xticks)
        ax.set_xticklabels(xlabels)
    
    ax.set_ylabel("Fraction", loc="top")
    ax.set_ylim(-0.05, 1.05)

    return
        
        
def plotDistributions(df, title: str, output:str, func) -> None:

    nentries = len(df.index)

    light = df[(df["flavour"]==0)]
    c = df[(df["flavour"]==4)]
    b = df[(df["flavour"]==5)]
    fl = [light, c, b]
    colors = ["green", "red", "blue"]
    labels = ["light", r"$\it{c}$", r"$\it{b}$"]

    figure, ax = plt.subplots(4, 2, figsize=(12, 12), gridspec_kw={"height_ratios": [4, 1, 4 ,1], "width_ratios": [2, 3]})
    title += ". STACKED." if func==plotVariableStacked else ". INDIVIDUAL."
    title += f" NUMBER OF JETS: {nentries}."
    figure.suptitle(title, weight='bold')

    # PLOT FLAVOUR DISTRIBUTION
    plotFlavourDistribution(ax=ax[0,0], data=fl, nentries=nentries, colors=colors, labels=labels)
    ax[1,0].axis("off")

    # PLOT SVMASS DISTRIBUTION
    sv = []
    for frame in fl:
        sv.append([x[0] for x in frame["SVmass"].to_numpy()])

    hist = func(ax=ax[0,1], data=sv, title="Secondary Vertex Mass Distribution", variable_name=r"SV Mass [MeV/$\it{c}$]",
                             nbins=30, colors=colors, labels=labels, log=True)
    plotResiduals(ax=ax[1,1], data=hist[0], bins=hist[1], labels=labels, colors=colors)

    # PLOT PT DISTRIBUTION
    pt = []
    for frame in fl:
        pt.append([x[0] for x in frame["pt"].to_numpy()])

    hist = func(ax=ax[2,1], data=pt, title="Transverse Momentum Distribution", variable_name=r"$p_{T}$ [GeV/$\it{c}^{2}$]",
                             nbins=25, colors=colors, labels=labels, x_min=0, x_max=375000,
                             xticks=[0, 50000, 100000, 150000, 200000, 250000, 300000, 350000],
                             xlabels=[0, 50, 100, 150, 200, 250, 300, 350])
    plotResiduals(ax=ax[3,1], data=hist[0], bins=hist[1], labels=labels, colors=colors)

    # PLOT PT DISTRIBUTION
    eta = []
    for frame in fl:
        eta.append([x[0] for x in frame["eta"].to_numpy()])

    hist = func(ax=ax[2,0], data=eta, title="Pseudorapidity Distribution", variable_name=r"Pseudorapidity",
                             nbins=17, colors=colors, labels=labels)
    plotResiduals(ax=ax[3,0], data=hist[0], bins=hist[1], labels=labels, colors=colors)
    

    figure.tight_layout()
    figure.savefig(output, dpi=400)
    
    return

def plotFile(input: str, title: str, output: str) -> None:

    df = pd.read_pickle(input)
    plotDistributions(df, title, output+"_independent.png", plotVariableIndependent)
    plotDistributions(df, title, output+"_stacked.png", plotVariableStacked)

    return

def producePlots(path: str, dataset_name: str, suffix: str):
    datasets_1 = ["B-ENHANCED", "C/L-ENHANCED"]
    datasets_2 = ["benhanced", "clenhanced"]
    methods = ["A", "B"]
    
    titles = []
    paths = []
    for i in range(len(datasets_1)):
        for j in range(len(methods)):
            titles.append(datasets_1[i] +". METHOD " + methods[j] + ". " + dataset_name)
            paths.append(path+"/"+datasets_2[i]+"_"+methods[j].lower()+"_"+suffix)
    for i in range(len(titles)):
        print("Iterating...")
        plotFile(paths[i]+".pkl", titles[i], paths[i])
        print("Plotted!")

if __name__ == "__main__":
    producePlots("/eos/user/m/moriolpe/mphys_project/calibration", "FOUR JETS", "4jets")
