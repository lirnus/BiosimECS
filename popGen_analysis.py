from cProfile import label
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path
import os
from collections import defaultdict

# initialize directory path, count number of files
#directory = Path("")
with open("folderdir.txt") as file:
    folder_dir = file.readline().strip("\n")
    os.chdir(folder_dir)
    directory = Path(folder_dir)

## FUNCTIONS ##

# plot Survival, Diversity, mean Fitness
def plotSDF():
    "plot survival, diversity, mean fitness of each generation"
    survival_path = folder_dir + "/survivalrate.txt"
    fitness_path = folder_dir + "/mean_fitness.txt"
    diversity_path = folder_dir + "/diversity.txt"

    survival_data = []
    survival_gens = []
    fitness_data = []
    fitness_gens = []
    diversity_data = []
    diversity_gens = []

    with open(survival_path) as file:
        for line in file:
            elems = line.strip("\n").split(",")
            survival_gens.append(int(elems[0]))
            survival_data.append(float(elems[1]))

    with open(fitness_path) as file:
        for line in file:
            elems = line.strip("\n").split(",")
            fitness_gens.append(int(elems[0]))
            fitness_data.append(float(elems[1]))

    with open(diversity_path) as file:
        for line in file:
            elems = line.strip("\n").split(",")
            diversity_gens.append(int(elems[0]))
            diversity_data.append(float(elems[1]))
    
    #plot it
    fig, ax = plt.subplots()
    fig.set_size_inches(20, 6)
    ax.plot(survival_gens, survival_data, label="survival rate")
    ax.plot(fitness_gens, fitness_data, label="mean fitness")
    ax.plot(diversity_gens, diversity_data, label="diversity")

    ax.set_title('Population Stats')
    ax.set_xlabel('Generation')
    ax.legend()

    save_dir = folder_dir + "/pop_stats.png"
    plt.savefig(save_dir)
    save_dir2 = folder_dir + "/pop_stats.pdf"
    plt.savefig(save_dir2)


# get Lineages
def getLineages(allLines, lineages_dict, num_currentfile):
    
    generation_metagenome = []
    for i in range(1, len(allLines)): #skip first line
        elements = allLines[i].strip("\n").split(",")

        genome = tuple([int(gene, 0) for j, gene in enumerate(elements) if j > 3])
        
        frequency = int(elements[0])
        color = (int(elements[1]), int(elements[2]), int(elements[3]))
        generation_metagenome.append([genome, frequency, color])
    #print(f"generation metagenome lineages {num_currentfile}:", len(generation_metagenome))
    
    # insert the lists into a dictionary 
    for gnm, frq, col in generation_metagenome:

        if gnm in lineages_dict.keys(): #shouldnt happen
            lineages_dict[gnm][num_currentfile][0] += frq
        else:
            # change the element at num_currenfile to the frequency
            lineages_dict[gnm][num_currentfile] = [frq, col]
            
# get GeneFrqs
def getGeneFrqs(allLines, genefrqs_dict, num_currentfile):

    generation_metagenome = []
    for i in range(1, len(allLines)): #skip first line
        elements = allLines[i].strip("\n").split(",")

        genome = [int(gene, 0) for j, gene in enumerate(elements) if j > 3]
        frequency = int(elements[0])
        color = (int(elements[1]), int(elements[2]), int(elements[3]))
        for gene in genome:
            generation_metagenome.append((gene, frequency, color))
    #print(f"generation metagenome genefrqs {num_currentfile}:", len(generation_metagenome))

    
    # insert the lists into a dictionary (or list?)
    for gene, frq, col in generation_metagenome:
        if gene in genefrqs_dict.keys():
            genefrqs_dict[gene][num_currentfile][0] += frq
        else:
            # change the element at num_currenfile to the frequency
            genefrqs_dict[gene][num_currentfile] = [frq, col]

# wrapper function
def browseMetagenomeFiles():
    folder = folder_dir + "/Metagenomes"
    folder_path = Path(folder)
    if folder_path.exists():

        num_files = sum(1 for f in folder_path.iterdir() if f.is_file())
        
        genefrqs_dict = defaultdict(lambda: [[0, None] for _ in range(num_files)])
        lineages_dict = defaultdict(lambda: [[0, None] for _ in range(num_files)])

        #print(os.listdir(folder))

        for i, filename in enumerate(os.listdir(folder)):
            file_path = folder_path / filename 
            num_currentfile = i
            
            qwertz, gen = Path(filename).stem.split("-")
            generations.append(int(gen))

            # for each file in the folder, extract the genomes as a tuple or list.
            with open(file_path) as textfile:  
                allLines = textfile.readlines()

                getGeneFrqs(allLines, genefrqs_dict, num_currentfile)
                getLineages(allLines, lineages_dict, num_currentfile)
        return genefrqs_dict, lineages_dict
    else:
        print("no metagenomes saved")
        return

# plot GeneFrqs
def plot_GeneFrqs(genefrqs_dict):
    ""
    # extract the keys, values of genefrqs_dict
    genes, frq_col = map(list, zip(*genefrqs_dict.items()))
    y_frequencies = []
    y_colors = []
    for indiv in frq_col:
        frq_str, col_str = map(list, zip(*indiv))

        frqs = []
        colores = []

        getCol = (lambda lst: next((x for x in lst if isinstance(x, tuple)), None))

        for idx in range(len(frq_str)):

            col_255 = getCol(col_str)

            col = [int(i)/255 for i in col_255]
            frqs.append(int(frq_str[idx]))
            colores.append(col)

        y_frequencies.append(frqs)
        y_colors.append(colores)


    # plot it
    fig, ax = plt.subplots()
    for f, c in zip(y_frequencies, y_colors):
        getCol = (lambda lst: next((x for x in lst if isinstance(x, list)), None))
        col = getCol(c)
        
        ax.plot(generations, f, color=col) # deactivate colors=col if mutations make graph unclear

    ax.set_title('Gene Frequencies')
    ax.set_xlabel('Generation')

    save_dir = folder_dir + "/gene_frequencies.png"
    plt.savefig(save_dir)
    save_dir2 = folder_dir + "/gene_frequencies.pdf"
    plt.savefig(save_dir2)

# plot Lineages (aka Mullerplot)
def mullerplot(lineages_dict):
    ""
    # extract the keys, values of lineages_dict
    genomes, frq_col = map(list, zip(*sorted(lineages_dict.items())))
    
    y_frequencies = []
    y_colors = []
    for indiv in frq_col:
        frq_str, col_str = map(list, zip(*indiv))

        frqs = []
        colores = []

        getCol = (lambda lst: next((x for x in lst if isinstance(x, tuple)), None))

        for idx in range(len(frq_str)):

            col_255 = getCol(col_str)

            col = [int(i)/255 for i in col_255]
            frqs.append(int(frq_str[idx]))
            colores.append(col)

        y_frequencies.append(frqs)
        y_colors.append(colores)

    #print(y_colors)

    
    # plot it
    fig, ax = plt.subplots()
    fig.set_size_inches(20, 6)
    ax.stackplot(generations, y_frequencies, colors=y_colors) # deactivate colors=colores if mutations make graph unclear
    #ax.set_xlim(1, )

    ax.set_title('Muller Plot')
    ax.set_xlabel('Generation')

    save_dir = folder_dir + "/mullerplot.png"
    plt.savefig(save_dir)
    save_dir2 = folder_dir + "/mullerplot.pdf"
    plt.savefig(save_dir2)


# wrapper function
def analyse():
    print("popGen analysis...")
    genefrqs_dict, lineages_dict = browseMetagenomeFiles()

    if lineages_dict:
        mullerplot(lineages_dict)
    if genefrqs_dict:
        #print(genefrqs_dict)
        plot_GeneFrqs(genefrqs_dict)

    plotSDF()

generations = []
analyse()